// Floor.c++

#include "Floor.h"

Floor::Floor(ShaderIF* sIF, const PhongMaterial& matlIn,
	double xmin, double xmax, double ymin, double ymax) :
	SceneElement(sIF), matl(matlIn), nx(0.0), ny(0.0), nz(1.0)
{
	xyz[0] = xmin; xyz[1] = xmax;
	xyz[2] = ymin; xyz[3] = ymax;
	xyz[4] = 0.0;  xyz[5] = 0.0;
	defineInitialGeometry();
}

Floor::~Floor()
{
	glDeleteBuffers(1, &buffer);
	glDeleteVertexArrays(1, &vao);
}

void Floor::defineInitialGeometry()
{
	float mcPosition[4][3] =
	{
		{ static_cast<float>(xyz[0]), static_cast<float>(xyz[2]), 0.0 },
		{ static_cast<float>(xyz[0]), static_cast<float>(xyz[3]), 0.0 },
		{ static_cast<float>(xyz[1]), static_cast<float>(xyz[2]), 0.0 },
		{ static_cast<float>(xyz[1]), static_cast<float>(xyz[3]), 0.0 }
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), mcPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
}

void Floor::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = xyz[i];
}

void Floor::renderFloor()
{
	establishMaterial(matl);
	glBindVertexArray(vao);

	// set the normal to the floor
	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), nx, ny, nz);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Floor::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderIF->getShaderPgmID());

	establishLightingEnvironment();
	establishView();

	renderFloor();

	// restore the previous program
	glUseProgram(pgm);
}
