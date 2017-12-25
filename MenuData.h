#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

/*
Generated using LCD Menu Builder at http://lcd-menu-bulder.cohesivecomputing.co.uk/.
*/

enum sampleMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdLcdLightTimeout,
  mnuCmdSensorRoot,
  mnuCmdAudioRoot,
  mnuCmdAudioSelect,
  mnuCmdAudioThreshold,
  mnuCmdAudioDelay,
  mnuCmdDigitalRoot,
  mnuCmdDigitalSelect,
  mnuCmdDigitalDelay,
  mnuCmdAccelGyroRoot,
  mnuCmdAccelGyroSelect,
  mnuCmdAccelGyroThreshold,
  mnuCmdAccelGyroDelay,
  mnuCmdCameraRoot,
  mnuCmdActivationDelay,
  mnuCmdShutterRoot,
  mnuCmdShutterTime,
  mnuCmdShutterSustain,
  mnuCmdResetToDefaults
};

PROGMEM const char sampleMenu_back[] = "Back";
PROGMEM const char sampleMenu_exit[] = "Exit";

PROGMEM const char sampleMenu_2_1_1[] = "2.1.1 Selected";
PROGMEM const char sampleMenu_2_1_2[] = "2.1.2 Threshold";
PROGMEM const char sampleMenu_2_1_3[] = "2.1.3 Delay";
PROGMEM const MenuItem sampleMenu_List_2_1[] = {{mnuCmdAudioSelect, sampleMenu_2_1_1}, {mnuCmdAudioThreshold, sampleMenu_2_1_2}, {mnuCmdAudioDelay, sampleMenu_2_1_3}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_2_1[] = "2.2.1 Selected";
PROGMEM const char sampleMenu_2_2_2[] = "2.2.2 Delay";
PROGMEM const MenuItem sampleMenu_List_2_2[] = {{mnuCmdDigitalSelect, sampleMenu_2_2_1}, {mnuCmdDigitalDelay, sampleMenu_2_2_2}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_3_1[] = "2.3.1 Selected";
PROGMEM const char sampleMenu_2_3_2[] = "2.3.2 Threshold";
PROGMEM const char sampleMenu_2_3_3[] = "2.3.3 Delay";
PROGMEM const MenuItem sampleMenu_List_2_3[] = {{mnuCmdAccelGyroSelect, sampleMenu_2_3_1}, {mnuCmdAccelGyroThreshold, sampleMenu_2_3_2}, {mnuCmdAccelGyroDelay, sampleMenu_2_3_3}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_3_2_1[] = "3.2.1 Open time";
PROGMEM const char sampleMenu_3_2_2[] = "3.2.2 Sustain";
PROGMEM const MenuItem sampleMenu_List_3_2[] = {{mnuCmdShutterTime, sampleMenu_3_2_1}, {mnuCmdShutterSustain, sampleMenu_3_2_2}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_1[] = "2.1 Audio";
PROGMEM const char sampleMenu_2_2[] = "2.2 Digital";
PROGMEM const char sampleMenu_2_3[] = "2.3 Accel/Gyro";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdAudioRoot, sampleMenu_2_1, sampleMenu_List_2_1, menuCount(sampleMenu_List_2_1)}, {mnuCmdDigitalRoot, sampleMenu_2_2, sampleMenu_List_2_2, menuCount(sampleMenu_List_2_2)}, {mnuCmdAccelGyroRoot, sampleMenu_2_3, sampleMenu_List_2_3, menuCount(sampleMenu_List_2_3)}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_3_1[] = "3.1 Delay";
PROGMEM const char sampleMenu_3_2[] = "3.2 Shutter";
PROGMEM const MenuItem sampleMenu_List_3[] = {{mnuCmdActivationDelay, sampleMenu_3_1}, {mnuCmdShutterRoot, sampleMenu_3_2, sampleMenu_List_3_2, menuCount(sampleMenu_List_3_2)}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1[] = "1 LCD light";
PROGMEM const char sampleMenu_2[] = "2 Sensor";
PROGMEM const char sampleMenu_3[] = "3 Camera";
PROGMEM const char sampleMenu_4[] = "4 Reset";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdLcdLightTimeout, sampleMenu_1}, {mnuCmdSensorRoot, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, {mnuCmdCameraRoot, sampleMenu_3, sampleMenu_List_3, menuCount(sampleMenu_List_3)}, {mnuCmdResetToDefaults, sampleMenu_4}, {mnuCmdBack, sampleMenu_exit}};

/*
case mnuCmdLcdLightTimeout :
  break;
case mnuCmdAudioSelect :
  break;
case mnuCmdAudioThreshold :
  break;
case mnuCmdAudioDelay :
  break;
case mnuCmdDigitalSelect :
  break;
case mnuCmdDigitalDelay :
  break;
case mnuCmdAccelGyroSelect :
  break;
case mnuCmdAccelGyroThreshold :
  break;
case mnuCmdAccelGyroDelay :
  break;
case mnuCmdActivationDelay :
  break;
case mnuCmdShutterTime :
  break;
case mnuCmdShutterSustain :
  break;
case mnuCmdResetToDefaults :
  break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Config IdPrefix="mnuCmd" VarPrefix="sampleMenu" UseNumbering="true" IncludeNumberHierarchy="true"
          MaxNameLen="15" MenuBackFirstItem="false" BackText="Back" ExitText="Exit" AvrProgMem="true" />
  <MenuItems>
    <Item Id="LcdLightTimeout" Name="LCD light"/>
    <Item Id="SensorRoot" Name="Sensor">
      <MenuItems>
        <Item Id="AudioRoot" Name="Audio">
          <MenuItems>
            <Item Id="AudioSelect" Name="Selected"/>
            <Item Id="AudioThreshold" Name="Threshold"/>
            <Item Id="AudioDelay" Name="Delay"/>
          </MenuItems>
        </Item>
        <Item Id="DigitalRoot" Name="Digital">
          <MenuItems>
            <Item Id="DigitalSelect" Name="Selected"/>
            <Item Id="DigitalDelay" Name="Delay"/>
          </MenuItems>
        </Item>
        <Item Id="AccelGyroRoot" Name="Accel/Gyro">
          <MenuItems>
            <Item Id="AccelGyroSelect" Name="Selected"/>
            <Item Id="AccelGyroThreshold" Name="Threshold"/>
            <Item Id="AccelGyroDelay" Name="Delay"/>
          </MenuItems>
        </Item>
      </MenuItems>
    </Item>
    <Item Id="CameraRoot" Name="Camera">
      <MenuItems>
        <Item Id="ActivationDelay" Name="Delay" />
        <Item Id="ShutterRoot" Name="Shutter">
          <MenuItems>
            <Item Id="ShutterTime" Name="Open time"/>
            <Item Id="ShutterSustain" Name="Sustain" />
          </MenuItems>
        </Item>
      </MenuItems>
    </Item>
    <Item Id="ResetToDefaults" Name="Reset"/>
  </MenuItems>
</RootMenu>

*/
#endif

