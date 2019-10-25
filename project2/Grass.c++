// Grass.c++
#include <iostream>
#include <math.h>

#include "Grass.h"

Grass::Grass(ShaderIF* sIF, double height, double xCenter, double zCenter) : shaderIF(sIF)
{
	kd[0] = 0.0; kd[1] = 0.5; kd[2] = 0.0;
	ka[0] = 0.0; ka[1] = 0.5; ka[2] = 0.0;
	// put vertices in array to simplify generation of geometry:

	double grassRadius=0.8*height; //circle around the three points of the base of the blade of grass

	const cryph::AffPoint p0(xCenter, height, zCenter);
	// bottom
	//the bark is 3/8th of the height passed in (total height of the tree)
	const cryph::AffPoint p1(xCenter+(grassRadius*sin(0)), 0, zCenter+(grassRadius*cos(0)));
	const cryph::AffPoint p2(xCenter+(grassRadius*sin(2*M_PI/3)), 0, zCenter+(grassRadius*cos(2*M_PI/3))); 
	const cryph::AffPoint p3(xCenter+(grassRadius*sin(4*M_PI/3)), 0, zCenter+(grassRadius*cos(4*M_PI/3)));


	cryph::AffPoint verts[] = { p0, p1, p2, p3 };
	defineTetrahedron(verts);
	// set min/max coordinates for MC bounding box:
	minMax[0] = minMax[1] = p0.x;
	minMax[2] = minMax[3] = p0.y;
	minMax[4] = minMax[5] = p0.z;
	updateXYZBounds(p1);
	updateXYZBounds(p2);
	updateXYZBounds(p3);
}

Grass::~Grass()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Grass::defineTetrahedron(const cryph::AffPoint verts[])
{
	typedef float vec3[3];

	// We need SIX vertices for GL_TRIANGLE_STRIP; the last two
	// are copies of the first two:
	vec3 vtx[6];
	for (int i=0 ; i<6 ; i++)
		// "i%4" because last two are copies of first two
		verts[i%4].aCoords(vtx, i);

	// There will be FOUR faces, so compute those FOUR normal vectors:
	for (int i=0 ; i<4 ; i++)
	{
		int viP1 = (i + 1) % 4;
		int viP2 = (i + 2) % 4;
		cryph::AffVector v01 = verts[viP1] - verts[i];
		cryph::AffVector v02 = verts[viP2] - verts[i];
		normal[i] = v01.cross(v02);
	}

	// ******************************************************************
	// EXERCISE: Do the usual VAO/VBO magic here so that:
	//           1) coordinates are sent in VBO
	//           2) Normals will NOT be sent in VBO; rather they will
	//              be set face-by-face (i.e., on a per-primitive basis)
	//              in Tetrahedron::renderTetrahedron
	// ******************************************************************

	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);

	// Initialize vao[0]
	glBindVertexArray(vao[0]);

	//initialize vbo[0]
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Allocate space for AND send data to GPU
	int numBytesInBuffer = 6 * sizeof(vec3);
	std::cout << numBytesInBuffer << "\n";
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
}

void Grass::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = minMax[i];
}

bool Grass::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// Tetrahedron does not look for events. Just hand off back to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Grass::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderTetrahedron();

	glUseProgram(pgm);
}

void Grass::renderTetrahedron()
{
	glBindVertexArray(vao[0]);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);

	// draw the four faces:
	for (int fi=0 ; fi<4 ; fi++)
	{
		glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), normal[fi].dx, normal[fi].dy, normal[fi].dz);
		glDrawArrays(GL_TRIANGLE_STRIP, fi, 3);
	}
}

void Grass::updateXYZBounds(const cryph::AffPoint& p)
{
	if (p.x < minMax[0])
		minMax[0] = p.x;
	else if (p.x > minMax[1])
		minMax[1] = p.x;

	if (p.y < minMax[2])
		minMax[2] = p.y;
	else if (p.y > minMax[3])
		minMax[3] = p.y;

	if (p.z < minMax[4])
		 minMax[4] = p.z;
	else if (p.z > minMax[5])
		minMax[5] = p.z;
}