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
 * MERcolorHelperANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
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
static void colorHelper(ColorTable (*ct)[], int index, int r, int g, int b);

static void colorHelper(ColorTable (*ct)[], int index, int r, int g, int b)
{
  (*ct)[index].r = r;
  (*ct)[index].g = g;
  (*ct)[index].b = b;
}

/*
 * Generate the color table.
 * TODO: Optimize/Clean me!
 */
static void populate_ColorTable(ColorTable (*ct)[])
{
  colorHelper(ct, 0,0,0,0);
  colorHelper(ct, 1,128,0,0);
  colorHelper(ct, 2,0,128,0);
  colorHelper(ct, 3,128,128,0);
  colorHelper(ct, 4,0,0,128);
  colorHelper(ct, 5,128,0,128);
  colorHelper(ct, 6,0,128,128);
  colorHelper(ct, 7,192,192,192);
  colorHelper(ct, 8,128,128,128);
  colorHelper(ct, 9,255,0,0);
  colorHelper(ct, 10,0,255,0);
  colorHelper(ct, 11,255,255,0);
  colorHelper(ct, 12,0,0,255);
  colorHelper(ct, 13,255,0,255);
  colorHelper(ct, 14,0,255,255);
  colorHelper(ct, 15,255,255,255);
  colorHelper(ct, 16,0,0,0);
  colorHelper(ct, 17,0,0,95);
  colorHelper(ct, 18,0,0,135);
  colorHelper(ct, 19,0,0,175);
  colorHelper(ct, 20,0,0,215);
  colorHelper(ct, 21,0,0,255);
  colorHelper(ct, 22,0,95,0);
  colorHelper(ct, 23,0,95,95);
  colorHelper(ct, 24,0,95,135);
  colorHelper(ct, 25,0,95,175);
  colorHelper(ct, 26,0,95,215);
  colorHelper(ct, 27,0,95,255);
  colorHelper(ct, 28,0,135,0);
  colorHelper(ct, 29,0,135,95);
  colorHelper(ct, 30,0,135,135);
  colorHelper(ct, 31,0,135,175);
  colorHelper(ct, 32,0,135,215);
  colorHelper(ct, 33,0,135,255);
  colorHelper(ct, 34,0,175,0);
  colorHelper(ct, 35,0,175,95);
  colorHelper(ct, 36,0,175,135);
  colorHelper(ct, 37,0,175,175);
  colorHelper(ct, 38,0,175,215);
  colorHelper(ct, 39,0,175,255);
  colorHelper(ct, 40,0,215,0);
  colorHelper(ct, 41,0,215,95);
  colorHelper(ct, 42,0,215,135);
  colorHelper(ct, 43,0,215,175);
  colorHelper(ct, 44,0,215,215);
  colorHelper(ct, 45,0,215,255);
  colorHelper(ct, 46,0,255,0);
  colorHelper(ct, 47,0,255,95);
  colorHelper(ct, 48,0,255,135);
  colorHelper(ct, 49,0,255,175);
  colorHelper(ct, 50,0,255,215);
  colorHelper(ct, 51,0,255,255);
  colorHelper(ct, 52,95,0,0);
  colorHelper(ct, 53,95,0,95);
  colorHelper(ct, 54,95,0,135);
  colorHelper(ct, 55,95,0,175);
  colorHelper(ct, 56,95,0,215);
  colorHelper(ct, 57,95,0,255);
  colorHelper(ct, 58,95,95,0);
  colorHelper(ct, 59,95,95,95);
  colorHelper(ct, 60,95,95,135);
  colorHelper(ct, 61,95,95,175);
  colorHelper(ct, 62,95,95,215);
  colorHelper(ct, 63,95,95,255);
  colorHelper(ct, 64,95,135,0);
  colorHelper(ct, 65,95,135,95);
  colorHelper(ct, 66,95,135,135);
  colorHelper(ct, 67,95,135,175);
  colorHelper(ct, 68,95,135,215);
  colorHelper(ct, 69,95,135,255);
  colorHelper(ct, 70,95,175,0);
  colorHelper(ct, 71,95,175,95);
  colorHelper(ct, 72,95,175,135);
  colorHelper(ct, 73,95,175,175);
  colorHelper(ct, 74,95,175,215);
  colorHelper(ct, 75,95,175,255);
  colorHelper(ct, 76,95,215,0);
  colorHelper(ct, 77,95,215,95);
  colorHelper(ct, 78,95,215,135);
  colorHelper(ct, 79,95,215,175);
  colorHelper(ct, 80,95,215,215);
  colorHelper(ct, 81,95,215,255);
  colorHelper(ct, 82,95,255,0);
  colorHelper(ct, 83,95,255,95);
  colorHelper(ct, 84,95,255,135);
  colorHelper(ct, 85,95,255,175);
  colorHelper(ct, 86,95,255,215);
  colorHelper(ct, 87,95,255,255);
  colorHelper(ct, 88,135,0,0);
  colorHelper(ct, 89,135,0,95);
  colorHelper(ct, 90,135,0,135);
  colorHelper(ct, 91,135,0,175);
  colorHelper(ct, 92,135,0,215);
  colorHelper(ct, 93,135,0,255);
  colorHelper(ct, 94,135,95,0);
  colorHelper(ct, 95,135,95,95);
  colorHelper(ct, 96,135,95,135);
  colorHelper(ct, 97,135,95,175);
  colorHelper(ct, 98,135,95,215);
  colorHelper(ct, 99,135,95,255);
  colorHelper(ct, 100,135,135,0);
  colorHelper(ct, 101,135,135,95);
  colorHelper(ct, 102,135,135,135);
  colorHelper(ct, 103,135,135,175);
  colorHelper(ct, 104,135,135,215);
  colorHelper(ct, 105,135,135,255);
  colorHelper(ct, 106,135,175,0);
  colorHelper(ct, 107,135,175,95);
  colorHelper(ct, 108,135,175,135);
  colorHelper(ct, 109,135,175,175);
  colorHelper(ct, 110,135,175,215);
  colorHelper(ct, 111,135,175,255);
  colorHelper(ct, 112,135,215,0);
  colorHelper(ct, 113,135,215,95);
  colorHelper(ct, 114,135,215,135);
  colorHelper(ct, 115,135,215,175);
  colorHelper(ct, 116,135,215,215);
  colorHelper(ct, 117,135,215,255);
  colorHelper(ct, 118,135,255,0);
  colorHelper(ct, 119,135,255,95);
  colorHelper(ct, 120,135,255,135);
  colorHelper(ct, 121,135,255,175);
  colorHelper(ct, 122,135,255,215);
  colorHelper(ct, 123,135,255,255);
  colorHelper(ct, 124,175,0,0);
  colorHelper(ct, 125,175,0,95);
  colorHelper(ct, 126,175,0,135);
  colorHelper(ct, 127,175,0,175);
  colorHelper(ct, 128,175,0,215);
  colorHelper(ct, 129,175,0,255);
  colorHelper(ct, 130,175,95,0);
  colorHelper(ct, 131,175,95,95);
  colorHelper(ct, 132,175,95,135);
  colorHelper(ct, 133,175,95,175);
  colorHelper(ct, 134,175,95,215);
  colorHelper(ct, 135,175,95,255);
  colorHelper(ct, 136,175,135,0);
  colorHelper(ct, 137,175,135,95);
  colorHelper(ct, 138,175,135,135);
  colorHelper(ct, 139,175,135,175);
  colorHelper(ct, 140,175,135,215);
  colorHelper(ct, 141,175,135,255);
  colorHelper(ct, 142,175,175,0);
  colorHelper(ct, 143,175,175,95);
  colorHelper(ct, 144,175,175,135);
  colorHelper(ct, 145,175,175,175);
  colorHelper(ct, 146,175,175,215);
  colorHelper(ct, 147,175,175,255);
  colorHelper(ct, 148,175,215,0);
  colorHelper(ct, 149,175,215,95);
  colorHelper(ct, 150,175,215,135);
  colorHelper(ct, 151,175,215,175);
  colorHelper(ct, 152,175,215,215);
  colorHelper(ct, 153,175,215,255);
  colorHelper(ct, 154,175,255,0);
  colorHelper(ct, 155,175,255,95);
  colorHelper(ct, 156,175,255,135);
  colorHelper(ct, 157,175,255,175);
  colorHelper(ct, 158,175,255,215);
  colorHelper(ct, 159,175,255,255);
  colorHelper(ct, 160,215,0,0);
  colorHelper(ct, 161,215,0,95);
  colorHelper(ct, 162,215,0,135);
  colorHelper(ct, 163,215,0,175);
  colorHelper(ct, 164,215,0,215);
  colorHelper(ct, 165,215,0,255);
  colorHelper(ct, 166,215,95,0);
  colorHelper(ct, 167,215,95,95);
  colorHelper(ct, 168,215,95,135);
  colorHelper(ct, 169,215,95,175);
  colorHelper(ct, 170,215,95,215);
  colorHelper(ct, 171,215,95,255);
  colorHelper(ct, 172,215,135,0);
  colorHelper(ct, 173,215,135,95);
  colorHelper(ct, 174,215,135,135);
  colorHelper(ct, 175,215,135,175);
  colorHelper(ct, 176,215,135,215);
  colorHelper(ct, 177,215,135,255);
  colorHelper(ct, 178,215,175,0);
  colorHelper(ct, 179,215,175,95);
  colorHelper(ct, 180,215,175,135);
  colorHelper(ct, 181,215,175,175);
  colorHelper(ct, 182,215,175,215);
  colorHelper(ct, 183,215,175,255);
  colorHelper(ct, 184,215,215,0);
  colorHelper(ct, 185,215,215,95);
  colorHelper(ct, 186,215,215,135);
  colorHelper(ct, 187,215,215,175);
  colorHelper(ct, 188,215,215,215);
  colorHelper(ct, 189,215,215,255);
  colorHelper(ct, 190,215,255,0);
  colorHelper(ct, 191,215,255,95);
  colorHelper(ct, 192,215,255,135);
  colorHelper(ct, 193,215,255,175);
  colorHelper(ct, 194,215,255,215);
  colorHelper(ct, 195,215,255,255);
  colorHelper(ct, 196,255,0,0);
  colorHelper(ct, 197,255,0,95);
  colorHelper(ct, 198,255,0,135);
  colorHelper(ct, 199,255,0,175);
  colorHelper(ct, 200,255,0,215);
  colorHelper(ct, 201,255,0,255);
  colorHelper(ct, 202,255,95,0);
  colorHelper(ct, 203,255,95,95);
  colorHelper(ct, 204,255,95,135);
  colorHelper(ct, 205,255,95,175);
  colorHelper(ct, 206,255,95,215);
  colorHelper(ct, 207,255,95,255);
  colorHelper(ct, 208,255,135,0);
  colorHelper(ct, 209,255,135,95);
  colorHelper(ct, 210,255,135,135);
  colorHelper(ct, 211,255,135,175);
  colorHelper(ct, 212,255,135,215);
  colorHelper(ct, 213,255,135,255);
  colorHelper(ct, 214,255,175,0);
  colorHelper(ct, 215,255,175,95);
  colorHelper(ct, 216,255,175,135);
  colorHelper(ct, 217,255,175,175);
  colorHelper(ct, 218,255,175,215);
  colorHelper(ct, 219,255,175,255);
  colorHelper(ct, 220,255,215,0);
  colorHelper(ct, 221,255,215,95);
  colorHelper(ct, 222,255,215,135);
  colorHelper(ct, 223,255,215,175);
  colorHelper(ct, 224,255,215,215);
  colorHelper(ct, 225,255,215,255);
  colorHelper(ct, 226,255,255,0);
  colorHelper(ct, 227,255,255,95);
  colorHelper(ct, 228,255,255,135);
  colorHelper(ct, 229,255,255,175);
  colorHelper(ct, 230,255,255,215);
  colorHelper(ct, 231,255,255,255);
  colorHelper(ct, 232,8,8,8);
  colorHelper(ct, 233,18,18,18);
  colorHelper(ct, 234,28,28,28);
  colorHelper(ct, 235,38,38,38);
  colorHelper(ct, 236,48,48,48);
  colorHelper(ct, 237,58,58,58);
  colorHelper(ct, 238,68,68,68);
  colorHelper(ct, 239,78,78,78);
  colorHelper(ct, 240,88,88,88);
  colorHelper(ct, 241,98,98,98);
  colorHelper(ct, 242,108,108,108);
  colorHelper(ct, 243,118,118,118);
  colorHelper(ct, 244,128,128,128);
  colorHelper(ct, 245,138,138,138);
  colorHelper(ct, 246,148,148,148);
  colorHelper(ct, 247,158,158,158);
  colorHelper(ct, 248,168,168,168);
  colorHelper(ct, 249,178,178,178);
  colorHelper(ct, 250,188,188,188);
  colorHelper(ct, 251,198,198,198);
  colorHelper(ct, 252,208,208,208);
  colorHelper(ct, 253,218,218,218);
  colorHelper(ct, 254,228,228,228);
  colorHelper(ct, 255,238,238,238);
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
