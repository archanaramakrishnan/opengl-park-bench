// Block.h

#ifndef BENCH_H
#define BENCH_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"


class Bench : public ModelView
{
public:
	Bench(ShaderIF* sIF, double xCorner, double zCorner);// coordinates of left corner of bench, y is always 0
	virtual ~Bench();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();
	//void renderBlock();
private:
	Block* blocks[5];

	// "float" here to match the need to send float to VBO.
	float xmin, xmax, ymin, ymax, zmin, zmax;
};

#endif
