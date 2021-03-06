// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"

#include "Tree.h"
#include "Grass.h"
#include "Bench.h"

void addTrees(Controller& c, ShaderIF* sIF)
{
	//Add 8 trees
	c.addModel(new Tree(sIF, 23, -30, 0));
	c.addModel(new Tree(sIF, 20, -15, 0));
	c.addModel(new Tree(sIF, 25, 0, 0));
	c.addModel(new Tree(sIF, 27, 15, 0));
	c.addModel(new Tree(sIF, 23, -20, 15));
	c.addModel(new Tree(sIF, 23, -5, 15));
	c.addModel(new Tree(sIF, 27, 5, 15));
	c.addModel(new Tree(sIF, 30, 20, 15));
}

void addGrass(Controller& c, ShaderIF* sIF)
{
	double offset;
	for(int x=0; x<100; x=x+1)
	{
		for(int z=0; z<100; z=z+1)
		{
			offset=(rand() % 10 + 1) * 0.1;
			c.addModel(new Grass(sIF, 0.5, (offset-35)+(0.6*x), (offset-18)+(0.6*z)));
		}
	}	

	c.addModel(new Block(sIF, -35.0, -0.01, -18.0, 61.0, 0.02, 61.0, 0.098, 0.0627, 0.0705));
}

void addPicnicTable(Controller& c, ShaderIF* sIF)
{	
	//bench 1
	c.addModel(new Bench(sIF, -14.0, 25.2));

	//bench 2
	c.addModel(new Bench(sIF, -4.0, 25.2));
}

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	double dXYZ[] = {0.0, 0.0, 0.0};
	float color1[] = { 0.3, 1.0, 1.0 };
	float color2[] = { 1.0, 0.25, 0.2 };
	addTrees(c, sIF);
	addGrass(c, sIF);
	addPicnicTable(c, sIF);
}

void set3DViewingInformation(double overallBB[])
{
	ModelView::setMCRegionOfInterest(overallBB);

	// MC -> EC:

	//Compute max and mins
	double xMin=overallBB[0]; double xMax=overallBB[1];
	double yMin=overallBB[2]; double yMax=overallBB[3];
	double zMin=overallBB[4]; double zMax=overallBB[5];

	//Center coordinates are coordinates of the midpoint of the MC Region of Interest
	double x=(xMin+xMax)/2;
	double y=(yMin+yMax)/2;
	double z=(zMin+zMax)/2;
	cryph::AffPoint center(x, y, z);

	//Radius max(xMax-xMin/2, yMax-yMin/2, zMax-zMin/2)
	double maxOfXY=std::max((xMax-xMin/2), (yMax-yMin/2));
	double r=std::max(maxOfXY, (zMax-zMin/2));

	//Distance
	double d=4*r;

	//Dir is any direction parallel to an MC axis
	cryph::AffVector dir=cryph::AffVector( 0.0 , 0.0 , 1.0 );

	cryph::AffPoint eye=center+(d*dir) + cryph::AffVector( 0.0 , 40.0 , 0.0 ); 

	//Up: any direction parallel to an MC axis thats is not parallel to dir
	cryph::AffVector up=cryph::AffVector( 0.0 , 1.0 , 0.0 );

	ModelView::setEyeCenterUp(eye, center, up);

	// EC -> LDS
	// Initial projection type
	ModelView::setProjection(PERSPECTIVE);

	double ecZmin=-d-r;
	double ecZmax=-d+r;
	ModelView::setECZminZmax(ecZmin, ecZmax);

	double ecZpp=ecZmax;
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
	ExtendedController c("Park Benches", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	createScene(c, sIF);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	std::cout << "Bounding box: " << xyz[0] << " <= x <= " << xyz[1] << '\n';
	std::cout << "              " << xyz[2] << " <= y <= " << xyz[3] << '\n';
	std::cout << "              " << xyz[4] << " <= z <= " << xyz[5] << "\n\n";
	set3DViewingInformation(xyz);

	c.run();

	delete sIF;

	return 0;
}
