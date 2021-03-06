// Tree.h

#ifndef TREE_H
#define TREE_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"
#include "ModelView.h"
#include "ShaderIF.h"

class Tree : public ModelView
{
public:
	Tree(ShaderIF* sIF, double height, double xCenter, double zCenter);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void defineCylinder(double y1, double y2, double xb, double zb, double r);
	void render();
	//combined render code of the cylinder and tetrahedron into one function
	//instead of renderCylinder() and renderTetrahedron
	//because of an unimplemented pure virtual method 'render' from Modelview.h

private:
	ShaderIF* shaderIF;
	GLuint vaoCylinder[1];
	GLuint vboCylinder[2];

	GLuint vaoTetra[1];
	GLuint vboTetra[1];

	float kdTetra[3]; // the RGB diffuse color of the leaves
	float kdCylinder[3]; // the RGB diffuse color of the tree bark

	float kaTetra[3]; // the RGB diffuse color of the leaves
	float kaCylinder[3]; // the RGB diffuse color of the tree bark
	

	cryph::AffVector normal[4]; // the four normals to the four faces
	double minMax[6]; // for MC bounding box

	double xmin, xmax, ymin, ymax, zmin, zmax; //cylinder

	void defineTetrahedron(const cryph::AffPoint verts[]);
	void updateXYZBounds(const cryph::AffPoint& p);
};

#endif
