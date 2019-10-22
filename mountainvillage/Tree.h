// Tree.h - concrete subclass of ModelView that represents a Tree

#ifndef TREE_H
#define TREE_H

#include "ShaderIF.h"

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"

class Tree : public ModelView
{
public:
	// A tree is defined with center of base at (xb,yb). The leafy tree top
	// is a sphere of radius rTreeTop centered 'height' units directy up from
	// the base. The trunk is drawn as a circular arc of radius rTrunk. The
	// center of the arc is (xb +/- rTrunk, yb + height).
	// REQUIREMENT: rTrunk >= d.
	Tree(ShaderIF* sIF,
		double xbIn, double ybIn, double rTreeTopIn, double rTrunkIn, double heightIn,
		int numTreeTopPointsIn=30, int numTrunkPointsIn=25);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderTree() const;
private:
	ShaderIF* shaderIF;
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[1]; // Stores both trunk and treetop points
	int numTrunkTriStripPoints;
	// original defining data
	double xb, yb, rTreeTop, rTrunk, height;
	int numTreeTopPoints, numTrunkPoints;

	void defineModel();
	void validateData();
};

#endif
