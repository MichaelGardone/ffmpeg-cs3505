/*
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_NICE_H
#define AVCODEC_NICE_H

#include "avcodec.h"

// Color Table definition
typedef struct {
  int r;
  int g;
  int b;
} ColorTable;

static void populate_ColorTable(ColorTable (*ct)[]);
static int rgb_clamp(int val);

/*
 * Generate the color table.
 * TODO: Optimize/Clean me!
 */
static void populate_ColorTable(ColorTable (*ct)[])
{
  // NEW
  int i = 0;
  while(i < 256) {
    (*ct)[i].r = 0;
    (*ct)[i].g = 0;
    (*ct)[i].b = 0;
    i++;
  }

  (*ct)[1].r = 255;
  (*ct)[1].g = 255;
  (*ct)[1].b = 255;

  (*ct)[2].r = 255;

  (*ct)[3].g = 255;

  (*ct)[4].b = 255;

  (*ct)[5].r = 255;
  (*ct)[5].g = 255;
  
  (*ct)[6].g = 255;
  (*ct)[6].b = 255;
  
  (*ct)[7].r = 255;
  (*ct)[7].b = 255;
  
  (*ct)[8].r = 192;
  (*ct)[8].g = 192;
  (*ct)[8].b = 192;
  
  (*ct)[9].r = 128;
  (*ct)[9].g = 128;
  (*ct)[9].b = 128;

  (*ct)[10].r = 128;
  
  (*ct)[11].r = 128;
  (*ct)[11].g = 128;

  (*ct)[12].g = 128;

  (*ct)[13].r = 128;
  (*ct)[13].b = 128;
  
  (*ct)[14].g = 128;
  (*ct)[14].b = 128;

  (*ct)[15].b = 128;

  /*
  i=0;
  while (i < 256) {
    // == RED == //
    if(i == 0 || i == 5 || i == 10 || i == 15 || i == 20 || i == 25 || i == 30 || i == 35 || i == 40 || i == 45 || i == 50 || i == 91 || i == 96 || i == 101 || i == 106 || i == 111 || i == 116 || i == 121
       || i == 126 || i == 131 || i == 136 || i == 141 || i == 146 || i == 151) {
      (*ct)[i].r = 50;
    }
    else if((i > 0 && i <= 4) || (i > 5 && i <= 9) || (i > 10 && i <= 14) || (i > 15 && i <= 19) || (i > 20 && i <= 24) || (i > 25 && i <= 29) || (i > 30 && i <= 34) || (i > 35 && i <= 39)
	    || (i > 40 && i <= 44) || (i > 45 && i <= 49) || (i > 50 && i <= 54) || (i > 100 && i <= 104) || (i > 105 && i <= 109) || (i > 110 && i <= 114) || (i > 115 && i <= 119)
	    || (i > 120 && i <= 124) || (i > 125 && i <= 129) || (i > 130 && i <= 134)) {
      (*ct)[i].r = (*ct)[i-1].r+50;
    }
    
    // == GREEN == //
    if((i >= 5 && i <= 9) || i == 55 || i == 60 || i == 65 || i == 70 || i == 75 || i == 80) {
      (*ct)[i].g = 50;
    } else if(i >= 10 && i <= 14) {
      (*ct)[i].g = 100;
    } else if(i >= 15 && i <= 19) {
      (*ct)[i].g = 150;
    } else if(i >= 20 && i <= 24) {
      (*ct)[i].g = 200;
    } else if(i >= 25 && i <= 29) {
      (*ct)[i].g = 250;
    } else if((i > 55 && i <= 59) || (i > 60 && i <= 64) || (i > 65 && i <= 69) || (i > 70 && i <= 74) || (i > 75 && i <= 79) || (i > 80 && i <= 84)) {
      (*ct)[i].g = (*ct)[i-1].g + 50;
    }

    // == BLUE == //
    if((i >= 30 && i <= 34) || (i >= 60 && i <= 64) || i == 85) {
      (*ct)[i].b = 50;
    } else if((i >= 35 && i <= 39) || (i >= 65 && i <= 69)) {
      (*ct)[i].b = 100;
    } else if((i >= 40 && i <= 44) || (i >= 70 && i <= 74)) {
      (*ct)[i].b = 150;
    } else if((i >= 45 && i <= 49) || (i >= 75 && i <= 79)) {
      (*ct)[i].b = 200;
    } else if((i >= 50 && i <= 54) || (i >= 80 && i <= 84)) {
      (*ct)[i].b = 250;
    } else if(i > 85 && i <= 89) {
      (*ct)[i].b = (*ct)[i-1].b + 50;
    }

    i++;
    }*/
}

/*
 * Clamp the RGB value to a valid RGB value for the color table.
 */
static int rgb_clamp(int val)
{

  if(val >= 64 && val < 132)
    return 128;
  else if(val >= 132 && val < 205)
    return 192;
  else if(val >= 205)
    return 255;
  return 0;
}


#endif /* AVCODEC_NICE_H */
