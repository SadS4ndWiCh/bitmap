#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

#include "bitmap.h"

void decode(char *path) {
  errno = 0;

  BITMAP bitmap = BITMAP_open(path);
  if (errno != 0) {
    printf("failed to open BITMAP image file.");
    return;
  }

  printf("Signature: %#x\n", le16toh(bitmap.Header.Signature));
  printf("File Size: %d bytes\n", le32toh(bitmap.Header.FileSize));
  printf("Reserved1: %#x\n", bitmap.Header.Reserved1);
  printf("Reserved2: %#x\n", bitmap.Header.Reserved2);
  printf("Data Offset: %#x\n", le32toh(bitmap.Header.DataOffset));

  printf("Header Size: %d\n", le32toh(bitmap.Info.HeaderSize));
  printf("Dimentions (width x height): %d x %d\n", le32toh(bitmap.Info.Width), le32toh(bitmap.Info.Height));
  printf("Image Size: %d bytes\n", le32toh(bitmap.Info.ImageSize));
  printf("Bits per Pixel: %d\n", le16toh(bitmap.Info.BitsPerPixel));
  printf("Pixels per Meter: %d x %d\n", le32toh(bitmap.Info.XpixelsPerMeter), le32toh(bitmap.Info.YpixelsPerMeter));
  printf("Compression Method: %d\n", le32toh(bitmap.Info.Compression));
  printf("Colors: %d\n", le32toh(bitmap.Info.NumColors));

  BITMAP_free(bitmap);
}

void create(DWORD width, DWORD height, char *path) {
  BITMAP bitmap = BITMAP_init(width, height);

  for (DWORD y = 0; y < height; y++) {
    for (DWORD x = 0; x < width; x++) {
      DWORD gray = 255 * ((x + y) / 510.0);

      BITMAP_set_pixel(bitmap, x, y, (BITMAPCOLOR) { gray, gray, gray });
    }
  }

  BITMAP_save(bitmap, path);
  BITMAP_free(bitmap);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf(
        "bitmap <COMMAND> [ARGUMENTS]\n\n"
        "Available commands:\n"
        "   decode <FILE>                  - Decode the BITMAP image and view the data.\n"
        "   create <FILE> <WIDTH> <HEIGHT> - Create a monochromatic grayscale BITMAP image.\n\n"
        "Examples:\n"
        "   bitmap decode sample.bmp\n"
        "   bitmap create created_image.bmp 200 200\n"
    );
    return 0;
  }

  if (strcmp(argv[1], "decode") == 0) {
    if (argc < 3) {
      printf("usage: bitmap decode <FILE>");
      return 0;
    }
    
    decode(argv[2]);
  } else if (strcmp(argv[1], "create") == 0) {
    if (argc < 3) {
      printf("usage: bitmap create <FILE> <WIDTH> <HEIGHT>");
      return 0;
    }

    DWORD width = atoi(argv[3]);
    DWORD height = atoi(argv[4]);

    create(width, height, argv[2]);
  }

  return 0;
}
