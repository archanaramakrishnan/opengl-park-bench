/*
 *  CGLString.cpp
 *  FontTest
 *
 *  Created by George Sealy on 15/04/09.
 *
 *  Modified by J. R. Miller 16Dec2011:
 *  1. OpenGL 3.3/4.x compatibility including GLSL accommodations
 *  2. Allow users to query 2D rectangle containing the generated text
 *     once the instance has been created. (Among other things, requires
 *     that BuildString be called from the constructor.)
 *  3. Save restore state related to blending.
 *  4. Make memory management more error-tolerant.
 *  5. Other miscellaneous changes.
 *
 *  More modifications by J. R. Miller 28Nov2012 to better support both 2D and
 *  3D rendering.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "CGLString.h"
#include "CFont.h"

static const double BasicDistanceTol = 1.0e-4;

CGLString::CGLString(const char *text, const CFont *font, int dim) :
	mText(nullptr), mFont(font), mVertices(nullptr), mVerticesForRender(nullptr),
	mUVs(nullptr), numCoordsInVerticesForRender(0), num2DCoords(0),
	mIndices(nullptr), numIndices(0), mNumberOfQuads(0),
	origin(0.0, 0.0, 0.0), uDir(1.0, 0.0, 0.0), vDir(0.0, 1.0, 0.0),
	renderingParametersModified(true), vertexCoordsDimension(dim),
	vao(0), vboVertexCoords(0), vboTexCoords(0), ebo(0)
{
	finishConstruction(text);
}

CGLString::CGLString(const std::string& text, const CFont *font, int dim) :
	mText(nullptr), mFont(font), mVertices(nullptr), mVerticesForRender(nullptr),
	mUVs(nullptr), numCoordsInVerticesForRender(0), num2DCoords(0),
	mIndices(nullptr), numIndices(0), mNumberOfQuads(0),
	origin(0.0, 0.0, 0.0), uDir(1.0, 0.0, 0.0), vDir(0.0, 1.0, 0.0),
	renderingParametersModified(true), vertexCoordsDimension(dim),
	vao(0), vboVertexCoords(0), vboTexCoords(0), ebo(0)
{
	finishConstruction(text.c_str());
}

CGLString::~CGLString()
{
	if (mText != nullptr)
		free(mText);
	if (mVertices != nullptr)
		delete [] mVertices;
	if (mVerticesForRender != nullptr)
		delete [] mVerticesForRender;
	if (mUVs != nullptr)
		delete [] mUVs;
	if (mIndices != nullptr)
		delete [] mIndices;
}

void CGLString::buildString()
{
	numCoordsInVerticesForRender = vertexCoordsDimension * 4 * mNumberOfQuads;
	num2DCoords = 2 * 4 * mNumberOfQuads;
	mVertices = new GLfloat[num2DCoords];
	mVerticesForRender = new GLfloat[numCoordsInVerticesForRender]; // filled later
	mUVs = new GLfloat[num2DCoords];
	
	// Using bytes for indices limits us to 256 / 4 = 64 characters
	// If more are needed, we could use unsigned shorts instead
	numIndices = 6 * mNumberOfQuads;
	mIndices = new GLbyte[numIndices];
	
	float baseX = 0.0f;
	float baseY = 0.0f;
	float x = 0.0f;
	float y;
	
	// Set up vertex data...
	int length = strlen(mText);
	int vertIndex = 0, texIndex = 0;
	for (int i = 0; i < length; ++i)
	{
		if (mText[i] == ' ')
		{
			// Simple hack to generate spaces, could be handled better though...
			baseX += mFont->mCharacterData['i'].screenWidth;
		}
		else
		{
			// Build a quad (two triangles) for the character
			CCharacterData &data = mFont->mCharacterData[mText[i]];

			x = baseX + data.xOffset;
			y = baseY - data.yOffset;

			mVertices[vertIndex++] = x;
			mVertices[vertIndex++] = y - data.byteHeight;
			mVertices[vertIndex++] = x;
			mVertices[vertIndex++] = y;
			
			x += data.byteWidth;
			
			mVertices[vertIndex++] = x;
			mVertices[vertIndex++] = y - data.byteHeight;
			mVertices[vertIndex++] = x;
			mVertices[vertIndex++] = y;
			
			memcpy(&mUVs[texIndex], data.texCoords, sizeof(float) * 8);
			
			texIndex += 8;
            
            baseX += data.screenWidth;
		}
	}
	
	// And now set up an array of index data
	int indIndex = 0;
	for (int i = 0; i < mNumberOfQuads; ++i)
	{
		mIndices[indIndex + 0] = (GLbyte)(4 * i + 0);
		mIndices[indIndex + 1] = (GLbyte)(4 * i + 1);
		mIndices[indIndex + 2] = (GLbyte)(4 * i + 2);
		
		mIndices[indIndex + 3] = (GLbyte)(4 * i + 2);
		mIndices[indIndex + 4] = (GLbyte)(4 * i + 1);
		mIndices[indIndex + 5] = (GLbyte)(4 * i + 3);
		
		indIndex += 6;
	}
}

void CGLString::copyFontSizeTo(CGLString* other) const
{
	if ((this == other) || (other == nullptr))
		return;
	float xmin, xmax, ymin, ymax;
	other->getBoundingBox(xmin, xmax, ymin, ymax);
	double width = aUDir * (xmax - xmin);
	double height = aVDir * (ymax - ymin);
	other->setStringDimensions(width, height);
}

void CGLString::finishConstruction(const char* text)
{
	if ((vertexCoordsDimension < 2) || (vertexCoordsDimension > 3))
		vertexCoordsDimension = 2;
	mText = strdup(text);
	
	// We figure out the number of texured quads we'll be rendering later
	// (the number of characters minus any spaces)
	int length = strlen(mText);
	for (int i = 0; i < length; ++i)
	{
		if (mText[i] != ' ')
		{
			++mNumberOfQuads;
		}
	}
	buildString();
	setStringDimensions(0.0, 0.0); // make sure scale/trans info sensible
}

void CGLString::getBoundingBox(GLfloat& xmin, GLfloat& xmax, GLfloat& ymin, GLfloat& ymax) const
{
	xmin = mVertices[0];
	ymin = mVertices[1];
	xmax = mVertices[num2DCoords - 2];
	ymax = mVertices[num2DCoords - 1];
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void CGLString::linearMap(double fromMin, double fromMax, double toMin, double toMax,
						  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}
void CGLString::makeVerticesForRendering()
{
	if (uDir.normalize() < BasicDistanceTol)
	{
		uDir = cryph::AffVector::xu;
		vDir = cryph::AffVector::yu;
	}
	else
	{
		cryph::AffVector par, perp;
		uDir.decompose(vDir, par, perp);
		if (perp.normalizeToCopy(vDir) < BasicDistanceTol)
		{
			if (vertexCoordsDimension == 2)
				vDir = cryph::AffVector::zu.cross(uDir);
			else
			{
				uDir.arbitraryNormal(vDir);
				vDir.normalize();
			}
		}
	}
	int loc = 0;
	for (int i=0 ; i<num2DCoords ; i+=2)
	{
		double uc = aUDir*mVertices[i] + bUDir;
		double vc = aVDir*mVertices[i+1] + bVDir;
		cryph::AffPoint p = origin + uc*uDir + vc*vDir;
		mVerticesForRender[loc++] = p[0];
		mVerticesForRender[loc++] = p[1];
		if (vertexCoordsDimension == 3)
			mVerticesForRender[loc++] = p[2];
	}
	renderingParametersModified = false;
}

void CGLString::renderString(int pvaLoc_mcPos, int pvaLoc_texCoords, int ppuLoc_texMap)
{
	if (renderingParametersModified)
		sendBufferDataToGPU(pvaLoc_mcPos, pvaLoc_texCoords);
	
	// save current state and then set up required blending
	int saveBlend, saveSrcFactor, saveDestFactor;
	glGetIntegerv(GL_BLEND, &saveBlend);
	glGetIntegerv(GL_BLEND_SRC, & saveSrcFactor);
	glGetIntegerv(GL_BLEND_DST, & saveDestFactor);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Bind the font's texture
	glBindTexture(GL_TEXTURE_2D, mFont->mTexId);
	
	// Bind our vertex data and draw the text
	glBindVertexArray(vao);
	glUniform1i(ppuLoc_texMap, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, 0);

	// Restore blending state
	if (saveBlend == GL_FALSE)
		glDisable(GL_BLEND);
	glBlendFunc(saveSrcFactor, saveDestFactor);
}

void CGLString::sendBufferDataToGPU(int pvaLoc_mcPos, int pvaLoc_texCoords)
{
	makeVerticesForRendering();

	if (vao == 0) // First call; allocate and fill all structures
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vboVertexCoords);
		glGenBuffers(1, &vboTexCoords);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vboVertexCoords);
		glBufferData(GL_ARRAY_BUFFER, numCoordsInVerticesForRender*sizeof(GLfloat),
			mVerticesForRender, GL_STATIC_DRAW);
		glEnableVertexAttribArray(pvaLoc_mcPos);
		glVertexAttribPointer(pvaLoc_mcPos, vertexCoordsDimension, GL_FLOAT, GL_FALSE, 0, 0);
	
		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
		glBufferData(GL_ARRAY_BUFFER, num2DCoords*sizeof(GLfloat), mUVs,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(pvaLoc_texCoords);
		glVertexAttribPointer(pvaLoc_texCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLbyte), mIndices, GL_STATIC_DRAW);
	}
	else // just updating the vertex coordinates
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboVertexCoords);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numCoordsInVerticesForRender*sizeof(GLfloat),
			mVerticesForRender);
	}
}

void CGLString::setStringOrigin(double x, double y, double z)
{
	if (vertexCoordsDimension == 2)
		z = 0.0;
	origin = cryph::AffPoint(x, y, z);
	renderingParametersModified = true;
}

void CGLString::setStringDirection(double dx, double dy, double dz)
{
	if (vertexCoordsDimension == 2)
		dz = 0.0;
	uDir = cryph::AffVector(dx, dy, dz);
	renderingParametersModified = true;
}

void CGLString::setStringUp(double dx, double dy, double dz)
{
	if (vertexCoordsDimension == 2)
		dz = 0.0;
	vDir = cryph::AffVector(dx, dy, dz);
	renderingParametersModified = true;
}

void CGLString::setStringDimensions(double width, double height)
{
	float xmin, xmax, ymin, ymax;
	getBoundingBox(xmin, xmax, ymin, ymax);
	if ((width == 0.0) && (height == 0.0))
		width = xmax - xmin;
	double labelAspectRatio = (ymax - ymin) / (xmax - xmin);
	if (width == 0)
		width = height / labelAspectRatio;
	else if (height == 0)
		height = labelAspectRatio * width;
	currentRenderWidth = width;
	currentRenderHeight = height;
	linearMap(xmin, xmax, 0.0, currentRenderWidth, aUDir, bUDir);
	linearMap(ymin, ymax, 0.0, currentRenderHeight, aVDir, bVDir);
	renderingParametersModified = true;
}

