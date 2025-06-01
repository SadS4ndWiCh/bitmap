#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitmap.h"

void BITMAP_Create(DWORD width, DWORD height, const char *filename) {
  DWORD row_size = sizeof(BITMAPCOLOR) * width;
  DWORD padding = row_size % 4;

  DWORD pixel_array_size = (row_size + padding) * height;

  BYTE *pixel_array = (BYTE *) malloc(pixel_array_size);
  if (!pixel_array) {
    printf("failed to create pixel array buffer.\n");
    return;
  }

  DWORD index = 0;
  for (DWORD y = 0; y < height; y++) {
    for (DWORD x = 0; x < width; x++) {
      float cx = (x) / (float) width;
      float cy = (y) / (float) height;

      BYTE mag = floor(255 * ((cx + cy) / 2));

      BITMAPCOLOR color = { mag, 0, mag };
      memcpy(pixel_array + index, &color, sizeof(BITMAPCOLOR));

      index += sizeof(color);
    }

    index += padding;
  }

  BITMAPINFOHEADER info = {
    .HeaderSize = 40,
    .Width = width,
    .Height = height,
    .BitsPerPixel = sizeof(BITMAPCOLOR) * 8,
    .ImageSize = pixel_array_size * 8,
    .Planes = 1,
    .XpixelsPerMeter = 2438,
    .YpixelsPerMeter = 2438,
    .Compression = BI_RGB,
    .NumColors = 0,
    .ImportantColors = 0,
  };

  BITMAPFILEHEADER header = {
    .Signature = 0x4d42,
    .DataOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
    .FileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pixel_array_size
  };

  FILE *bitmap_file = fopen(filename, "wb");
  if (!bitmap_file) {
    printf("failed to create bitmap file.\n");
    return;
  }

  fwrite(&header, sizeof(header), 1, bitmap_file);
  fwrite(&info, sizeof(info), 1, bitmap_file);
  fwrite(pixel_array, pixel_array_size, 1, bitmap_file);

  fclose(bitmap_file);
}
