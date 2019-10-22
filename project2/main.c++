// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"

#include "Tree.h"

void addTetrahedra(Controller& c, ShaderIF* sIF)
{
	c.addModel(new Tree(sIF, 1, 0, 0));
	/*cryph::AffPoint p0(-1.1, -0.4, 0.4);
	cryph::AffPoint p1(-0.7, -0.4, 0.6);
	cryph::AffPoint p2(-0.3, -0.4, 0.5);
	cryph::AffPoint p3(-0.6, -0.1, 0.5);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3));

	p0.assign(0.971345, -0.500694, 1.11945);
	p1.assign(0.651218, -0.376426, 1.26641);
	p2.assign(1.04476, -0.30766, 1.16418);
	p3.assign(0.73595, -0.240765, 1.29965);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3, 0, 0.6, 0.2));

	p0.assign(0.650288, -0.167408, 0.514672);
	p1.assign(0.559247, -0.394237, 0.405895);
	p2.assign(0.548077, -0.133869, 0.479483);
	p3.assign(0.500305, -0.177127, 0.48515);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3, 0.2, 0.3, 0.8));

	p0.assign(1.25395, -0.248015, 0.674291);
	p1.assign(1.31247, -0.0525569, 0.6631);
	p2.assign(1.20728, -0.103504, 0.68977);
	p3.assign(1.22578, -0.324471, 0.594728);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3, 0.9, 0.6, 0.2));

	p0.assign(0.399133, -0.239639, 0.412327);
	p1.assign(0.214498, -0.113268, 0.450438);
	p2.assign(0.23978, -0.397119, 0.427417);
	p3.assign(0.113959, -0.212317, 0.450724);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3, 0.3, 0.7, 0.2));

	p0.assign(-0.286891, -0.140591, 0.471908);
	p1.assign(-0.0966624, -0.258691, 0.352737);
	p2.assign(-0.0833539, -0.0689214, 0.398166);
	p3.assign(-0.178247, -0.0579734, 0.466438);
	c.addModel(new Tetrahedron(sIF, p0, p1, p2, p3, 0.3, 0.3, 0.7));*/
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
	addTetrahedra(c, sIF);
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
