# 🏝️ bitmap

Some bitmap image manipulation.

## API

```c
BITMAP BITMAP_init(DWORD width, DWORD height);
BITMAP BITMAP_open(const char *path);

void BITMAP_set_pixel(BITMAP bitmap, DWORD x, DWORD y, BITMAPCOLOR color);
BITMAPCOLOR BITMAP_get_pixel(BITMAP bitmap, DWORD x, DWORD y);

void BITMAP_save(BITMAP bitmap,  const char *path);
void BITMAP_free(BITMAP bitmap);
```

## Usage

```
bitmap <COMMAND> [ARGUMENTS]

Available commands:
    decode <FILE>                  - Decode the BITMAP image and view the data.
    create <FILE> <WIDTH> <HEIGHT> - Create a monochromatic grayscale BITMAP image.

Examples:
    bitmap decode sample.bmp
    bitmap create created_image.bmp 200 200
```

## References

- [BMP File Format](https://en.wikipedia.org/wiki/BMP_file_format)
