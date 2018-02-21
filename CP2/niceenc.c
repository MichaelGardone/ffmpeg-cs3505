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

#include "libavutil/imgutils.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bytestream.h"
#include "nice.h"
#include "internal.h"

static const uint32_t monoblack_pal[] = { 0x000000, 0xFFFFFF };
static const uint32_t rgb565_masks[]  = { 0xF800, 0x07E0, 0x001F };
static const uint32_t rgb444_masks[]  = { 0x0F00, 0x00F0, 0x000F };

static av_cold int nice_encode_init(AVCodecContext *avctx){

     switch (avctx->pix_fmt) {
     case AV_PIX_FMT_BGR24:
	 avctx->bits_per_coded_sample = 24;
	 break;
     default:
	 av_log(avctx, AV_LOG_INFO, "unsupported pixel format\n");
	 return AVERROR(EINVAL);
     }

     return 0;
 }

static int nice_encode_frame(AVCodecContext *avctx, AVPacket *pkt, const AVFrame *pict, int *got_packet)
{
     const AVFrame * const p = pict;
     // hsize - header size
     // n_bytes - size of file/number of bytes in file
     // n_bytes_image - size of pixel array
     // i - for loop
     // n - second for loop, internal for loop
     // ret - return value for error
     int n_bytes_image, n_bytes_per_row, n_bytes, i, n, hsize, ret;
     // pal - ??????
     const uint32_t *pal = NULL;
     // 256 color table
     uint32_t palette256[256];
     // pad_bytes_per_row - padding between rows
     // pal_entires - ?????
     int pad_bytes_per_row, pal_entries = 0;
     // bit count - color channel
     int bit_count = avctx->bits_per_coded_sample;
     // *buf - buffer pointer
     // *ptr - points to the end of the file
     uint8_t *ptr, *buf;

     // GUARD AGAINST DEPRECATED CODE
 #if FF_API_CODED_FRAME
 FF_DISABLE_DEPRECATION_WARNINGS
     avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;
     avctx->coded_frame->key_frame = 1;
 FF_ENABLE_DEPRECATION_WARNINGS
 #endif
   // GUARD AGAINST DEPRECATED CODE

     if (pal && !pal_entries)pal_entries = 1 << bit_count;
 
     n_bytes_per_row = ((int64_t)avctx->width * (int64_t)bit_count + 7LL) >> 3LL;
     pad_bytes_per_row = (4 - n_bytes_per_row) & 3;
     n_bytes_image = avctx->height * (n_bytes_per_row + pad_bytes_per_row);

     // Size of the NICE Header and NICE Info Header(IH)
 #define SIZE_BITMAPFILEHEADER 4
 #define SIZE_BITMAPINFOHEADER 8
    hsize = SIZE_BITMAPFILEHEADER + SIZE_BITMAPINFOHEADER + (pal_entries << 2);
    n_bytes = n_bytes_image + hsize;
    
    // Checking to see if memory can be alloacted to a packet for putting in file
    if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes, 0)) < 0)
        return ret;
    
    buf = pkt->data;

    // == INSERT NICE HEADER == //
    bytestream_put_byte(&buf, 'N');
    bytestream_put_byte(&buf, 'I'); 
    bytestream_put_byte(&buf, 'C');
    bytestream_put_byte(&buf, 'E');
    // == INSERT NICE HEADER == //
    // == INSERT FILE SIZE == //
    bytestream_put_le32(&buf, n_bytes);
    // == INSERT FILE SIZE == //
    // == INSERT HEADER SIZE == //
    bytestream_put_le32(&buf, hsize);
    // == INSERT HEADER SIZE == //
    // == INSERT IH SIZE == //
    bytestream_put_le32(&buf, SIZE_BITMAPINFOHEADER);
    // == INSERT IH SIZE == //
    // == INSERT WIDTH AND HEIGHT == //
    bytestream_put_le32(&buf, avctx->width);
    bytestream_put_le32(&buf, avctx->height);
    // == INSERT WIDTH AND HEIGHT == //
    //bytestream_put_le16(&buf, 1);                     // BITMAPINFOHEADER.biPlanes
    //bytestream_put_le16(&buf, bit_count);             // BITMAPINFOHEADER.biBitCount
    // == SIZE OF PIXEL ARRAY == //
    bytestream_put_le32(&buf, n_bytes_image);
    // == SIZE OF PIXEL ARRAY == //

    for (i = 0; i < pal_entries; i++)
        bytestream_put_le32(&buf, pal[i] & 0xFFFFFF);

    // BMP files are bottom-to-top so we start from the end...
    ptr = p->data[0] + (avctx->height - 1) * p->linesize[0];
    buf = pkt->data + hsize;

    // All encoding an compression goes here
    // Loop through height
    for(i = 0; i < avctx->height; i++) {
        if (bit_count == 16) {
            const uint16_t *src = (const uint16_t *) ptr;
            uint16_t *dst = (uint16_t *) buf;
	    
	    // Loop through width
            for(n = 0; n < avctx->width; n++)
                AV_WL16(dst + n, src[n]);
        } else {
            memcpy(buf, ptr, n_bytes_per_row);
        }
        buf += n_bytes_per_row;
        memset(buf, 0, pad_bytes_per_row);
        buf += pad_bytes_per_row;
        ptr -= p->linesize[0]; // ... and go back
    }

    pkt->flags |= AV_PKT_FLAG_KEY;
    *got_packet = 1;
    return 0;
}

AVCodec ff_nice_encoder = {
    .name           = "nice",
    .long_name      = NULL_IF_CONFIG_SMALL("BMP (Windows and OS/2 bitmap)"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_NICE,
    .init           = nice_encode_init,
    .encode2        = nice_encode_frame,
    .pix_fmts       = (const enum AVPixelFormat[]){
        AV_PIX_FMT_BGR24,
        AV_PIX_FMT_NONE
    },
};
