// Mountain.c++ - concrete subclass of ModelView that represents a Mountain

#include <iostream>
#include <cmath>

#include "Mountain.h"

typedef float vec2[2];

Mountain::Mountain(ShaderIF* sIF, double xMinIn, double xMaxIn, double yMinIn,
		double yMaxIn, double freq, double phase,
		int numSamplePointsIn) : shaderIF(sIF),
	xMin(xMinIn), xMax(xMaxIn), yMin(yMinIn), yMax(yMaxIn),
	frequency(freq), phaseShift(phase),
	numSamplePoints(numSamplePointsIn)
{
	defineModel();
}

Mountain::~Mountain()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

void Mountain::defineModel()
{
	vao[0] = 0; // delete this line and the following "cout" when completing the exercise
	std::cout << "Mountain::defineModel: Implementation left as exercise.\n";
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Mountain::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xMin;
	xyzLimits[1] = xMax;
	xyzLimits[2] = yMin;
	xyzLimits[3] = yMax;
	// All z coordinates are implicitly zero, nevertheless:
	xyzLimits[4] = -1.0;
	xyzLimits[5] = 1.0;
}

void Mountain::render()
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

	renderMountain();

	// restore the previous program
	glUseProgram(pgm);
}

void Mountain::renderMountain() const
{
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*numSamplePoints);
}
