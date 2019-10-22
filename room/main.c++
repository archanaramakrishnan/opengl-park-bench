// main.c++: An OpenGL program that creates a nice little room

#include <string.h>

#include "ExtendedController.h"
#include "Barbell.h"
#include "Floor.h"
#include "Tetrahedron.h"
#include "Table.h"

void createRoom(Controller& c, ShaderIF* sIF)
{
	// units are roughly in feet
	cryph::AffPoint c1(-8.0, -5, 0);
	cryph::AffVector u1(1.0, -0.5, 0.0);
	double legHeight = 2.5, tableThick = 0.25;
	PhongMaterial tableMatl(0.8, 0.0, 0.0, 0.3, 0.3, 0.6, 15.0, 1.0);
	std::string t1ImageSrc = "images/ku_seal_1024.png";
	Table* t1 = new Table(sIF, tableMatl, t1ImageSrc,
		c1, u1, legHeight, 0.3, 5.0, 3.5, tableThick);
	c.addModel( t1 );

	cryph::AffPoint c2(1.0, 4, 0);
	cryph::AffVector u2(0.0, 1.0, 0.0);
	std::string t2ImageSrc = "images/albert_500.png";
	Table* t2 = new Table(sIF, tableMatl, t2ImageSrc,
		c2, u2, legHeight, 0.3, 5.0, 3.5, tableThick);
	c.addModel( t2 );

	cryph::AffPoint topTable1 = c1 + (legHeight+tableThick)*cryph::AffVector::zu;
	cryph::AffPoint p0 = topTable1 + cryph::AffVector(2.3, 1.4, 0.0);
	cryph::AffPoint p1 = topTable1 + cryph::AffVector(3.3, 1.8, 0.0);
	cryph::AffPoint p2 = topTable1 + cryph::AffVector(2.3, 2.5, 0.0);
	cryph::AffPoint p3 = topTable1 + cryph::AffVector(2.6, 1.9, 1.6);
	PhongMaterial tetMatl(1.0, 1.0, 0.0, 0.2, 0.2, 0.8, 20.0, 1.0);
	c.addModel( new Tetrahedron(sIF, tetMatl, p0, p1, p2, p3));

	PhongMaterial floorMatl(0.6, 0.8, 0.4, 0.3, 0.5, 0.4, 1.0, 1.0);
	c.addModel( new Floor(sIF, floorMatl, -10.0, 10.0, -10.0, 10.0) );

	PhongMaterial barbellMatl(0.1, 0.1, 0.1, 0.2, 0.5, 9.5, 55.0, 1.0);
	double barR = 0.25, bellR = 1.0;
	c.addModel( new Barbell(sIF, barbellMatl, cryph::AffPoint(-9.0, 7.0, bellR),
		cryph::AffPoint(-6.5, 9.5, bellR), barR, bellR) );
	c.addModel( new Barbell(sIF, barbellMatl, cryph::AffPoint(8.0, -7.0, bellR),
		cryph::AffPoint(8.0, -3.5, bellR), barR, bellR) );
}

void set3DViewingInformation(double overallBB[])
{
	// Notify class ModelView that we initially want to see the entire scene
	// (This assumes 3D Viewing strategy #1.)
	ModelView::setMCRegionOfInterest(overallBB);

	// MC -> EC:

	// TODO: Compute/set eye, center, up:
	cryph::AffPoint eye, center;
	cryph::AffVector up;
	ModelView::setEyeCenterUp(eye, center, up);

	// EC -> LDS:

	// Specify the initial projection type desired
	ModelView::setProjection(PERSPECTIVE);

	// TODO: Compute/set ecZmin, ecZmax
	double ecZmin, ecZmax;
	ModelView::setECZminZmax(ecZmin, ecZmax);

	// TODO: Compute/set ecZpp
	double ecZpp;
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
	int rcFlags = MVC_USE_DEPTH_BIT;
	ExtendedController c(argv[0], rcFlags);
	c.reportVersions(std::cout);
	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	createRoom(c, sIF);

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
