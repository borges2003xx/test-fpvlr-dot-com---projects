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

#ifndef GPS_COMMON_H_
#define GPS_COMMON_H_

#include "home.h"

#ifdef GPS_ENABLED

static int8_t altitudeArrow = 0;
static TGpsData gGpsLastValidData = {};
static uint8_t gGpsValidData = 0;
static TGpsData gGpsLastData = {};
static TTime gLastFix = {};

static void setupGps() {
	// USART setup
	UBRR0H = (uint8_t)(GPS_UBRR>>8); // set baud
	UBRR0L = (uint8_t)GPS_UBRR;
	UCSR0C = (3<<UCSZ00); // 8N1
	UCSR0B = (1<<RXEN0); // Enable RX
}

static void setHomePos() {

	gHomePos = gGpsLastValidData.pos;
	gHomePosSet = 1;
}



static void finishGpsDecoding() {
	
if (gGpsLastData.pos.altitude > gGpsLastValidData.pos.altitude) 
altitudeArrow = 1;
		
else if (gGpsLastData.pos.altitude < gGpsLastValidData.pos.altitude)
altitudeArrow = -1;


	if (gGpsLastData.checksumValid != 0) {

		gGpsLastValidData = gGpsLastData;
		gGpsValidData = 1;
		gLastFix = gTime;
		
		

		if (gHomePosSet == 0) {
			if (gGpsLastValidData.fix != 0) {
#ifdef HOME_SET_AT_FIX
        if (gHomeFixCount >= HOME_SET_FIX_COUNT) {
			    if (gGpsLastValidData.sats >= HOME_SET_MIN_SATS) { // (Improved thanks to Yury Smirnov)
			      setHomePos();
				  }				  
	      }
		    else {
			    ++gHomeFixCount;
		    }
#endif //HOME_FIRST_FIX
#ifdef HOME_AUTO_SET
        if (gGpsLastValidData.speed >= HOME_FIX_MIN_SPEED) {
			    setHomePos();
		    }
#endif //HOME_AUTO_SET
      }
		}	  

	}		  
}

#endif //GPS_ENABLED


#endif /* GPS_COMMON_H_ */