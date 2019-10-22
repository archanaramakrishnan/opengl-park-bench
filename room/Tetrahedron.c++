// Tetrahedron.c++

#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(ShaderIF* sIF, const PhongMaterial& matlIn,
		cryph::AffPoint P0, cryph::AffPoint P1, cryph::AffPoint P2,
		cryph::AffPoint P3) : SceneElement(sIF), matl(matlIn)
{
	xyz[0] = xyz[1] = P0[0];
	xyz[2] = xyz[3] = P0[1];
	xyz[4] = xyz[5] = P0[2];
	updateXYZBounds(P1);
	updateXYZBounds(P2);
	updateXYZBounds(P3);

	defineInitialGeometry(P0, P1, P2, P3);
}

Tetrahedron::~Tetrahedron()
{
	glDeleteBuffers(1, &buffer);
	glDeleteVertexArrays(1, &vao);
}

void Tetrahedron::defineInitialGeometry(cryph::AffPoint P0, cryph::AffPoint P1,
	cryph::AffPoint P2, cryph::AffPoint P3)
{
	float mcPosition[12][3];

	// face 0: P0-P1-P2
	P0.aCoords(mcPosition[0]); P1.aCoords(mcPosition[1]); P2.aCoords(mcPosition[2]);
	n[0] = (P2-P1).cross(P0-P1);
	// face 1: P0-P1-P3
	P0.aCoords(mcPosition[3]); P1.aCoords(mcPosition[4]); P3.aCoords(mcPosition[5]);
	n[1] = (P3-P1).cross(P0-P1);
	// face 2: P0-P2-P3
	P0.aCoords(mcPosition[6]); P2.aCoords(mcPosition[7]); P3.aCoords(mcPosition[8]);
	n[2] = (P2-P0).cross(P0-P3);
	// face 3: P1-P2-P3
	P1.aCoords(mcPosition[9]); P2.aCoords(mcPosition[10]); P3.aCoords(mcPosition[11]);
	n[3] = (P2-P1).cross(P3-P1);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 12*3*sizeof(float), mcPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
}

void Tetrahedron::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = xyz[i];
}

void Tetrahedron::renderTetrahedron()
{
	establishMaterial(matl);
	glBindVertexArray(vao);

	// set the normal to the Tetrahedron
	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	float vc[3];
	for (int i=0 ; i<4 ; i++)
	{
		glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), n[i].vComponents(vc));
		glDrawArrays(GL_TRIANGLES, i*3, 3);
	}
}

void Tetrahedron::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderIF->getShaderPgmID());

	establishLightingEnvironment();
	establishView();

	renderTetrahedron();

	// restore the previous program
	glUseProgram(pgm);
}

void Tetrahedron::updateXYZBounds(cryph::AffPoint p)
{
	if (p[0] < xyz[0])
		xyz[0] = p[0];
	else if (p[0] > xyz[1])
		xyz[1] = p[0];

	if (p[1] < xyz[2])
		xyz[2] = p[1];
	else if (p[1] > xyz[3])
		xyz[3] = p[1];

	if (p[2] < xyz[4])
		xyz[4] = p[2];
	else if (p[2] > xyz[5])
		xyz[5] = p[2];
}
