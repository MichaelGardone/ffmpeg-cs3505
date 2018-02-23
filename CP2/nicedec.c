/*
 * BMP image format decoder
 * Copyright (c) 2005 Mans Rullgard
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

static int nice_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
  ColorTable ct[256];
  populate_ColorTable(&ct);
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    AVFrame *p         = data;
    unsigned int fsize, hsize;
    int width, height;
    unsigned int depth;
    //BiCompression comp;
    unsigned int ihsize;
    int i, j, n, linesize, ret;
    uint32_t rgb[3] = {0};
    uint32_t alpha = 0;
    uint8_t *ptr, *t_ptr;
    int dsize;

    int n_bytes_image;
    const uint8_t *buf0 = buf;
    GetByteContext gb;

    if (buf_size < 14) {
        av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    if (bytestream_get_byte(&buf) != 'N' ||
        bytestream_get_byte(&buf) != 'I' ||
	      bytestream_get_byte(&buf) != 'C' ||
	      bytestream_get_byte(&buf) != 'E') {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }

    //fsize = bytestream_get_le32(&buf);
    /*if (buf_size < fsize) {
        av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %u), trying to decode anyway\n",
               buf_size, fsize);
        fsize = buf_size;
    }*/

    /*av_log(avctx, AV_LOG_INFO, "%u\n", ihsize);

    if (ihsize + 14LL > hsize) {
        av_log(avctx, AV_LOG_ERROR, "invalid header size %u\n", hsize);
        return AVERROR_INVALIDDATA;
    }

    /* sometimes file size is set to some headers size, set a real size in that case */
    /*if (fsize == 14 || fsize == ihsize + 14)
        fsize = buf_size - 2;

    if (fsize <= hsize) {
        av_log(avctx, AV_LOG_ERROR,
               "Declared file size is less than header size (%u < %u)\n",
               fsize, hsize);
        return AVERROR_INVALIDDATA;
	}*/

    // Get width, height, and number of bytes in the image
    width = bytestream_get_le32(&buf);
    height = bytestream_get_le32(&buf);
    n_bytes_image = bytestream_get_le32(&buf);

    depth = 8;

    ret = ff_set_dimensions(avctx, width, height > 0 ? height : -(unsigned)height);
    if (ret < 0) {
        av_log(avctx, AV_LOG_ERROR, "Failed to set dimensions %d %d\n", width, height);
        return AVERROR_INVALIDDATA;
    }

    avctx->pix_fmt = AV_PIX_FMT_BGR24;

    if ((ret = ff_get_buffer(avctx, p, 0)) < 0)
        return ret;

    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;

    //buf   = buf0 + hsize;
    //dsize = buf_size - hsize;
    dsize = buf_size;

    /* Line size in file multiple of 4*/
    //n = ((avctx->width * depth + 31) / 8) & ~3;
    n = 2;

    if (n * avctx->height > dsize) {
        n = (avctx->width * depth + 7) / 8;
        if (n * avctx->height > dsize) {
            av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %d)\n",
                   dsize, n * avctx->height);
            return AVERROR_INVALIDDATA;
        }
        av_log(avctx, AV_LOG_ERROR, "data size too small, assuming missing line alignment\n");
    }

    if (height > 0) {
      //ptr      = p->data[0] + (avctx->height - 1) * p->linesize[0];
	ptr = p->data[0];
        linesize = -p->linesize[0];
    } else {
        ptr      = p->data[0];
        linesize = p->linesize[0];
    }
    
    // Translation here
    for (i = 0; i < avctx->height; i++) {
      int y;
      for(y = 0; y < avctx->width; y++) {
        int t = bytestream_get_le32(&buf);
	int bgr[3] = { ct[t].b, ct[t].g, ct[t].r };
      }
      
      buf += n;
      ptr += linesize;
    }
    // Translation here

    /*
    if (avctx->pix_fmt == AV_PIX_FMT_PAL8) {
        int colors = 1 << depth;

        memset(p->data[1], 0, 1024);

        if (ihsize >= 36) {
            int t;
            buf = buf0 + 46;
            t   = bytestream_get_le32(&buf);
            if (t < 0 || t > (1 << depth)) {
                av_log(avctx, AV_LOG_ERROR,
                       "Incorrect number of colors - %X for bitdepth %u\n",
                       t, depth);
            } else if (t) {
                colors = t;
            }
        } else {
            colors = FFMIN(256, (hsize-ihsize-14) / 3);
        }
        buf = buf0 + 14 + ihsize; //palette location
        // OS/2 bitmap, 3 bytes per palette entry
        if ((hsize-ihsize-14) < (colors << 2)) {
            if ((hsize-ihsize-14) < colors * 3) {
                av_log(avctx, AV_LOG_ERROR, "palette doesn't fit in packet\n");
                return AVERROR_INVALIDDATA;
            }
            for (i = 0; i < colors; i++)
                ((uint32_t*)p->data[1])[i] = (0xFFU<<24) | bytestream_get_le24(&buf);
        } else {
            for (i = 0; i < colors; i++)
                ((uint32_t*)p->data[1])[i] = 0xFFU << 24 | bytestream_get_le32(&buf);
        }
        buf = buf0 + hsize;
    }
    if (comp == BMP_RLE4 || comp == BMP_RLE8) {
        if (comp == BMP_RLE8 && height < 0) {
            p->data[0]    +=  p->linesize[0] * (avctx->height - 1);
            p->linesize[0] = -p->linesize[0];
        }
        bytestream2_init(&gb, buf, dsize);
        ff_msrle_decode(avctx, p, depth, &gb);
        if (height < 0) {
            p->data[0]    +=  p->linesize[0] * (avctx->height - 1);
            p->linesize[0] = -p->linesize[0];
        }
	} else {
        switch (depth) {
        case 1:
            for (i = 0; i < avctx->height; i++) {
                int j;
                for (j = 0; j < n; j++) {
                    ptr[j*8+0] =  buf[j] >> 7;
                    ptr[j*8+1] = (buf[j] >> 6) & 1;
                    ptr[j*8+2] = (buf[j] >> 5) & 1;
                    ptr[j*8+3] = (buf[j] >> 4) & 1;
                    ptr[j*8+4] = (buf[j] >> 3) & 1;
                    ptr[j*8+5] = (buf[j] >> 2) & 1;
                    ptr[j*8+6] = (buf[j] >> 1) & 1;
                    ptr[j*8+7] =  buf[j]       & 1;
                }
                buf += n;
                ptr += linesize;
            }
            break;
        case 8:
        case 24:
        case 32:
            break;
        case 4:
            for (i = 0; i < avctx->height; i++) {
                int j;
                for (j = 0; j < n; j++) {
                    ptr[j*2+0] = (buf[j] >> 4) & 0xF;
                    ptr[j*2+1] = buf[j] & 0xF;
                }
                buf += n;
                ptr += linesize;
            }
            break;
        case 16:
            for (i = 0; i < avctx->height; i++) {
                const uint16_t *src = (const uint16_t *) buf;
                uint16_t *dst       = (uint16_t *) ptr;

                for (j = 0; j < avctx->width; j++)
                    *dst++ = av_le2ne16(*src++);

                buf += n;
                ptr += linesize;
            }
            break;
        default:
            av_log(avctx, AV_LOG_ERROR, "BMP decoder is broken\n");
            return AVERROR_INVALIDDATA;
        }
    }*/
    // if (avctx->pix_fmt == AV_PIX_FMT_BGRA) {
    //     for (i = 0; i < avctx->height; i++) {
    //         int j;
    //         uint8_t *ptr = p->data[0] + p->linesize[0]*i + 3;
    //         for (j = 0; j < avctx->width; j++) {
    //             if (ptr[4*j])
    //                 break;
    //         }
    //         if (j < avctx->width)
    //             break;
    //     }
    //     if (i == avctx->height)
    //         avctx->pix_fmt = p->format = AV_PIX_FMT_BGR0;
    // }

    *got_frame = 1;

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
