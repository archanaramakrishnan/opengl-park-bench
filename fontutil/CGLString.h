/*
 *  CGLString.h
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

#ifndef CGLSTRING_H
#define CGLSTRING_H

#ifdef __APPLE__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"

// Forward declaration
class CFont;

class CGLString
{
public:
	CGLString(const char *text, const CFont *font, int dim=2);
	CGLString(const std::string& text, const CFont *font, int dim=2);
	~CGLString();

	void copyFontSizeTo(CGLString* other) const;

	double getCurrentRenderHeight() const { return currentRenderHeight; }
	double getCurrentRenderWidth() const { return currentRenderWidth; }
	cryph::AffPoint getOrigin() const { return origin; }
	cryph::AffVector getUDir() const { return uDir; }
	cryph::AffVector getVDir() const { return vDir; }

	void renderString(int pvaLoc_mcPos, int pvaLoc_texCoords, int ppuLoc_texMap);

	// All z coordinates/components are assumed to be 0 and ignored when dim==2
	void setStringOrigin(double x, double y, double z=0.0);
	void setStringDirection(double dx, double dy, double dz=0.0);
	void setStringUp(double dx, double dy, double dz=0.0);
	void setStringDimensions(double width, double height);
	
private:
	char *mText;
	const CFont *mFont;
	GLfloat *mVertices, *mVerticesForRender;
	GLfloat *mUVs;
	int numCoordsInVerticesForRender, num2DCoords;
	GLbyte *mIndices;
	int numIndices;
	int mNumberOfQuads;

	// For rendering
	cryph::AffPoint origin;
	cryph::AffVector uDir, vDir;
	double aUDir, bUDir, aVDir, bVDir; // to scale string to requested width/height
	bool renderingParametersModified;
	double currentRenderWidth, currentRenderHeight;
	int vertexCoordsDimension;

	void buildString();
	void finishConstruction(const char* text);
	void getBoundingBox(GLfloat& xmin, GLfloat& xmax, GLfloat& ymin, GLfloat& ymax) const;
	void makeVerticesForRendering();
	void sendBufferDataToGPU(int pvaLoc_mcPos, int pvaLoc_texCoords);

	static void linearMap(double fromMin, double fromMax, double toMin, double toMax,
						  double& scale, double& trans);

	GLuint vao, vboVertexCoords, vboTexCoords, ebo;
};

#endif
