// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"

#include "Tree.h"
#include "Grass.h"
#include "Bench.h"

void addTrees(Controller& c, ShaderIF* sIF)
{
	//Add 9 trees
	c.addModel(new Tree(sIF, 28, -30, 0));
	c.addModel(new Tree(sIF, 25, -15, 0));
	c.addModel(new Tree(sIF, 30, 0, 0));
	c.addModel(new Tree(sIF, 32, 15, 0));
	//c.addModel(new Tree(sIF, 35, 30, 0));

	c.addModel(new Tree(sIF, 28, -20, 15));
	c.addModel(new Tree(sIF, 28, -5, 15));
	c.addModel(new Tree(sIF, 32, 5, 15));
	c.addModel(new Tree(sIF, 35, 20, 15));
}

//ShaderIF* sIF, double height, double xCenter, double zCenter
void addGrass(Controller& c, ShaderIF* sIF)
{
	//the ground is on the XZ plane

	//started out with 100x100 but spaced it out 5*x/z which was too much
	//moved to 20x20 
	//but thought 100 would be better for a denser patch of grass

	for(int x=0; x<100; x++)
	{
		for(int z=0; z<100; z++)
		{
			c.addModel(new Grass(sIF, 0.5, -35+(x*0.6), -35+(0.6*z)));
		}
	}	
	//c.addModel(new Bench(sIF, -2.0, -2.4, -1.2, 5.3, 5.3, 5.4));
}

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	double dXYZ[] = {0.0, 0.0, 0.0};
	float color1[] = { 0.3, 1.0, 1.0 };
	//c.addModel(new M(sIF, dXYZ, color1));
	dXYZ[0] = 0.7; // translate next 'M' by deltaX = 0.7
	float color2[] = { 1.0, 0.25, 0.2 };
	//c.addModel(new M(sIF, dXYZ, color2));
	//c.addModel(new Block(sIF, 0.3, -0.4, 1.2, 0.3, 0.3, 0.4));
	//c.addModel(new Cylinder(sIF));
	addTrees(c, sIF);
	addGrass(c, sIF);
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
	double d=2*r;

	//Dir is any direction parallel to an MC axis
	cryph::AffVector dir=cryph::AffVector( 0.0 , 0.0 , 1.0 );

	cryph::AffPoint eye=center+(d*dir); 

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
	ExtendedController c("A Park Bench", MVC_USE_DEPTH_BIT);
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
