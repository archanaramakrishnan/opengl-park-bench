// Floor.h

#ifndef FLOOR_H
#define FLOOR_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "SceneElement.h"

class Floor : public SceneElement
{
public:
	Floor(ShaderIF* sIF, const PhongMaterial& matlIn,
		double xmin, double xmax, double ymin, double ymax);
	virtual ~Floor();

	void getMCBoundingBox(double* xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	void render();
	void renderFloor();

private:
	GLuint vao;
	GLuint buffer; // coordinates
	PhongMaterial matl;

	double xyz[6];
	double nx, ny, nz;

	void defineInitialGeometry();
};

#endif
