//////////////////////////////////////////////////////////////////////////////
// by Jacob Marner. Feb 2002.
//
// A OpenGL BMP loader
//
//////////////////////////////////////////////////////////////////////////////
// Copyright 2002 Jacob Marner. jacob@marner.dk. Released under LGPL.

#ifndef BMP_LOADER_HEADER
#define BMP_LOADER_HEADER

// The following is the function return type. Use this to
// get information about how the loading operation went.

enum LOAD_TEXTUREBMP_RESULT {
  // The texture loading operation succeeded.
  LOAD_TEXTUREBMP_SUCCESS=0,
  // The file could not be found or it could not be opened for reading.
  LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE,
  // The file does not comply with the specification.
  LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT,
  // OpenGL could not accept the texture. You proably used a internal
  // format not accepted by your OpenGL implementation or you may have run 
  // out of available texture names.
  LOAD_TEXTUREBMP_OPENGL_ERROR,
  // The system ran out of heap memory during the texture load.
  LOAD_TEXTUREBMP_OUT_OF_MEMORY};

LOAD_TEXTUREBMP_RESULT loadBMPData(const char* fName,
 unsigned char** bitmapData, int& widthOut, int& heightOut, int& nChannelsOut);

#endif
