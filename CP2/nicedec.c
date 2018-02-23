/*
 * NICE is a file format created for CS3505 Spring 2018 class.
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

#include <inttypes.h>

#include "avcodec.h"
#include "bytestream.h"
#include "nice.h"
#include "internal.h"
#include "msrledec.h"

/**
 * nice_decode_frame returns the size of the buffer and will set the AVCodecContext's data up correctly for a .nice file.
 * Method will return pixels in the BGR24 format. Method expects a .nice file with the correctly labeled header and will
 * return an error if such a thing is incorrect. As well, the buffer needs to have enough space to convert the .nice to a
 * computer-readable form.
 */
static int nice_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
  // Color table array
  ColorTable ct[256];
  // Populate ct with all 256 colors
  populate_ColorTable(&ct);
  // Our buffer for the data
  const uint8_t *buf = avpkt->data;
  // The size of the buffer
  int buf_size       = avpkt->size;
  // Where the pixel data goes
  AVFrame *p         = data;
  // Dimensions of the picture
  int width, height;
  // Bit/Channel depth
  unsigned int depth;
  // x, y, rgb_cnt -> used in three for loops below
  // n -> Linesize
  // ret -> Used to help initialize avctx width and height
  int x, y, rgb_cnt, n, ret;
  
  // Check if buffer size is big enough
  if (buf_size < 14) {
    av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
    return AVERROR_INVALIDDATA;
  }

  // Check to make sure the first four bytes are correctly in the NICE format
  if (bytestream_get_byte(&buf) != 'N' || bytestream_get_byte(&buf) != 'I' ||
       bytestream_get_byte(&buf) != 'C' || bytestream_get_byte(&buf) != 'E') {
    av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
    return AVERROR_INVALIDDATA;
  }

  // Get width and height
  width = bytestream_get_le32(&buf);
  height = bytestream_get_le32(&buf);

  // 8 because 8 bits per pixel (or 1 byte)
  depth = 8;

  // Set the avctx width and height dimensions using FFMPEG
  ret = ff_set_dimensions(avctx, width, height > 0 ? height : -(unsigned)height);
  if (ret < 0) {
    av_log(avctx, AV_LOG_ERROR, "Failed to set dimensions %d %d\n", width, height);
    return AVERROR_INVALIDDATA;
  }
  
  // The video type we are returning
  avctx->pix_fmt = AV_PIX_FMT_BGR24;

  // Check to make sure that the buffer isn't having issues/out of bounds
  if ((ret = ff_get_buffer(avctx, p, 0)) < 0)
     return ret;

  // Putting right settings on pixel information
  p->pict_type = AV_PICTURE_TYPE_I;
  p->key_frame = 1;

  /* Line size in file multiple of 4*/
  n = ((avctx->width * depth + 31) / 8) & ~3;

  if (n * avctx->height > buf_size) {
    n = (avctx->width * depth + 7) / 8;
    if (n * avctx->height > buf_size) {
        av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %d)\n", buf_size, n * avctx->height);
        return AVERROR_INVALIDDATA;
    }
    av_log(avctx, AV_LOG_ERROR, "data size too small, assuming missing line alignment\n");
  }

  // Translation here
  // Reverse height and width, the image mirrors and flips
  for (y = 0; y < avctx->height; y++) {
    for(x = 0; x < avctx->width; x++) {
      // Get the next byte in the stream
      int8_t t = bytestream_get_byte(&buf);
      // Get the RGB value in the color table but formatted as BGR
      int bgr[3];
      bgr[2] = ct[t].r;
      bgr[1] = ct[t].g;
      bgr[0] = ct[t].b;
      
      // Put the BGR value in the picture data
      for(rgb_cnt = 0; rgb_cnt < 3; rgb_cnt++) {
	p->data[0][y*p->linesize[0]+x*3+rgb_cnt] = bgr[rgb_cnt];
      }
    }
  }
  // Translation here

  // Success!
  *got_frame = 1;

  // Return the size of our buffer for the next encoder/ffplay
  return buf_size;
}

AVCodec ff_nice_decoder = {
    .name           = "nice",
    .long_name      = NULL_IF_CONFIG_SMALL("Nice"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_NICE,
    .decode         = nice_decode_frame,
    .capabilities   = AV_CODEC_CAP_DR1,
};
