// M.c++

#include <iostream>
#include <math.h>

#include "M.h"

M::M(ShaderIF* sIF, double dXYZ[], float colorIn[]) : shaderIF(sIF)
{
	for (int i=0 ; i<3 ; i++)
		kd[i] = colorIn[i];
	defineM(dXYZ);
}

M::~M()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void M::defineM(double dXYZ[])
{
	typedef float vec3[3];

	vec3 vtx[] = {
		{-0.6, -0.25, -0.7}, { -0.6, -0.25, 0.25},
		{-0.6,  0.25, -0.7}, { -0.6,  0.25, 0.25},
		{-0.35, 0.0 , -0.7}, { -0.35, 0.0 , 0.25},
		{-0.1 , 0.25, -0.7}, { -0.1 , 0.25, 0.25},
		{-0.1 ,-0.25, -0.7}, { -0.1 ,-0.25, 0.25}
	};
	for (int i=0 ; i<10 ; i++)
		for (int j=0 ; j<3 ; j++)
			vtx[i][j] += dXYZ[j];
	// Remember the min/max XYZ for getMCBoundingBox:
	xyzMinMax[0] = -0.6 + dXYZ[0];
	xyzMinMax[1] = -0.1 + dXYZ[0];
	xyzMinMax[2] = -0.25 + dXYZ[1];
	xyzMinMax[3] =  0.25 + dXYZ[1];
	xyzMinMax[4] = -0.7 + dXYZ[2];
	xyzMinMax[5] =  0.25 + dXYZ[2];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 10*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void M::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = xyzMinMax[i];
}

bool M::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// M does not look for events. Just hand off back to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void M::renderM()
{
	glBindVertexArray(vao[0]);
	// Exercise: show that the normal vectors specified here
	//           are correct.
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 1.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);
}

void M::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderM();

	glUseProgram(pgm);
}
