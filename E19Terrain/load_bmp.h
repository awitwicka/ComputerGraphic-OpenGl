/*
*  Example for using the code in this file:
*
*    int width, height;
*    GLubyte* image = (GLubyte*) loadBMP("filename", &width, &height);
*    GLuint texture;
*    if(image != NULL) {
*      glBindTexture(GL_TEXTURE_2D, texture);
*      
*      // Call glTexParameteri, for example, in order to
*      // change any properties of the the texture, such as mipmapping...
*
*      glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
*      free(image);
*    }
*    else {
*      printf("Cannot load texture %s!\n", textureFiles[i]);
*    }
*
*  Memory is allocated automatically, so do not forget to call "free". 
*/

#ifndef __LOADBMP_H__
#define __LOADBMP_H__

#include <stdlib.h>

typedef struct {
  unsigned short  FileType;
  unsigned int    FileSize;
  unsigned short  Reserved1;
  unsigned short  Reserved2;
  unsigned int    OffBits;
  unsigned int    Size;
  unsigned int    Width;
  unsigned int    Height;
  unsigned short  Planes;
  unsigned short  BitCount;
  unsigned int    Compression;
  unsigned int    SizeImage;
  unsigned int    XPelsPerMeter;
  unsigned int    YPelsPerMeter;
  unsigned int    ClrUsed;
  unsigned int    ClrImportant;
} BMPHeader;

static FILE *curr_image_fd;
static int isSwapped;

static void swab_short(unsigned short *x) {
  if(isSwapped)
    *x = ((*x >>  8) & 0x00FF) | 
         ((*x <<  8) & 0xFF00);
}

static void swab_int(unsigned int *x) {
  if(isSwapped)
    *x = ((*x >> 24) & 0x000000FF) | 
         ((*x >>  8) & 0x0000FF00) | 
         ((*x <<  8) & 0x00FF0000) | 
         ((*x << 24) & 0xFF000000);
}

static void swab_int_array(int *x, int leng) {
  if(!isSwapped)
    return;
  
  int i;
  
  for(i = 0 ; i < leng ; i++) {
    *x = ((*x >> 24) & 0x000000FF) | 
         ((*x >>  8) & 0x0000FF00) | 
         ((*x <<  8) & 0x00FF0000) | 
         ((*x << 24) & 0xFF000000);
    x++;
  }
}

static unsigned char readByte() {
  unsigned char x;
  
  fread(&x, sizeof(unsigned char), 1, curr_image_fd);
  
  return x;
}

static unsigned short readShort() {
  unsigned short x;
  
  fread(&x, sizeof(unsigned short), 1, curr_image_fd);
  swab_short(&x);
  
  return x;
}

static unsigned int readInt() {
  unsigned int x;
  
  fread(&x, sizeof(unsigned int), 1, curr_image_fd);
  swab_int(&x);
  
  return x;
}

static GLubyte* loadBMP(char *fname, int* image_width, int* image_height) {
  /* Open file & get size */
  if((curr_image_fd = fopen(fname, "rb")) == NULL) {
    printf("Cannot open file\n");
    return 0;
  }
  /*
    Load the BMP piecemeal to avoid struct packing issues
  */
  isSwapped = 0;

  BMPHeader bmphdr;
  bmphdr.FileType = readShort();

  if(bmphdr.FileType == (((int) 'B') + (((int) 'M') << 8)))
    isSwapped = 0;
  else if(bmphdr.FileType == (((int) 'M') + (((int) 'B') << 8)))
    isSwapped = 1;
  else {
    printf("Unexpected file type\n");
    fclose(curr_image_fd);
    return 0;
  }

  bmphdr.FileSize      = readInt();
  bmphdr.Reserved1     = readShort();
  bmphdr.Reserved2     = readShort();
  bmphdr.OffBits       = readInt();
  bmphdr.Size          = readInt();
  bmphdr.Width         = readInt();
  bmphdr.Height        = readInt();
  bmphdr.Planes        = readShort();
  bmphdr.BitCount      = readShort();
  bmphdr.Compression   = readInt();
  bmphdr.SizeImage     = readInt();
  bmphdr.XPelsPerMeter = readInt();
  bmphdr.YPelsPerMeter = readInt();
  bmphdr.ClrUsed       = readInt();
  bmphdr.ClrImportant  = readInt();
  
  GLubyte *image;
  int w, h, bpp;
  
  w   = bmphdr.Width;
  h   = bmphdr.Height;
  bpp = bmphdr.Planes * bmphdr.BitCount;
  bmphdr.SizeImage = w * h * (bpp / 8);

  if(bpp != 24) {
    printf("bpp != 24\n");
    return 0;
  }
  
  image = (GLubyte *) malloc(w * h * 3);
  
  if(image_width)
    *image_width = w;
  if(image_height)
    *image_height = h;


  fseek(curr_image_fd, bmphdr.OffBits, SEEK_SET);
  
  if(fread(image, 1, bmphdr.SizeImage, curr_image_fd) != bmphdr.SizeImage) {
    printf("Cannot read image\n");
    fclose(curr_image_fd);
    return 0;
  }

  fclose(curr_image_fd);

  /* BGR --> RGB */
  int i;
  GLubyte tmp;
  
  for(i = 0 ; i < w * h ; i++) {
    tmp = image [ 3 * i ] ;
    image[3 * i + 0] = image[3 * i + 2];
    image[3 * i + 2] = tmp;
  }

  return image;
}

#endif
