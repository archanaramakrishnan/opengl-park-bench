// Block.c++

#include <iostream>
#include <math.h>

#include "Bench.h"

typedef float vec3[3];

//dimensions of the bench
const double height = 2.5;
const double length = 4.0;
const double width = 1.5;
const double thickness = 0.167; //2 inches thickness

Bench::Bench(ShaderIF* sIF, double xCorner, double zCorner)
{
	xmin = xCorner; xmax = xCorner + length;
	ymin = 0; ymax = height;
	zmin = zCorner; zmax = zCorner + width;

	//top
	blocks[0]= new Block(sIF, xmin, height - thickness, zmin, length, thickness, width);

	const double legDimension = length/10;	//square n*n dimension of cuboid cross section

	//legs
	blocks[1]= new Block(sIF, xmin, 0, zmin, legDimension, height - thickness, legDimension);
	blocks[2]= new Block(sIF, xmin, 0, zmin+width-legDimension, legDimension, height - thickness, legDimension);
	blocks[3]= new Block(sIF, xmin+length-legDimension, 0, zmin, legDimension, height - thickness, legDimension);
	blocks[4]= new Block(sIF, xmin+length-legDimension, 0, zmin+width-legDimension, legDimension, height - thickness, legDimension);
	
	// kd[0] = 0.7; kd[1] = 0.7; kd[2] = 0.0;
	// defineBlock();
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
