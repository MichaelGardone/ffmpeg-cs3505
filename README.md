# ffmpeg-cs3505

Created for Dr. Peter Jenson's 2018 Spring CS 3505 ("Software Practices II") class. Project contians two benchmarks: finding methods called by FFMPEG to convert to image types and implement a decoder and encoder in C to attach to FFMPEG.

## Benchmark 1

Benchmark 1 was about navigating a large code base, finding three encoding and decoding methods (JPEG, PNG, BMP) that would be called FFMPEG to convert between displayable bytes and encoding from one format to another.

## Benchmark 2

We were to write an encoder and decoder for FFMPEG in C utilizing what we learned and saw in the other decoders and encoders. We had several limitations on our compression algorithm and a strict time constraint. The format was called NICE with an extension of .nice.

Limitations:
* 1 byte per pixel
* Moderately fast
* As accurate as possible
* Six days to complete the assignment

### About Compression Format

In order to achieve the one byte per pixel limitation, we looked at two options for compressing the images (served as three bytes per pixel from FFMPEG). The two options were:
* A static 256 color table
* Dynamic generated color table based off of image contents

We decided to go with the static, 256 color table due to the time constraint. From there, we researched on the most common colors that would appear in images to make sure it looked presentable and accurate. As well, this reduced the number of bytes in the header table to just the file type (NICE), the height, and the width of the image.

With the compression in mind, we were able to just store the table in a header file that could be accessed anytime the program ran. We tested against images ranging from 10x10 to 900x900. All completed in a relatively quick amout of time. The files with bright, vibrant colors lost image quality but those with more relaxed, calmer colors (not just blacks, browns, and grays) reatined most of their original quality.
