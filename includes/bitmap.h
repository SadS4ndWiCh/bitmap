#ifndef __BITMAP_H
#define __BITMAP_H

#include "types.h"

// Compression Methods
#define BI_RGB            0
#define BI_RLE8           1
#define BI_RLE4           2
#define BI_BITFIELDS      3
#define BI_JPEG           4
#define BI_PNG            5
#define BI_ALPHABITFIELDS 6
#define BI_CYMK           11
#define BI_CYMKRLE8       12
#define BI_CYMKRLE4       13

#pragma pack(1)
typedef struct {
  WORD  Signature;
  DWORD FileSize;
  WORD  Reserved1; 
  WORD  Reserved2; 
  DWORD DataOffset;
} BITMAPFILEHEADER;

#pragma pack(1)
typedef struct {
  DWORD HeaderSize;
  DWORD Width, Height;
  WORD  Planes;
  WORD  BitsPerPixel;
  DWORD Compression;
  DWORD ImageSize;
  DWORD XpixelsPerMeter, YpixelsPerMeter;
  DWORD NumColors;
  DWORD ImportantColors;
} BITMAPINFOHEADER;

typedef struct {
  BYTE Blue;
  BYTE Green;
  BYTE Red;
} BITMAPCOLOR;

typedef struct {
  BITMAPFILEHEADER Header;
  BITMAPINFOHEADER Info;
  BYTE *Pixels;
} BITMAP;

BITMAP BITMAP_init(DWORD width, DWORD height);

void BITMAP_set_pixel(BITMAP bitmap, DWORD x, DWORD y, BITMAPCOLOR color);
BITMAPCOLOR BITMAP_get_pixel(BITMAP bitmap, DWORD x, DWORD y);

void BITMAP_save(BITMAP bitmap,  const char *path);
void BITMAP_free(BITMAP bitmap);

#endif
