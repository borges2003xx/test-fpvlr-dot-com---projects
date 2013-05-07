/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Carl Ljungström

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "alarms.h"

#ifdef TEXT_ENABLED
static void updateText(uint8_t textId) {
  //testPrintDebugInfo();
  uint8_t pos = 0;

  // --------------- TEXT LINE 1 (From top) -----------------------
	if (textId == 0) {
#ifdef SENSOR_VOLTAGE_1_ENABLED
		printText(gText[textId], pos+ 1, "\1521"); //Battery symbol + '1'
#endif
		pos += 4 ;

#ifdef SENSOR_VOLTAGE_1_ENABLED
		 if (!gAlarmBatt1 || gBlink1Hz) {
			 printAdc(gText[textId], pos , ANALOG_IN_1); // Voltage 1
		 }
#endif
		 pos += 8;

#ifdef SENSOR_RSSI_ENABLED
     if (gSensorRssi > 0) {
	  printText(gText[textId], pos, "\151"); //Antenna symbol (RSSI)
	  }
#endif
         pos += 6 ;
		 
#ifdef GPS_ENABLED

  		  pos = printText(gText[textId], pos, "\150"); // Satellites symbol
		
	#endif
	  pos += 2 ;	  
	  

#ifdef SHOW_GPS_IN_FLIGHT
	 printGpsNumber(gText[textId], pos, gGpsLastValidData.pos.latitude, 1); 
#endif	
  }
  // --------------- TEXT LINE 2 (From top) -----------------------
  else if (textId == 1) {
	  
#ifdef SENSOR_VOLTAGE_2_ENABLED
		if(((gSensorVoltage2.high * 100) + gSensorVoltage2.low) > 0) {
		printText(gText[textId], pos+ 1, "\1522"); //Battery symbol + '2'
		}
#endif		
		pos += 4 ;
#ifdef SENSOR_VOLTAGE_2_ENABLED
		 if(((gSensorVoltage2.high * 100) + gSensorVoltage2.low) > 0) {
     if (!gAlarmBatt2 || gBlink1Hz) {
			 printAdc(gText[textId], pos , ANALOG_IN_2); // Voltage 2
		 }
		  }
#endif
		 pos += 8;

#ifdef SENSOR_RSSI_ENABLED
      if ((!gAlarmRssi || gBlink1Hz ) && gSensorRssi > 0)
	    printRssiLevel(gText[textId], pos, SENSOR_RSSI_INPUT); 
#endif
	  pos += 6 ;


#ifdef GPS_ENABLED
    if (gGpsLastValidData.fix) {
		printNumber(gText[textId], pos, gGpsLastValidData.sats); 
		}
		else {			
		printText(gText[textId], pos, "-");
		
	  }
#endif
	  pos += 3 ;	  
	  
#ifdef SHOW_GPS_IN_FLIGHT
	 printGpsNumber(gText[textId], pos, gGpsLastValidData.pos.longitude, 0); 
#endif	
  }
  // --------------- TEXT LINE 3 (From top) -----------------------
  else if (textId == 2) {
	 
#ifdef GPS_ENABLED
	  if (gHomePosSet) {
			 pos = printText(gText[textId], pos, "\146");
			 pos = printText(gText[textId], 2, "MT");
			 pos = printNumber(gText[textId], 5 , gHomeDistance);
		}
	    else if (gBlink1Hz) {
	      pos = printText(gText[textId], 10, "NO HOME POS");
	    }
	
	
#endif //GPS_ENABLED
  }
  // --------------- TEXT LINE 4 (From top) -----------------------
  else if (textId == 3) {
#ifdef GPS_ENABLED
        printText(gText[textId], pos, "ALT"); //ALTITUDE
	 if (altitudeArrow == 1) 
	      pos = printText(gText[textId], 5, "\154");
		  
     else if (altitudeArrow == -1) 
	      pos = printText(gText[textId], 5, "\155");
		  
	 else pos = printText(gText[textId], 5, "=");
	
		 pos = printNumber(gText[textId], 7 , gGpsLastValidData.pos.altitude - gHomePos.altitude); // Altitude
	   		
	  
          pos = printText(gText[textId],TEXT_LINE_MAX_CHARS-8 , "HDG");
		  pos = printNumber(gText[textId], TEXT_LINE_MAX_CHARS-4, gGpsLastValidData.angle);
		 
#endif //GPS_ENABLED	  
	}
	// --------------- TEXT LINE 5 (From top) -----------------------
	else if (textId == 4) {
#ifdef GPS_ENABLED

	    pos = printNumber(gText[textId], 0, gGpsLastValidData.speed); // Speed
			
        pos = printText(gText[textId],5 , "KM/H");
		 
#endif //GPS_ENABLED

#ifdef TIME_HOUR_ENABLED
	  pos = printTime(gText[textId], TEXT_LINE_MAX_CHARS-9); // Time with hours
#else
    pos = printTime(gText[textId], TEXT_LINE_MAX_CHARS-6); // Time without hours
#endif //TIME_HOUR_ENABLED
	}
	// --------------- TEXT LINE 6 (From top) -----------------------
	else if (textId == 5) {

    pos =  printText(gText[textId], 0, TEXT_CALL_SIGN); // Call sign
	pos = printText(gText[textId],TEXT_LINE_MAX_CHARS-9 , "FPVLR.COM");


	}

}
#endif //TEXT_ENABLED

static void updateGrapics() {
#ifdef GPS_ENABLED
  drawArrow(gHomeBearing - gGpsLastValidData.angle);
#endif //GPS_ENABLED
}

#endif /* LAYOUT_H_ */