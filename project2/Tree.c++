// Tree.c++

#include <iostream>
#include <math.h>

#include "Tree.h"

Tree::Tree(ShaderIF* sIF, double height, double xCenter, double zCenter) : shaderIF(sIF)
{
	double tetraRadius=0.4*height; //circle around the three points of the base of the tetrahedron
	double cylinderRadius=0.05*height; //radius of the bark of the tree


	//Tetrahedron Group of Leaves
	// top
	const cryph::AffPoint p0(xCenter, height, zCenter);
	// bottom
	//the bark is 3/8th of the height passed in (total height of the tree)
	const cryph::AffPoint p1(xCenter+(tetraRadius*sin(0)), 3*height/8, zCenter+(tetraRadius*cos(0)));
	const cryph::AffPoint p2(xCenter+(tetraRadius*sin(2*M_PI/3)), 3*height/8, zCenter+(tetraRadius*cos(2*M_PI/3))); 
	const cryph::AffPoint p3(xCenter+(tetraRadius*sin(4*M_PI/3)), 3*height/8, zCenter+(tetraRadius*cos(4*M_PI/3)));

	float R=0.0;
	float G=0.6;
	float B=0.2;

	kdTetra[0] = R; kdTetra[1] = G; kdTetra[2] = B;
	kaTetra[0] = R; kaTetra[1] = G; kaTetra[2] = B;
	// put vertices in array to simplify generation of geometry:
	cryph::AffPoint verts[] = { p0, p1, p2, p3 };
	defineTetrahedron(verts);
	// set min/max coordinates for MC bounding box:
	minMax[0] = minMax[1] = p0.x;
	minMax[2] = minMax[3] = p0.y;
	minMax[4] = minMax[5] = p0.z;
	updateXYZBounds(p1);
	updateXYZBounds(p2);
	updateXYZBounds(p3);

	//CYLINDER
	kdCylinder[0] = 0.545; kdCylinder[1] = 0.271; kdCylinder[2] = 0.074;
	kaCylinder[0] = 0.545; kaCylinder[1] = 0.271; kaCylinder[2] = 0.074;
	double x1 = 0, x2 = height * .5;
	double yb = -0.35, zb = 1.4;
	xmin = x1;
	xmax = x2;
	ymin = yb - cylinderRadius;
	ymax = yb + cylinderRadius;
	zmin = zb - cylinderRadius;
	zmax = zb + cylinderRadius;
	defineCylinder(x1, x2, xCenter, zCenter, cylinderRadius);
}

Tree::~Tree()
{
	glDeleteBuffers(1, vboCylinder);
	glDeleteVertexArrays(2, vaoCylinder);

	glDeleteBuffers(1, vboTetra);
	glDeleteVertexArrays(1, vaoTetra);
}

void Tree::defineTetrahedron(const cryph::AffPoint verts[])
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
	glGenVertexArrays(1, vaoTetra);
	glGenBuffers(1, vboTetra);

	// Initialize vao[0]
	glBindVertexArray(vaoTetra[0]);

	//initialize vbo[0]
	glBindBuffer(GL_ARRAY_BUFFER, vboTetra[0]);

	// Allocate space for AND send data to GPU
	int numBytesInBuffer = 6 * sizeof(vec3);
	std::cout << numBytesInBuffer << "\n";
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
	}

void Tree::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = minMax[i];
}

bool Tree::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// Tetrahedron does not look for events. Just hand off back to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}
const int N_POINTS_AROUND_SLICE = 18; // number points around a cross-section

void Tree::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	glBindVertexArray(vaoTetra[0]);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kdTetra);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, kaTetra);

	// draw the four faces:
	for (int fi=0 ; fi<4 ; fi++)
	{
		glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), normal[fi].dx, normal[fi].dy, normal[fi].dz);
		glDrawArrays(GL_TRIANGLE_STRIP, fi, 3);
	}

	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kdCylinder);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, kaCylinder);
	glBindVertexArray(vaoCylinder[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

	glUseProgram(pgm);
}

void Tree::updateXYZBounds(const cryph::AffPoint& p)
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



void Tree::defineCylinder(double y1, double y2, double xb, double zb, double r)
{
	typedef float vec3[3];
	int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1); // "+1" because last point = first
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	double theta = 0.0;
	double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;
	
	for (int i=0 ; i<nPoints ; i+=2)
	{
		// Each time through this loop, create two points and their corresponding
		// (and common) normal vectors
		double nx=r*cos(theta);
		double nz=r*sin(theta);

		//vboPts[i] = (x1, ny, nz)
		coords[i][0] = nx +xb;
		coords[i][1] = y1;
		coords[i][2] = nz+zb;

		//vboPts[i+1] = (x2, ny, nz)
		coords[i+1][0] = nx+xb;
		coords[i+1][1] = y2;
		coords[i+1][2] = nz+zb;

        // ny and nz also specify the normal vector along this ruling:
        //vboNormals[i] = (0, ny, nz)
		normals[i][0] = nx;
		normals[i][1] = 0;
		normals[i][2] = nz;

		//vboNormals[i+1] = (0, ny, nz);
		normals[i+1][0] = nx;
		normals[i+1][1] = 0;
		normals[i+1][2] = nz;

		// Refer to 672's InClass/3DModeling/3DGeometryAndAttributeModeling101.html

		theta += dTheta;
	}

	// Create the VAO and VBO names
	glGenVertexArrays(1, vaoCylinder);
	glGenBuffers(2, vboCylinder);

	// Initialize vao[0]
	glBindVertexArray(vaoCylinder[0]);

	//initialize vbo[0]
	glBindBuffer(GL_ARRAY_BUFFER, vboCylinder[0]);

	// Allocate space for AND send data to GPU
	int numBytesInBuffer = nPoints * sizeof(vec3);
	std::cout << numBytesInBuffer << "\n";
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	//initialize vbo[1]
	glBindBuffer(GL_ARRAY_BUFFER, vboCylinder[1]);

	// Allocate space for AND send data to GPU
	std::cout << numBytesInBuffer << "\n";
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

	delete [] coords;
	delete [] normals;
}
