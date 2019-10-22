// Tetrahedron.h

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "SceneElement.h"
#include "AffPoint.h"
#include "AffVector.h"

class Tetrahedron : public SceneElement
{
public:
	Tetrahedron(ShaderIF* sIF, const PhongMaterial& matlIn,
		cryph::AffPoint P0, cryph::AffPoint P1, cryph::AffPoint P2,
		cryph::AffPoint P3);
	virtual ~Tetrahedron();

	void getMCBoundingBox(double* xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	void render();
	void renderTetrahedron();

private:
	GLuint vao;
	GLuint buffer; // coordinates
	PhongMaterial matl;

	cryph::AffVector n[4]; // the four normals to the four faces

	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint P0, cryph::AffPoint P1,
		cryph::AffPoint P2, cryph::AffPoint P3);
	void updateXYZBounds(cryph::AffPoint p);
};

#endif
