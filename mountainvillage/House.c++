// House.c++ - concrete subclass of ModelView that represents a House

#include <iostream>
#include <cmath>

#include "House.h"

#include "CFont.h"
#include "CGLString.h"

typedef float vec2[2];

CFont* House::houseLabelFont = nullptr;

House::House(ShaderIF* sIF, double xbIn, double ybIn, double widthIn, double heightIn,
	double roofWidthIn, double roofHeightIn, const std::string& label) :
	shaderIF(sIF),
	xb(xbIn), yb(ybIn), width(widthIn), height(heightIn),
	roofWidth(roofWidthIn), roofHeight(roofHeightIn), houseLabel(nullptr)
{
	validateData();
	defineModel();
	// finally, create the house label
	buildHouseLabel(label);
}

House::~House()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
	if (houseLabel != nullptr)
		delete houseLabel;
}

void House::buildHouseLabel(const std::string& label)
{
	if (houseLabelFont == nullptr)
		return;
	houseLabel = new CGLString(label.c_str(), houseLabelFont, 2);
	houseLabel->setStringDimensions(0.95*width, 0.0);
	double mcLabelWidth = houseLabel->getCurrentRenderWidth();
	double mcLabelHeight = houseLabel->getCurrentRenderHeight();
	double mcXStart = xb - 0.5*mcLabelWidth;
	double mcYStart = yb + height - 1.4*mcLabelHeight;
	houseLabel->setStringOrigin(mcXStart, mcYStart);
}

void House::defineModel()
{
	// We need 11 points: 3 for the roof; 4 for the front wall; 4 for the front door
	vec2 points[11];
	// create the geometry of the roof
	points[0][0] = xb - roofWidth/2.0;
	points[0][1] = yb + height;
	points[1][0] = xb + roofWidth/2.0;
	points[1][1] = points[0][1];
	points[2][0] = xb;
	points[2][1] = yb + height + roofHeight;
	// create the geometry of the front wall
	points[3][0] = xb - width/2.0;
	points[3][1] = yb;
	points[4][0] = xb + width/2.0;
	points[4][1] = yb;
	points[5][0] = points[3][0];
	points[5][1] = yb + height;
	points[6][0] = points[4][0];
	points[6][1] = points[5][1];
	// create the geometry of the front door
	double dw = width / 6.0;
	double dh = height / 4.0;
	points[7][0] = xb - dw/2.0;
	points[7][1] = yb;
	points[8][0] = xb + dw/2.0;
	points[8][1] = yb;
	points[9][0] = points[7][0];
	points[9][1] = yb + dh;
	points[10][0] = points[8][0];
	points[10][1] = points[9][1];

	// send data to GPU:
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 11*sizeof(vec2), points, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void House::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xb - roofWidth/2.0;
	xyzLimits[1] = xb + roofWidth/2.0;
	xyzLimits[2] = yb;
	xyzLimits[3] = yb + height + roofHeight;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void House::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderIF->getShaderPgmID());

	// define the mapping from MC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(shaderIF->ppuLoc("scaleTrans"), 1, scaleTrans);

	renderHouse();

	// restore the previous program
	glUseProgram(pgm);
}

void House::renderHouse() const
{
	glBindVertexArray(vao[0]);
	// draw roof
	glUniform1i(shaderIF->ppuLoc("housePart"), 0); // '0' means roof
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3); // offset: 0
	// front wall
	glUniform1i(shaderIF->ppuLoc("housePart"), 1); // '1' means walls of house
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
	// door
	glUniform1i(shaderIF->ppuLoc("housePart"), 2); // '2' means door
	glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);
	// house label
	renderHouseLabel();
}

void House::renderHouseLabel() const
{
	if (houseLabel == nullptr)
		return;
	float fColor[] = {0.0, 0.0, 0.0, 1.0};
	glUniform4fv(shaderIF->ppuLoc("fontColor"), 1, fColor);
	glUniform1i(shaderIF->ppuLoc("renderingFontString"), 1);
	houseLabel->renderString(shaderIF->pvaLoc("mcPosition"), shaderIF->pvaLoc("texCoords"), shaderIF->ppuLoc("fontTextureMap"));
	glUniform1i(shaderIF->ppuLoc("renderingFontString"), 0);
}

void House::setHouseLabelFont(std::string font)
{
	houseLabelFont = CFont::getFont(font);
}

void House::validateData()
{
	if (width <= 0.0)
		width = 1.0;
	if (height <= 0.0)
		height = 1.0;
	if (roofWidth < width)
		roofWidth = width;
	if (roofHeight <= 0.0)
		roofHeight = 1.0;
}
