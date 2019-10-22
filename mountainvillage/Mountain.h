// Mountain.h - concrete subclass of ModelView that creates a Mountain on which
//              some of our Houses and Trees are placed.

#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "ShaderIF.h"

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"

class Mountain : public ModelView
{
public:
	// A Mountain is modeled as a cosine curve defined over -PI <= theta <= PI.
	// (A "mountain range" can then be assembled by creating several individual
	// mountains.) When generating the geometry, the actual coordinates will
	// be mapped from the nominal (-PI, +PI, -1, +1) range to the provided
	// (xmin, xmax, ymin, ymax) range. The frequency and phase shift parameters
	// will be applied in the usual way as: cos(freq*theta + phaseShift).
	// The mountain is of course generated as a piecewise linear approximation
	// to the cosine curve; "numSamplePoints" specifies how many samples are
	// to be taken along the curve.
	Mountain(ShaderIF* sIF, double xMinIn, double xMaxIn, double yMinIn,
	         double yMaxIn, double freq, double phase,
	         int numSamplePointsIn=30);
	virtual ~Mountain();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderMountain() const;
private:
	ShaderIF* shaderIF;
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[2]; // [0] has coordinates; [1] has pv fraction up mountain
	// original defining data
	double xMin, xMax, yMin, yMax, frequency, phaseShift;
	int numSamplePoints;

	void defineModel();
};

#endif
