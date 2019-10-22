//////////////////////////////////////////////////////////////////////////////
// by Jacob Marner. Feb 2002.
//
// This file contains a BMP texture loader for OpenGL.
//
//////////////////////////////////////////////////////////////////////////////
// Copyright 2002 Jacob Marner. jacob@marner.dk. Released under LGPL.

#include <iostream>
using namespace std;

#include "BMPLoader.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>

// Note: Data in the .BMP format is stored in little-endian format.

// The file that the BMP is stored in.
static FILE* file;

// The offset from the BITMAPFILEHEADER structure
// to the actual bitmap data in the file.
static long byteOffset;

// Image width in pixels
static long width;

// Image height in pixels
static long height;

// Number channels
static int numChannels;

// Number of bit per pixel. Is 1, 4, 8, 24. If it is 1,4 or 8 then
// images is paletted, othewise not.
static int bitCount;

// Compression
enum CompressionType {BMP_BI_RGB=0, BMP_BI_RLE8, BMP_BI_RLE4 };
static CompressionType compression;

// Palette used for paletted images during load.
static unsigned char palette[3][256];

// The number of bytes read so far
static long bytesRead;

// Reads and returns a 32-bit value from the file.
static long read32BitValue()
{
  int c1 = fgetc(file);
  int c2 = fgetc(file);
  int c3 = fgetc(file);
  int c4 = fgetc(file);
  
  bytesRead+=4;
  
  return c1 + (c2<<8) + (c3<<16) + (c4<<24);
}

// Reads and returns a 16-bit value from the file
static short read16BitValue()
{
  int c1 = fgetc(file);
  int c2 = fgetc(file);
  
  bytesRead+=2;
  
  return c1 + (c2<<8);
}

// Reads and returns a 8-bit value from the file
static unsigned char read8BitValue()
{
  unsigned int c1 = fgetc(file);
  
  bytesRead+=1;
  
  return (unsigned char)c1;
}

// In Windows terms read this structure
// typedef struct tagBITMAPFILEHEADER {    /* bmfh */
//   UINT    bfType;
//   DWORD   bfSize;
//   UINT    bfReserved1;
//   UINT    bfReserved2;
//   DWORD   bfOffBits;
// } BITMAPFILEHEADER;
static LOAD_TEXTUREBMP_RESULT readFileHeader(void)
{
  // Read "BM" tag in ASCII.
  if (read8BitValue()!=66 || read8BitValue()!=77)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Read file size. We do not need this. Ignore.
  read32BitValue();
  
  // Skip the two reserved areas
  read16BitValue();
  read16BitValue();
  
  // Read the byte offset
  byteOffset = read32BitValue();
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// In windows terms read this struct:
//typedef struct tagBITMAPINFOHEADER {    /* bmih */
//  DWORD   biSize;
//  LONG    biWidth;
//  LONG    biHeight;
//  WORD    biPlanes;
//  WORD    biBitCount;
//  DWORD   biCompression;
//  DWORD   biSizeImage;
//  LONG    biXPelsPerMeter;
//  LONG    biYPelsPerMeter;
//  DWORD   biClrUsed;
//  DWORD   biClrImportant;
//} BITMAPINFOHEADER;
static LOAD_TEXTUREBMP_RESULT readInfoHeader(void)
{
  // We expect this to be at least 40. If it is larger we read
  // some more bytes in the end to be forward compatible.
  unsigned int sizeOfInfoHeader = (unsigned int)read32BitValue();
  
  if (sizeOfInfoHeader<40)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  width = read32BitValue();
  
  height = read32BitValue();
  
  // Read number of planes. According to the specification this
  // must be 1.
  if (read16BitValue()!=1)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  bitCount = read16BitValue();
  
  if (bitCount != 1 && bitCount != 4 && bitCount != 8 && bitCount != 24 && bitCount != 32)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  compression = (CompressionType)read32BitValue();

	numChannels = (bitCount == 32) ? 4 : 3;
  
  if (compression != BMP_BI_RGB && compression != BMP_BI_RLE8 && 
      compression != BMP_BI_RLE4)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Read image size. We do not need this since we have the 
  // image size.
  read32BitValue();
  
  // Pixel to device mapping. This is irrelevant since we simply
  // want the bitmap.
  read32BitValue();
  read32BitValue();
  
  // Read colors used. We do not need this, so it is ignored.
  read32BitValue();
  
  // Read the number of important colors. This will not be needed
  // in OpenGL so we will ignore this.
  read32BitValue();
  
  // Apply padding in end of header to be forward compatible.
  sizeOfInfoHeader -= 40;
  while (sizeOfInfoHeader>0)
  {
    read8BitValue();
    sizeOfInfoHeader--;
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// The palette follows directly after the
// info header
//
//typedef struct tagRGBQUAD {     /* rgbq */
//  BYTE    rgbBlue;
//  BYTE    rgbGreen;
//  BYTE    rgbRed;
//  BYTE    rgbReserved;
// } RGBQUAD;
static LOAD_TEXTUREBMP_RESULT readPalette(void)
{ 
  // 24-bit images are not paletted.
  if (bitCount==24)
    return LOAD_TEXTUREBMP_SUCCESS;
  
  int numColors = 1<<bitCount;
  for (int i=0;i<numColors;i++)
  {
    // Read RGB.
    for (int j=2;j>=0;j--)
      palette[j][i] = read8BitValue();
    
    // Skip reversed byte.
    read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

static LOAD_TEXTUREBMP_RESULT readBitmapData1Bit(unsigned char* bitmapData)
{
  // 1-bit format cannot be compressed
  if (compression!=BMP_BI_RGB)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;

  unsigned char byteRead;
  for (int y=0;y<height;y++)
  {
    int index = y*width*3;
    for (int x=0;x<width;x++)
    {
      if (x%8==0)
      {
        byteRead = read8BitValue();
      }
      
      unsigned char color = (byteRead >> (7-(x%8))) & 1;
      
      bitmapData[index+x*3] = palette[0][color];
      bitmapData[index+x*3+1] = palette[1][color];
      bitmapData[index+x*3+2] = palette[2][color]; 
    }

    // Pad to 32-bit boundery.
    while(bytesRead%4 != 0)
      read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// This is called after the first byte has been found to be 0
// and the second to be 0-2. This is only used in RLE-4 and RLE-8
// encoding.
static bool handleEscapeCode(int secondByte, int* x, int* y, 
                             LOAD_TEXTUREBMP_RESULT* res)
{
  if (secondByte==0x00)
  {
    // End of line
    (*x)=0;
    if ((*y)>=height)
    {
      *res = LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
      return true;
    }
    (*y)++;
  }
  else if (secondByte==0x01)
  {
    // End of bitmap
    *res = LOAD_TEXTUREBMP_SUCCESS;
    return true;            
  }
  else // secondByte=0x02
  {
    // Delta. Move drawing cursor.
    *x += read8BitValue();
    *y += read8BitValue();
    if (*x>=width || *x<0 || *y>=height || *y<0)
    {
      *res = LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
      return true;
    }
  }

  return false;
}

// Draw a 4-bit image. Can be uncompressed or RLE-4 compressed.
static LOAD_TEXTUREBMP_RESULT readBitmapData4Bit(unsigned char* bitmapData)
{
  if (compression!=BMP_BI_RGB && compression!=BMP_BI_RLE4)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Uncompressed 4 bit encoding
  if (compression==BMP_BI_RGB)
  {
    for (int j=0;j<height;j++)
    {
      int index = j*width*3;
      unsigned char byteValue;
      unsigned char color;
      for (int i=0;i<width;i++)
      {      
        if (i%2==0)
        {
          byteValue = read8BitValue();
          color = byteValue>>4;           
        }
        else
        {
          color = byteValue & 0x0F;
        }
        
        bitmapData[index+i*3] = palette[0][color];
        bitmapData[index+i*3+1] = palette[1][color];
        bitmapData[index+i*3+2] = palette[2][color];
      }
      
      // Pad to 32-bit boundery.
      for (int k=0; k<(((width+1)/2)%4);k++)
        read8BitValue();
    }
  }
  
  // RLE encoded 4-bit compression
  if (compression==BMP_BI_RLE4)
  {
    // Drawing cursor pos  
    int x=0;
    int y=0;
    
    // Clear bitmap data since it is legal not to 
    // fill it all out.
    memset(bitmapData,0,sizeof(unsigned char)*width*height*3);

    bytesRead=0;
    
    while(true)
    {
      unsigned char firstByte = read8BitValue();
      unsigned char secondByte = read8BitValue();
      
      // Is this an escape code or absolute encoding?
      if (firstByte==0)
      {
        // Is this an escape code
        if (secondByte<=0x02)
        {
          LOAD_TEXTUREBMP_RESULT res;
          if (handleEscapeCode(secondByte,&x,&y,&res))
            return res;
        }
        else
        {
          // Absolute encoding
          int index = y*width*3;
          int color;
          unsigned char databyte;
          for (int i=0; i<secondByte; i++)
          {
            if (x>=width)
              return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;

            if (i%2==0)
            {
              databyte = read8BitValue();
              color = databyte >> 4;
            }
            else
            {
              color = databyte & 0x0F;
            }

            bitmapData[index+x*3] = palette[0][color];
            bitmapData[index+x*3+1] = palette[1][color];
            bitmapData[index+x*3+2] = palette[2][color]; 
            x++;
          }
          
          // Pad to 16-bit word boundery
          while (bytesRead%2 != 0)
            read8BitValue();
        }
      }
      else
      {
        // If not absolute or escape code, perform data decode for next 
        // length of colors
        int color1 = secondByte >> 4;
        int color2 = secondByte & 0x0F;
        
        for (int i=0;i<firstByte;i++)
        {
          if (x>=width)
            return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
          
          int color;
          if (i%2==0)
            color = color1;
          else
            color = color2;
          
          int index = y*width*3+x*3;
          bitmapData[index] = palette[0][color];
          bitmapData[index+1] = palette[1][color];
          bitmapData[index+2] = palette[2][color];
          x++;
        }        
      }
    }
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// Read 8-bit image. Can be uncompressed or RLE-8 compressed.
static LOAD_TEXTUREBMP_RESULT readBitmapData8Bit(unsigned char* bitmapData)
{
  if (compression!=BMP_BI_RGB && compression!=BMP_BI_RLE8)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  if (compression==BMP_BI_RGB)
  {
    // For each scan line
    for (int i=0;i<height;i++)
    {
      int index = i*width*3;
      for (int j=0;j<width;j++)
      {
        int color = read8BitValue();       
        bitmapData[index+j*3] = palette[0][color];
        bitmapData[index+j*3+1] = palette[1][color];
        bitmapData[index+j*3+2] = palette[2][color];
      }                                             
      
      // go to next alignment of 4 bytes.
      for (int k=0; k<(width%4);k++)
        read8BitValue();
    }
  }
  
  if (compression==BMP_BI_RLE8)
  {
    // Drawing cursor pos
    int x=0;
    int y=0;

    bytesRead=0;
    
    // Clear bitmap data since it is legal not to 
    // fill it all out.
    memset(bitmapData,0,sizeof(unsigned char)*width*height*3);
    
    while(true)
    {
      unsigned char firstByte = read8BitValue();
      unsigned char secondByte = read8BitValue();
      
      // Is this an escape code or absolute encoding?
      if (firstByte==0)
      {
        // Is this an escape code
        if (secondByte<=0x02)
        {
          LOAD_TEXTUREBMP_RESULT res;
          if (handleEscapeCode(secondByte,&x,&y,&res))
            return res;
        }
        else
        {
          // Absolute encoding
          int index = y*width*3;
          for (int i=0; i<secondByte; i++)
          {
            if (x>=width)
              return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
            int color = read8BitValue(); 
            bitmapData[index+x*3] = palette[0][color];
            bitmapData[index+x*3+1] = palette[1][color];
            bitmapData[index+x*3+2] = palette[2][color]; 
            x++;
          }
          
          // Pad to 16-bit word boundery
          if (secondByte%2 == 1)
            read8BitValue();
        }
      }
      else
      {
        // If not, perform data decode for next length of colors
        for (int i=0;i<firstByte;i++)
        {
          if (x>=width)
            return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
          int index = y*width*3+x*3;
          bitmapData[index] = palette[0][secondByte];
          bitmapData[index+1] = palette[1][secondByte];
          bitmapData[index+2] = palette[2][secondByte];
          x++;
        }        
      }
    }
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// Load a 24-bit image. Cannot be encoded.
static LOAD_TEXTUREBMP_RESULT readBitmapData24Bit(unsigned char* bitmapData)
{
  // 24-bit bitmaps cannot be encoded. Verify this.
  if (compression!=BMP_BI_RGB)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  for (int i=0;i<height;i++)
  {
    int index = i*width*3;
    for (int j=0;j<width;j++)
    {
      bitmapData[index+j*3+2] = read8BitValue();
      bitmapData[index+j*3+1] = read8BitValue();
      bitmapData[index+j*3] = read8BitValue();
    }                                             

    // go to next alignment of 4 bytes.
//    for (int k=0; k<((width*3)%4);k++), the fix:
	for (int k=(width*3)%4; k!=0 && k<4;k++)
      read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// Load a 24-bit image. Cannot be encoded.
static LOAD_TEXTUREBMP_RESULT readBitmapData32Bit(unsigned char* bitmapData)
{
  for (int i=0;i<height;i++)
  {
    int index = i*width*4;
    for (int j=0;j<width;j++)
    {
      bitmapData[index+j*4+2] = read8BitValue();
      bitmapData[index+j*4+1] = read8BitValue();
      bitmapData[index+j*4] = read8BitValue();
      bitmapData[index+j*4+3] = read8BitValue();
    }
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

static LOAD_TEXTUREBMP_RESULT readBitmapData(unsigned char* bitmapData)
{
  // Pad until byteoffset. Most images will need no padding
  // since they already are made to use as little space as
  // possible.
  while(bytesRead<byteOffset)
    read8BitValue();
  
  // The data reading procedure depends on the bit depth.
  switch(bitCount)
  {
  case 1: 
    return readBitmapData1Bit(bitmapData);
  case 4:
    return readBitmapData4Bit(bitmapData);
  case 8:
    return readBitmapData8Bit(bitmapData);
  case 24:
    return readBitmapData24Bit(bitmapData);
  default: // 32-bit
    return readBitmapData32Bit(bitmapData);
  }
}

// Load the BMP. Assumes that no extension is appended to the filename.
// If successful *bitmapData will contain a pointer to the data.
LOAD_TEXTUREBMP_RESULT loadBMP(const char* filename, 
                               unsigned char** bitmapData)
{
  *bitmapData = nullptr; 

  // Append .bmp extension
  char* str = new char[strlen(filename)+5];
  if (!str)
    return LOAD_TEXTUREBMP_OUT_OF_MEMORY;

  strcpy(str,filename);
  strcat(str,".bmp");
  int widthOut, heightOut, nChannelsOut;
  LOAD_TEXTUREBMP_RESULT res = loadBMPData(str,bitmapData,
		widthOut, heightOut, nChannelsOut);
 
  // Clean up
  delete[] str;

  return res;
}

LOAD_TEXTUREBMP_RESULT loadBMPData(const char* fName,
	unsigned char** bitmapData,
	int& widthOut, int& heightOut, int& nChannelsOut)
{
  // Open file for buffered read.
  file = fopen(fName,"rb");

  char readBuffer[BUFSIZ];

  LOAD_TEXTUREBMP_RESULT res = LOAD_TEXTUREBMP_SUCCESS;

  if (!file)
    res = LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE;
  
  if (!res)
  {
    setbuf(file, readBuffer);
  
    bytesRead=0;
  }

  // Read File Header
  if (!res)
    res=readFileHeader();
  
  // Read Info Header
  if (!res)
    res=readInfoHeader();
  
  // Read Palette
  if (!res)
    res=readPalette();

  if (!res)
  {
    // The bitmap data we are going to hand to OpenGL
    *bitmapData = new unsigned char[width*height*numChannels];

    if (!(*bitmapData))
      res = LOAD_TEXTUREBMP_OUT_OF_MEMORY;
  }

  if (!res)
  {
    // Read Data
    res=readBitmapData(*bitmapData);
  }

  // Only clean up bitmapData if there was an error.
  if (*bitmapData && res)
    delete[] *bitmapData;
  else
  {
	widthOut = width;
	heightOut = height;
	nChannelsOut = numChannels;
  }

  if (file)
    fclose(file);

  return res;
}

// Removes the .bmp extension of the filename if it exists.
static void removeBMPextension(const char* withext, char* result)
{
  for(int i=strlen(withext)-1;i>=0;i--)
  {
    if (!strcmp(withext+i,".bmp") || !strcmp(withext+i,".BMP"))
    {
      for (int j=0;j<i;j++)
      {
        result[j] = withext[j];
      }
      result[i]='\0';
      return;
    }
  }
}
