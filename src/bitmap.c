#include <errno.h>
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
    errno = 2;
    return;
  }

  DWORD padding = (sizeof(BITMAPCOLOR) * bitmap.Info.Width) % 4;
  DWORD index = (sizeof(BITMAPCOLOR) * bitmap.Info.Width + padding) * y + sizeof(BITMAPCOLOR) * x;

  memcpy(bitmap.Pixels + index, &color, sizeof(BITMAPCOLOR));
}

BITMAPCOLOR BITMAP_get_pixel(BITMAP bitmap, DWORD x, DWORD y) {
  BITMAPCOLOR color;

  if (x >= bitmap.Info.Width || y >= bitmap.Info.Height) {
    errno = 3;
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
    errno = 4;
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
