// Tetrahedron.h

#ifndef GRASS_H
#define GRASS_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"
#include "ModelView.h"
#include "ShaderIF.h"

class Grass : public ModelView
{
public:
	Grass(ShaderIF* sIF, double height, double xCenter, double zCenter);
	virtual ~Grass();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();
	void renderTetrahedron();
private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];

	float kd[3]; // the RGB diffuse color of the grass
	cryph::AffVector normal[4]; // the four normals to the four faces
	double minMax[6]; // for MC bounding box

	void defineTetrahedron(const cryph::AffPoint verts[]);
	void updateXYZBounds(const cryph::AffPoint& p);
};

#endif
