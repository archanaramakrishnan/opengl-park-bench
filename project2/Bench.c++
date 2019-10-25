// Block.c++

#include <iostream>
#include <math.h>

#include "Bench.h"

typedef float vec3[3];

//dimensions of the bench
const double height = 2.5;
const double length = 6.0;
const double width = 2.0;
const double thickness = 0.3; //few inches thickness
float R=0.639;
float G=0.631;
float B=0.584;
const double legDimension = length/15;	//square n*n dimension of cuboid cross section

Bench::Bench(ShaderIF* sIF, double xCorner, double zCorner)
{
	xmin = xCorner; xmax = xCorner + length;
	ymin = 0; ymax = height;
	zmin = zCorner; zmax = zCorner + width;

	//top
	blocks[0]= new Block(sIF, xmin, height - thickness, zmin, length, thickness, width, R, G, B);

	//legs
	blocks[1]= new Block(sIF, xmin, 0, zmin, legDimension, height - thickness, legDimension, R, G, B);
	blocks[2]= new Block(sIF, xmin, 0, zmin+width-legDimension, legDimension, height - thickness, legDimension, R, G, B);
	blocks[3]= new Block(sIF, xmin+length-legDimension, 0, zmin, legDimension, height - thickness, legDimension, R, G, B);
	blocks[4]= new Block(sIF, xmin+length-legDimension, 0, zmin+width-legDimension, legDimension, height - thickness, legDimension, R, G, B);
}

Bench::~Bench()
{
	for(int i=0; i<5; i++)
	{
		delete blocks[i];
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Bench::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

bool Bench::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	// Block does not look for events; just hand off to inherited handleCommand.
	return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Bench::render()
{
	for(int i=0; i<5; i++)
	{
		blocks[i]->render();
	}
}
