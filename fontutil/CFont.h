/*
 *  CFont.h
 *  CFontTest
 *
 *  Created by George Sealy on 9/04/09.
 *
 *  Modified by J. R. Miller 16Dec2011:
 *  1. OpenGL 3.3/4.x compatibility
 *  2. Make file reading and memory management more error-tolerant.
 *  3. Other miscellaneous changes.
 *
 */

#ifndef CFONT_H
#define CFONT_H

#include <string>
#include <vector>

#ifdef __APPLE__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

// A store for each character's data
class CCharacterData
{
public:
		
	CCharacterData();
	~CCharacterData();
		
	// Given an open file, load a character's worth of data
	void Load(FILE *fontFile);
	
	// Refer to the README file that comes with Paul Nettle's Font generator
	// for details of these values.
	int byteWidth;
	int byteHeight;
	int xOffset;
	int yOffset;
	int screenWidth;
	int screenHeight;
	unsigned char *buffer;

	// Mark whether we want this character or not
	bool isWanted;
	float texCoords[8];
private:
	void readFailed(FILE* fontFile, int loc);
	static bool doneReading;
};

class CFont 
{
public:
	~CFont();
	
	GLuint mTexId;
	CCharacterData *mCharacterData;

	static CFont* getFont(const std::string &fontFilename);
private:
	CFont(FILE* fontFile);
	CFont(const CFont& cf) {}
	// Determine what size of texture we need for a given width of texture
	int getHeightForTexture(int textureWidth, int charWidth, int charHeight);
	static void show(int charCode);
};

#endif
