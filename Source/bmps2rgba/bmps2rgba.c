#include <machine/endian.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define quit(msg) { fprintf(stderr, (msg)); exit(EXIT_FAILURE); }
#define quit_if(b, msg) if (b) quit(msg)

#if BYTE_ORDER == BIG_ENDIAN

uint16_t swap2(uint16_t i) {
  const uint8_t* p = (uint8_t*)&i;
  return ((uint16_t)(p[0])) | ((uint16_t)(p[1]) << 8);
}

uint32_t swap4(uint32_t i) {
  const uint8_t* p = (uint8_t*)&i;
  return ((uint32_t)(p[0])) | ((uint32_t)(p[1]) << 8)
                            | ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

#else

uint16_t swap2(uint16_t i) { return i; }
uint32_t swap4(uint32_t i) { return i; }

#endif

typedef struct BMPHeader {
  uint16_t padding;
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offBits;
} BMPHeader;

typedef struct BMPInfo {
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t sizeImage;
	uint32_t xPelsPerMeter;
	uint32_t yPelsPerMeter;
	uint32_t clrUsed;
	uint32_t clrImportant;
} BMPInfo;

typedef struct RGBAHeader {
  uint32_t width;
  uint32_t height;
} RGBAHeader;

int main(int argc, char** argv) {
  FILE* rgbfile; FILE* alphafile;
  BMPHeader rgbheader, alphaheader;
  size_t nbytes;
  uint32_t rgboffset, alphaoffset;
  BMPInfo rgbinfo, alphainfo;
  
  int seekerr;
  
  FILE* rgbafile;
  RGBAHeader rgbaheader;
  uint8_t r, g, b, a;
  uint32_t count;
  
  quit_if(offsetof(BMPHeader, offBits) != 12, "structure is screwed\n");

  quit_if(argc != 4, "usage: bmps2rgba /path/to/rgb.bmp /path/to/alpha.bmp /path/to/dest.rgba\n\n");
  
  //--- Open the files ---//
  rgbfile = fopen(argv[1], "rb");
  quit_if(rgbfile == NULL, "couldn't open rgb file for reading\n");
  
  alphafile = fopen(argv[2], "rb");
  quit_if(alphafile == NULL, "couldn't open alpha file for reading\n");
  
  //--- Process the headers ---//
  nbytes = fread((uint8_t*)&rgbheader + 2, 1, 14, rgbfile);
  quit_if(nbytes != 14, "not enough data in rgb file for header\n");
  
  nbytes = fread((uint8_t*)&alphaheader + 2, 1, 14, alphafile);
  quit_if(nbytes != 14, "not enough data in alpha file for header\n");
  
  rgbheader.type = swap2(rgbheader.type);
  quit_if(rgbheader.type != 19778, "rgb file is not a windows BMP file\n");
  
  alphaheader.type = swap2(alphaheader.type);
  quit_if(alphaheader.type != 19778, "alpha file is not a windows BMP file\n");
  
  rgboffset = swap4(rgbheader.offBits);
  alphaoffset = swap4(alphaheader.offBits);
  
  //--- Process the info headers ---//
  nbytes = fread(&rgbinfo, 1, 40, rgbfile);
  quit_if(nbytes != 40, "not enough data in rgb file for info\n");
  
  nbytes = fread(&alphainfo, 1, 40, alphafile);
  quit_if(nbytes != 40, "not enough data in alpha file for info\n");
  
  rgbinfo.width = swap4(rgbinfo.width);
  rgbinfo.height = swap4(rgbinfo.height);
  quit_if(rgbinfo.width != rgbinfo.height, "rgb file not square\n");
  quit_if((rgbinfo.width & (rgbinfo.width - 1)) != 0, "rgb file size not a power of 2\n");
  
  alphainfo.width = swap4(alphainfo.width);
  alphainfo.height = swap4(alphainfo.height);
  quit_if(alphainfo.width != rgbinfo.width, "alpha file not the same width as rgb file\n");
  quit_if(alphainfo.height != rgbinfo.height, "alpha file not the same height as rgb file\n");
  
  rgbinfo.bitCount = swap2(rgbinfo.bitCount);
  quit_if(rgbinfo.bitCount != 24, "rgb file not 24bpp\n");
  
  alphainfo.bitCount = swap2(alphainfo.bitCount);
  quit_if(alphainfo.bitCount != 8, "alpha file not 8bpp\n");
  
  quit_if(rgbinfo.compression != 0, "rgb file is compressed\n");
  quit_if(alphainfo.compression != 0, "alpha file is compressed\n");
  
  //--- Create the .rgba file ---//
  rgbafile = fopen(argv[3], "wb");
  quit_if(rgbafile == NULL, "couldn't open rgba file for writing\n");
  
  //--- write the .rgba file header ---//
  rgbaheader.width = swap4(rgbinfo.width);
  rgbaheader.height = rgbaheader.width;
  
  nbytes = fwrite(&rgbaheader, 1, sizeof(rgbaheader), rgbafile);
  quit_if(nbytes != sizeof(rgbaheader), "couldn't write rgba header\n");
  
  //--- find the bitmap data in the source files ---//
  seekerr = fseek(rgbfile, rgboffset, SEEK_SET);
  quit_if(seekerr == -1, "error seeking to rgb bitmap data\n");
  
  seekerr = fseek(alphafile, alphaoffset, SEEK_SET);
  quit_if(seekerr == -1, "error seeking to alpha bitmap data\n");
  
  //--- process the files ---//
  for (count = 0; count < rgbinfo.width * rgbinfo.height; ++count) {
  
    nbytes = fread(&b, 1, sizeof(b), rgbfile);
    quit_if(nbytes != sizeof(b), "couldn't read enough picture data (r) from rgb file\n");
    nbytes = fread(&g, 1, sizeof(g), rgbfile);
    quit_if(nbytes != sizeof(g), "couldn't read enough picture data (g) from rgb file\n");
    nbytes = fread(&r, 1, sizeof(r), rgbfile);
    quit_if(nbytes != sizeof(r), "couldn't read enough picture data (b) from rgb file\n");
    
    nbytes = fread(&a, 1, sizeof(a), alphafile);
    quit_if(nbytes != sizeof(a), "couldn't read enough picture data from alpha file\n");
    
    nbytes = fwrite(&r, 1, sizeof(r), rgbafile);
    quit_if(nbytes != sizeof(r), "couldn't write picture data (r) to rgba file\n");
    nbytes = fwrite(&g, 1, sizeof(g), rgbafile);
    quit_if(nbytes != sizeof(g), "couldn't write picture data (g) to rgba file\n");
    nbytes = fwrite(&b, 1, sizeof(b), rgbafile);
    quit_if(nbytes != sizeof(b), "couldn't write picture data (b) to rgba file\n");
    nbytes = fwrite(&a, 1, sizeof(a), rgbafile);
    quit_if(nbytes != sizeof(a), "couldn't write picture data (a) to rgba file\n");
  
  }
  
  //--- clean up ---//
  fclose(rgbfile);
  fclose(alphafile);
  
  fclose(rgbafile);
  
  return 0;
}
