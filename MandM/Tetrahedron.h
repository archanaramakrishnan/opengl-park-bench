// Tetrahedron.h

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "AffPoint.h"
#include "AffVector.h"
#include "ModelView.h"
#include "ShaderIF.h"

class Tetrahedron : public ModelView
{
public:
	Tetrahedron(ShaderIF* sIF,
		const cryph::AffPoint& p0, const cryph::AffPoint& p1,
		const cryph::AffPoint& p2, const cryph::AffPoint& p3,
		float R=1.0, float G=0.0, float B=0.0);
	virtual ~Tetrahedron();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();
	void renderTetrahedron();

	void defineCylinder(double x1, double x2, double yb, double zb, double r);
private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];

	float kd[3]; // the RGB diffuse color of this tetrahedron
	cryph::AffVector normal[4]; // the four normals to the four faces
	double minMax[6]; // for MC bounding box

	void defineTetrahedron(const cryph::AffPoint verts[]);
	void updateXYZBounds(const cryph::AffPoint& p);
};

#endif
