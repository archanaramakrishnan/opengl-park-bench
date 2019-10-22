// Tree.c++ - concrete subclass of ModelView that represents a Tree

#include <iostream>
#include <cmath>

#include "Tree.h"

typedef float vec2[2];

Tree::Tree(ShaderIF* sIF,
	double xbIn, double ybIn, double rTreeTopIn, double rTrunkIn, double heightIn,
	int numTreeTopPointsIn, int numTrunkPointsIn) : shaderIF(sIF),
	xb(xbIn), yb(ybIn), rTreeTop(rTreeTopIn), rTrunk(rTrunkIn), height(heightIn),
	numTreeTopPoints(numTreeTopPointsIn), numTrunkPoints(numTrunkPointsIn)
{
	validateData();
	defineModel();
}

Tree::~Tree()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Tree::defineModel()
{
	vao[0] = 0; // delete this line and the following "cout" when completing the exercise
	std::cout << "Tree::defineModel: Implementation left as an exercise.\n";
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Tree::getMCBoundingBox(double* xyzLimits) const
{
	double dx1 = xb - rTreeTop; // min x of tree top
	double trunkTheta = asin(height/rTrunk);
	double dx2 = xb - rTrunk*(1.0 - cos(trunkTheta)); // min x of base
	if (dx1 < dx2)
		xyzLimits[0] = dx1;
	else
		xyzLimits[0] = dx2;
	xyzLimits[1] = 2.0*xb - xyzLimits[0];
	xyzLimits[2] = yb;
	xyzLimits[3] = yb + height + rTreeTop;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void Tree::render()
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

	renderTree();

	// restore the previous program
	glUseProgram(pgm);
}

void Tree::renderTree() const
{
	glBindVertexArray(vao[0]);
	// draw trunk first because tree top overwrites it
	glUniform1i(shaderIF->ppuLoc("treePart"), 0); // '0' means trunk
	int numTrunkTriStripPoints = 2 * numTrunkPoints;
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTrunkTriStripPoints); // offset: 0
	// then draw tree top
	glUniform1i(shaderIF->ppuLoc("treePart"), 1); // '1' means tree top
	//                            offset:                 num points:
	glDrawArrays(GL_TRIANGLE_FAN, numTrunkTriStripPoints, numTreeTopPoints);
}

void Tree::validateData()
{
	if (rTreeTop <= 0.0)
		rTreeTop = 1.0;
	if (height <= 0.0)
		height = 1.0;
	if (rTrunk < height)
		rTrunk = height;
	if (numTreeTopPoints < 5)
		numTreeTopPoints = 5;
	if (numTrunkPoints < 2)
		numTrunkPoints = 2;
}
