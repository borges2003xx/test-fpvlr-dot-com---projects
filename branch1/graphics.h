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


#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "config.h"

#ifdef GRAPICSENABLED

#include "time.h"
#include "delay.h"
#include "gps.h"
#include "home.h"
#include "global.h"

#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// Graphics vars

static uint8_t gPixelData[GRAPHICS_WIDTH][GRAPHICS_HEIGHT];

// Functions

static void clearGraphics() {
	for (uint8_t x = 0; x < GRAPHICS_WIDTH; ++x) {
	  for (uint8_t y = 0; y < GRAPHICS_HEIGHT; ++y) {
			gPixelData[x][y] = 0x00;
		}		
	}
}	

inline uint8_t validPos(uint8_t x, uint8_t y) {
	if (x >= GRAPHICS_WIDTH_REAL || y >= GRAPHICS_HEIGHT) {
		return 0;
	}
	return 1;	
}

static void setPixel(uint8_t x, uint8_t y, uint8_t state) {
	if (!validPos(x, y)) {
		return;
	}
	uint8_t bitPos = 7-(x%8);
	uint8_t temp = gPixelData[x/8][y];
	if (state == 0) {
		temp &= ~(1<<bitPos);
	}
	else if (state == 1) {
		temp |= (1<<bitPos);
	}
	else {
		temp ^= (1<<bitPos);
	}
	gPixelData[x/8][y] = temp;
}

// Credit for this one goes to wikipedia! :-)
// Some mods done by me ( name, int -> uint8 )
static void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius) {
  volatile int8_t f = 1 - radius;
  volatile int8_t ddF_x = 1;
  volatile int8_t ddF_y = -2 * radius;
  volatile int8_t x = 0;
  volatile int8_t y = radius;
 
  setPixel(x0, y0 + radius, 1);
  setPixel(x0, y0 - radius, 1);
  setPixel(x0 + radius, y0, 1);
  setPixel(x0 - radius, y0, 1);
 
  while(x < y) {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    setPixel(x0 + x, y0 + y, 1);
    setPixel(x0 - x, y0 + y, 1);
    setPixel(x0 + x, y0 - y, 1);
    setPixel(x0 - x, y0 - y, 1);
    setPixel(x0 + y, y0 + x, 1);
    setPixel(x0 - y, y0 + x, 1);
    setPixel(x0 + y, y0 - x, 1);
    setPixel(x0 - y, y0 - x, 1);
  }
}

static void swap(uint8_t* a, uint8_t* b) {
	uint8_t temp = *a;
	*a = *b;
	*b = temp;
}

static void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}		 
	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}		 
	int8_t deltax = x1 - x0;
	int8_t deltay = abs(y1 - y0);
	int8_t error = deltax / 2;
	int8_t ystep;
	int8_t y = y0;
	if (y0 < y1) { 
		ystep = 1; 
	}
	else {
		ystep = -1;
	}		
	for (uint8_t x = x0; x <= x1; ++x) {
		if (steep) {
			setPixel(y, x, 1);
		}
		else { 
			setPixel(x, y, 1);
		}			
		error = error - deltay;
		if (error < 0) {
			y = y + ystep;
			error = error + deltax;
		}
	}				 
}

static void drawArrow(uint16_t angle) {
	drawCircle(GRAPHICS_MID, GRAPHICS_MID, GRAPHICS_MID);
	if (gHomeDistance < 10) {
	  drawCircle(GRAPHICS_MID, GRAPHICS_MID, 2);
	  return;
	}
	if (gGpsLastData.fix == 0) {
		drawLine(GRAPHICS_MID - (GRAPHICS_SIZE/4), GRAPHICS_MID - (GRAPHICS_SIZE/4), GRAPHICS_MID + (GRAPHICS_SIZE/4), GRAPHICS_MID + (GRAPHICS_SIZE/4));
		drawLine(GRAPHICS_MID - (GRAPHICS_SIZE/4), GRAPHICS_MID + (GRAPHICS_SIZE/4), GRAPHICS_MID + (GRAPHICS_SIZE/4), GRAPHICS_MID - (GRAPHICS_SIZE/4));
		return;
	}
	int16_t a = myCos(angle);
	int16_t b = mySin(angle);
	a = (a * (GRAPHICS_SIZE / 3)) / 100;
	b = (b * (GRAPHICS_SIZE / 3)) / 100;
	drawLine(GRAPHICS_MID - b, GRAPHICS_MID + a, GRAPHICS_MID + b, GRAPHICS_MID - a); //Direction line
	//drawLine(GRAPHICS_MID_X + a/2, GRAPHICS_MID_X + b/2, GRAPHICS_MID_X - a/2, GRAPHICS_MID_X - b/2); //Arrow bottom line
	drawLine(GRAPHICS_MID + b, GRAPHICS_MID - a, GRAPHICS_MID - a/2, GRAPHICS_MID - b/2); // Arrow "wings"
	drawLine(GRAPHICS_MID + b, GRAPHICS_MID - a, GRAPHICS_MID + a/2, GRAPHICS_MID + b/2);
}

static void drawGrapicsLine()
{
#ifdef TEXT_SMALL_ENABLED
	SPSR &= ~(1<<SPI2X); // Set normal speed
#endif //TEXT_SMALL_ENABLED
  _delay_us(GRAPHICS_OFFSET);
  uint16_t currLine = gActivePixmapLine;
  for (uint8_t i = 0; i < GRAPHICS_WIDTH; ++i) {
	  SPDR = gPixelData[i][currLine];
	  DDRB |= OUT1;
	  DELAY_9_NOP();
	  DELAY_9_NOP();
	  DELAY_5_NOP();
  }
  DDRB &= ~OUT1;
  DELAY_1_NOP();
  SPDR = 0x00;
#ifdef TEXT_SMALL_ENABLED
	SPSR |= (1<<SPI2X); // Set dual speed
#endif //TEXT_SMALL_ENABLED
}

#endif //GRAPICSENABLED  

#endif /* GRAPHICS_H_ */