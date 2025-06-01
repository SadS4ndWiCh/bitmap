#include <endian.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: bitmap <decode | create> <PARAMS>\n");
    return 0;
  }

  if (strcmp(argv[1], "decode") == 0) {
    if (argc < 3) {
      printf("usage: bitmap decode <FILE>");
      return 0;
    }

    FILE *bitmap_file = fopen(argv[2], "rb");
    if (!bitmap_file) {
      printf("failed to open bitmap file.\n");
      return 1;
    }

    BITMAPFILEHEADER header;
    fread(&header, sizeof(header), 1, bitmap_file);

    printf("Signature: %#x\n", le16toh(header.Signature));
    printf("File Size: %d bytes\n", le32toh(header.FileSize));
    printf("Reserved1: %#x\n", header.Reserved1);
    printf("Reserved2: %#x\n", header.Reserved2);
    printf("Data Offset: %#x\n", le32toh(header.DataOffset));

    BITMAPINFOHEADER info;
    fread(&info, sizeof(info), 1, bitmap_file);

    printf("Header Size: %d\n", le32toh(info.HeaderSize));
    printf("Dimentions (width x height): %d x %d\n", le32toh(info.Width), le32toh(info.Height));
    printf("Image Size: %d bytes\n", le32toh(info.ImageSize));
    printf("Bits per Pixel: %d\n", le16toh(info.BitsPerPixel));
    printf("Pixels per Meter: %d x %d\n", le32toh(info.XpixelsPerMeter), le32toh(info.YpixelsPerMeter));
    printf("Compression Method: %d\n", le32toh(info.Compression));
    printf("Colors: %d\n", le32toh(info.NumColors));

    DWORD row_size = (DWORD) ceil((info.BitsPerPixel * info.Width) / 32.0);
    DWORD padding = row_size % 4;
    DWORD pixel_array_size = row_size * info.Height;

    printf("Data: (row_size: %d, padding: %d)\n", info.BitsPerPixel * info.Width, padding);

    for (DWORD y = 0; y < info.Height; y++) {
      for (DWORD x = 0; x < info.Width; x++) {
        BITMAPCOLOR color;
        fread(&color, sizeof(color), 1, bitmap_file);

        printf("rgb(%d, %d, %d)\n", color.Red, color.Green, color.Blue);
      }

      fseek(bitmap_file, padding, SEEK_CUR);
    }

    fclose(bitmap_file);
  } else if (strcmp(argv[1], "create") == 0) {
    if (argc < 3) {
      printf("usage: bitmap create <FILE> <WIDTH> <HEIGHT>");
      return 0;
    }

    DWORD width = atoi(argv[3]);
    DWORD height = atoi(argv[4]);

    BITMAP_Create(width, height, argv[2]);
  }

  return 0;
}
