#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitmap.h"

BITMAP BITMAP_init(DWORD width, DWORD height) {
  BITMAP bitmap;

  DWORD pixels_row_bytes = sizeof(BITMAPCOLOR) * width;
  DWORD pixels_row_padding = pixels_row_bytes % 4;
  DWORD pixels_array_bytes = (pixels_row_bytes + pixels_row_padding) * height;

  bitmap.Pixels = (BYTE *) malloc(pixels_array_bytes);
  if (!bitmap.Pixels) {
    errno = 1;
    return bitmap;
  }

  memset(bitmap.Pixels, 0, pixels_array_bytes);

  bitmap.Header.Signature  = 0x4d42;
  bitmap.Header.DataOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bitmap.Header.FileSize   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pixels_array_bytes;

  bitmap.Info.HeaderSize      = 40;
  bitmap.Info.Width           = width;
  bitmap.Info.Height          = height;
  bitmap.Info.BitsPerPixel    = sizeof(BITMAPCOLOR) * 8;
  bitmap.Info.ImageSize       = pixels_array_bytes;
  bitmap.Info.Planes          = 1;
  bitmap.Info.XpixelsPerMeter = 2438;
  bitmap.Info.YpixelsPerMeter = 2438;
  bitmap.Info.Compression     = BI_RGB;
  bitmap.Info.NumColors       = 0;
  bitmap.Info.ImportantColors = 0;

  return bitmap;
}

void BITMAP_set_pixel(BITMAP bitmap, DWORD x, DWORD y, BITMAPCOLOR color) {
  if (x >= bitmap.Info.Width || y >= bitmap.Info.Height) {
    return;
  }

  DWORD padding = (sizeof(BITMAPCOLOR) * bitmap.Info.Width) % 4;
  DWORD index = (sizeof(BITMAPCOLOR) * bitmap.Info.Width + padding) * y + sizeof(BITMAPCOLOR) * x;

  memcpy(bitmap.Pixels + index, &color, sizeof(BITMAPCOLOR));
}

BITMAPCOLOR BITMAP_get_pixel(BITMAP bitmap, DWORD x, DWORD y) {
  BITMAPCOLOR color;

  if (x >= bitmap.Info.Width || y >= bitmap.Info.Height) {
    errno = 2;
    return color;
  }

  DWORD padding = (sizeof(BITMAPCOLOR) * bitmap.Info.Width) % 4;
  DWORD index = (sizeof(BITMAPCOLOR) * bitmap.Info.Width + padding) * y + sizeof(BITMAPCOLOR) * x;

  memcpy(&color, bitmap.Pixels + index, sizeof(BITMAPCOLOR));
  return color;
}

void BITMAP_save(BITMAP bitmap, const char *path) {
  FILE *bitmap_file = fopen(path, "wb");
  if (!bitmap_file) {
    printf("failed to create bitmap file.\n");
    return;
  }

  fwrite(&bitmap.Header, sizeof(BITMAPFILEHEADER), 1, bitmap_file);
  fwrite(&bitmap.Info, sizeof(BITMAPINFOHEADER), 1, bitmap_file);
  fwrite(bitmap.Pixels, bitmap.Info.ImageSize, 1, bitmap_file);

  fclose(bitmap_file);
}

void BITMAP_free(BITMAP bitmap) {
  free(bitmap.Pixels);
}

/*
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
*/
