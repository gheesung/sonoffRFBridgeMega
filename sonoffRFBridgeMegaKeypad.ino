/*
  esp01_sonoff_rfbridge_uno.ino - RF Bridge for Tasmota and Arduino

  Copyright (C) 2017  Ghee Sung

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Acknowledgement to 
 *  Theo Arends for Tasmota
 *  Suat Özgür for RCSwitch
 *  SlashDevin for NeoSWSerial
 */


#include <RCSwitch.h>
#include <LiquidCrystal.h>
#include "LcdKeypad.h"
#include "MenuData.h"

#define INPUT_BUFFER_SIZE      128          // Max number of characters in (serial) command buffer
#define rxPin 4
#define txPin 5
#define LED_BUILTIN1 7
#define IR_RECV_PIN  11

unsigned int  byteRecv =0;
byte startText = 0;
byte endText = 0;
int firstbyte = 0;
int stackcounter = 0;
unsigned long recvData = 0;

RCSwitch mySwitch = RCSwitch(); 


// array to store the message
char svalue[INPUT_BUFFER_SIZE];
char stack[INPUT_BUFFER_SIZE];


// LCD Keypad
enum AppModeValues
{
  APP_NORMAL_MODE,
  APP_MENU_MODE,
  APP_PROCESS_MENU_CMD
};

byte appMode = APP_NORMAL_MODE;

MenuManager Menu1(sampleMenu_Root, menuCount(sampleMenu_Root));

char strbuf[LCD_COLS + 1]; // one line of lcd display
byte btn;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void refreshMenuDisplay (byte refreshMode);
byte getNavAction(); 

void setup() {
  Serial.begin(19200);
  Serial.println("DIY Sonoff RF Bridge");
  Serial3.begin(19200);
  
  mySwitch.enableReceive(2);  
  mySwitch.enableTransmit(20);  
  mySwitch.setProtocol(1);
  pinMode(LED_BUILTIN1, OUTPUT);

  // LCD Init
  backLightOn();
  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_COLS, LCD_ROWS);

  // fall in to menu mode by default.
  appMode = APP_MENU_MODE;
  refreshMenuDisplay(REFRESH_DESCEND);

  setBacklightBrightness(1); 
}



void loop(){
  // this loop is for Tasmota to talk to RF bridge
  espArduinoBridge();
  
  // This loop is for the RF receiver
  // As long as the RF receiver cna decode the data, it will be send to Tasmota.
  recvRFSwitch();
  if (0 != recvData){
    processRecvData();
    recvData = 0;
  }

}

void espArduinoBridge(){
  if (Serial3.available()){
    
    //Serial.println("received triggered");
    while(Serial3.available()){

      svalue[byteRecv] = (byte)Serial3.read();;
      //Serial.print((byte) svalue[byteRecv], HEX);
      //Serial.print(" ");
      byteRecv++;
      // prevent buffer overflow. 
      // when ESP boot, it sends the log thru' UART
      if (byteRecv>= INPUT_BUFFER_SIZE) { 
        byteRecv = 0; 
        break; 
      }

    } 

    svalue[byteRecv] = '\0';
    Serial3.flush();

    //Serial.print(" receive byte:");
    //Serial.println(byteRecv, HEX);
    //Serial.println(svalue);
    if (byteRecv){
      
      // start of text message 
      if ((byte)svalue[0] == 170){
        startText=1;
        stack[0] = svalue[0];
      }
      // the rest
      if ( startText){
        for(int i=0;i<byteRecv;i++){
          stack[stackcounter] = svalue[i];
          stackcounter++;

          // end byte (85)
          if ((byte)svalue[i] == 85){  endText =1;  stack[stackcounter]=0;  }
        }
      }
      // detect end text
      if(endText){
        // dump out the stack
        dumpStack();
        stackcounter = 0;
        endText = 0;

        // end of command character detected
        //Serial.println("Processing Commands");
        processCommand();
      }
      // 
      byteRecv =0;
    }
  }
}

// process the command from Tasmota
void processCommand(){
  // Learn Mode
  if (((byte)stack[0] == 170) && ((byte)stack[1] == 161) && ((byte)stack[2] == 85)){
    Serial.println("Learn Mode ");
    processLearnMode();
  }

  // normal mode
  if (((byte)stack[0] == 170) && ((byte)stack[1] == 165) && ((byte)stack[2] == 32)){
    sendRFSwitch();
    recvData=0;
    stack[0] = 0;
  }

}

// Learn mode
void processLearnMode(){
    Serial.println("Command --> Start Learning");
    delay(1000);
    //recvRFSwitch();
    
    int i = 0;
    while (recvData == 0){
      recvRFSwitch();

      if (recvData != 0){ break;}
      // no signal received, send learn fail
      if (i > 200) {
        Serial.println("Learned code fail");
        Serial3.write(0xAA);
        Serial3.write(0xA2);
        Serial3.write(0x55);
        digitalWrite(LED_BUILTIN1,LOW);
        return;
      }
      //flash the LED for 20s when learn mode is enabled.
      digitalWrite(LED_BUILTIN1,HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN1,LOW);
      i++;
    }

    // some preamble from SONOFF bridge
    // may make use of this in future.
    Serial3.write(0xAA);
    Serial3.write(0xA3);
    Serial3.write(0x20);
    Serial3.write(0xF8);
    Serial3.write(0x01);
    Serial3.write(0x18);
    Serial3.write(0x03);
    Serial3.write(0x3E);

    // actual data
    Serial3.write((byte)((unsigned long)recvData>>16));
    Serial3.write((byte)((unsigned long)recvData>>8));
    Serial3.write((byte)recvData);

    Serial3.write(0x55);
    stack[0] = 0;
    Serial.println("Command --> Learned code sent");
    recvData =0;
    delay(1000);

}

// process recv data from the RF
void processRecvData(){

  // some preamble from SONOFF bridge
  Serial3.write(0xAA);
  Serial3.write(0xA4);
  Serial3.write(0x20);
  Serial3.write(0xEE);
  Serial3.write(0x01);
  Serial3.write(0x18);
  Serial3.write(0x03);
  Serial3.write(0x3E);

  // actual data
  Serial3.write((byte)((unsigned long)recvData>>16));
  Serial3.write((byte)((unsigned long)recvData>>8));
  Serial3.write((byte)recvData);
  
  // postamble
  Serial3.write(0x55);
  recvData=0;
  stack[0] = 0;
}

// receive the data from the remote RF switch
void recvRFSwitch(){
  if (mySwitch.available()) {
    Serial.println( mySwitch.getReceivedProtocol() );

    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.println("Unknown encoding");
    } else {
      
      Serial.print("Receiver Received: ");
      recvData = mySwitch.getReceivedValue();
      Serial.println( recvData, HEX );
      //output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());      
    }
    mySwitch.resetAvailable();
  }
}

// Send the data out (RF signal out).
void sendRFSwitch(){

  unsigned long sendData = 0UL;
 
  sendData = (byte)stack[8];
  sendData = sendData<<16 ;
  sendData = sendData | ((byte)stack[9]<<8);
  sendData = sendData | (byte)stack[10];
  Serial.print("Transmitter Sent: " );
  Serial.println( sendData, HEX );
  mySwitch.send(sendData, 24);
  stack[0] = 0;
  recvData = 0;
}


// for testing
void setuptest(){
  Serial.begin(19200);
  Serial.println("Init");
  //attachInterrupt(digitalPinToInterrupt(rxPin), handleRxChar, CHANGE);
  Serial3.begin(19200);
}

void looptest(){
  if (Serial3.available()) {
    Serial.write(Serial3.read());
  }
  if (Serial.available()) {
    Serial3.write(Serial.read());
  }
  
  //recvRFSwitch();
  mySwitch.send(8991298, 24);
}

void simulateSend(){
     Serial.println("Command --> Simulated signal");
    //A4 20 EE 01 18 03 3E 2E 1A 22 55

      /*Serial3.write(0xAA);
      Serial3.write(0xA4);
      Serial3.write(0x20);
      Serial3.write(0xEE);
      Serial3.write(0x01);
      Serial3.write(0x18);
      Serial3.write(0x03);
      Serial3.write(0x3E);
      Serial3.write(0x2E);
      Serial3.write(0x1A);
      Serial3.write(0x22);
      Serial3.write(0x55);*/

    Serial3.write(0xAA);
    Serial3.write(0xA3);
    Serial3.write(0x20);
    Serial3.write(0xF8);
    Serial3.write(0x01);
    Serial3.write(0x18);
    Serial3.write(0x03);
    Serial3.write(0x3E);
    Serial3.write(0x2E);
    Serial3.write(0x1A);
    Serial3.write(0x22);
    Serial3.write(0x55);      
    delay(5000);
  
}
void dumpStack(){
  // dump out the stack
  int i = 0;
  Serial.println("Stack Dump");
  while (stack[i] != 0){
    Serial.print((byte) stack[i],HEX);
    Serial.print(" ");
    i++;
  }
  Serial.println(" ");
}

//----------------------------------------------------------------------
const char EmptyStr[] = "";

// Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
void refreshMenuDisplay (byte refreshMode)
{
  char nameBuf[LCD_COLS+1];

/*
  if (refreshMode == REFRESH_DESCEND || refreshMode == REFRESH_ASCEND)
  {
    byte menuCount = Menu1.getMenuItemCount();
    
    // uncomment below code to output menus to serial monitor
    if (Menu1.currentMenuHasParent())
    {
      Serial.print("Parent menu: ");
      Serial.println(Menu1.getParentItemName(nameBuf));
    }
    else
    {
      Serial.println("Main menu:");
    }
    
    for (int i=0; i<menuCount; i++)
    {
      Serial.print(Menu1.getItemName(nameBuf, i));

      if (Menu1.itemHasChildren(i))
      {
        Serial.println("->");
      }
      else
      {
        Serial.println();
      }
    }
  }
*/

  lcd.setCursor(0, 0);
  if (Menu1.currentItemHasChildren())
  {
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    strbuf[LCD_COLS-1] = 0b01111110;            // Display forward arrow if this menu item has children.
    lcd.print(strbuf);
    lcd.setCursor(0, 1);
    lcd.print(rpad(strbuf, EmptyStr));          // Clear config value in display
  }
  else
  {
    byte cmdId;
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    
    if ((cmdId = Menu1.getCurrentItemCmdId()) == 0)
    {
      strbuf[LCD_COLS-1] = 0b01111111;          // Display back arrow if this menu item ascends to parent.
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print(rpad(strbuf, EmptyStr));        // Clear config value in display.
    }
    else
    {
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      
      // TODO Display config value.
    }
  }
}


