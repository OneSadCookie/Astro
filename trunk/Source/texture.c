#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

#define HEADER_LENGTH 8

GLuint texture_load_rgba(const char* name) {
  GLuint w, h;
  unsigned char header[HEADER_LENGTH];
  size_t nbytes;
  unsigned char* buffer;
  size_t image_size;
  GLuint tex;
  
  FILE* file = fopen(name, "rb");
  if (file == NULL) {
    fprintf(stderr, "can't open file\n");
    return 0;
  }
  
  nbytes = fread(header, 1, HEADER_LENGTH, file);
  if (nbytes != HEADER_LENGTH) {
    fprintf(stderr, "can't read header\n");
    fclose(file);
    return 0;
  }
  
  w = header[0] | (((GLuint)header[1]) << 8) |
      (((GLuint)header[2]) << 16) | (((GLuint)header[3]) << 24);
  h = header[4] | (((GLuint)header[5]) << 8) |
      (((GLuint)header[6]) << 16) | (((GLuint)header[7]) << 24);
      
  if (w != h) {
    fprintf(stderr, "texture not square\n");
    fclose(file);
    return 0;
  }
  
  if ((w & (w - 1)) != 0) {
    fprintf(stderr, "texture not a power of 2\n");
    fclose(file);
    return 0;
  }
  
  image_size = 4 * w * h;
  buffer = (unsigned char*)malloc(image_size);
  if (buffer == NULL) {
    fprintf(stderr, "can't allocate memory for texture\n");
    fclose(file);
    return 0;
  }
  
  nbytes = fread(buffer, 1, image_size, file);
  if (nbytes != image_size) {
    fprintf(stderr, "premature end of file\n");
    free(buffer);
    fclose(file);
    return 0;
  }
  
  glGenTextures(1, &tex);
  glBindTexture (GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA,
                     (GLsizei)w, (GLsizei)h,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     buffer);
  
  free(buffer);
  fclose(file);
  return tex;
}
