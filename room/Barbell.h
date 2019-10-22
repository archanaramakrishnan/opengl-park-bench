// Barbell.h

#ifndef BARBELL_H
#define BARBELL_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "SceneElement.h"
#include "AffPoint.h"
#include "AffVector.h"
#include "BasicShapeRenderer.h"

class Barbell : public SceneElement
{
public:
	Barbell(ShaderIF* sIF, const PhongMaterial& matlIn,
		cryph::AffPoint P1, cryph::AffPoint P2,
		double radiusBar, double radiusBell);
	virtual ~Barbell();

	void getMCBoundingBox(double* xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	void render();
	void renderBarbell();

private:
	BasicShape* pieces[3]; // two bells and a bar
	BasicShapeRenderer* piecesR[3];
	PhongMaterial matl;

	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint P1, cryph::AffPoint P2,
		double radiusBar, double radiusBell);
};

#endif
