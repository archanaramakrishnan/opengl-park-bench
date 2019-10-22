// main.c++: Driver for creating Houses and Trees on Mountains

#include <iostream>
#include <fstream>
#include <string>

#include "GLFWController.h"
#include "House.h"
#include "Mountain.h"
#include "Tree.h"

ShaderIF* house_sIF = nullptr;
ShaderIF* mountain_sIF = nullptr;
ShaderIF* tree_sIF = nullptr;

void makeDefaultScene(Controller& c)
{
	c.addModel( new Mountain(mountain_sIF, -200, 200, -20, 60, 1.5, 0.5) );
	c.addModel( new Mountain(mountain_sIF, -200, 200, -20, 20, 1.0, -M_PI/2.0) );
	c.addModel( new Tree(tree_sIF, -150.0, -120.0, 18.0, 60.0, 45.0) );
	c.addModel( new House(house_sIF, -15.0, -150.0, 85.0, 70.0, 95.0, 35.0, "Rock Chalk") );
	c.addModel( new House(house_sIF, 60.0, -10.0, 15.0, 12.0, 25.0, 10.0, "Jayhawk") );
	c.addModel( new Tree(tree_sIF, 120.0, -220.0, 43.0, 220.0, 125.0) );
}

House* readHouse(std::istream& is)
{
	double xb, yb, width, height, roofWidth, roofHeight;
	is >> xb >> yb >> width >> height >> roofWidth >> roofHeight;
	std::string label;
	getline(is, label);
	return new House(house_sIF, xb, yb, width, height, roofWidth, roofHeight, label);
}

Mountain* readMountain(std::istream& is)
{
	double xMin, xMax, yMin, yMax, freq, phase;
	int numSamplePts;
	is >> xMin >> xMax >> yMin >> yMax >> freq >> phase;
	is >> numSamplePts;
	return new Mountain(mountain_sIF, xMin, xMax, yMin, yMax, freq, phase,
		numSamplePts);
}

Tree* readTree(std::istream& is)
{
	double xb, yb, rTreeTop, rTrunk, height;
	int nTreeTopPts, nTrunkPts;
	is >> xb >> yb >> rTreeTop >> rTrunk >> height >> nTreeTopPts >> nTrunkPts;
	return new Tree(tree_sIF, xb, yb, rTreeTop, rTrunk, height, nTreeTopPts, nTrunkPts);
}

bool readScene(char* fName, Controller& c)
{
	std::ifstream is(fName);
	if (!is.good())
	{
		std::cerr << "Could not open " << fName << " for reading. Generating default scene.\n";
		return false;
	}
	std::string obj;
	while (is >> obj)
	{
		if (obj == "Mountain")
			c.addModel( readMountain(is) );
		else if (obj == "Tree")
			c.addModel( readTree(is) );
		else if (obj == "House")
			c.addModel( readHouse(is) );
		else
		{
			std::cerr << "Unrecognized object: " << obj << '\n';
			is.close();
			return false;
		}
	}
	is.close();
	return true;
}

static char* defaultFont = (char*)("../fontutil/fonts/LucidaCalligraphyItalic24.fnt");

int main(int argc, char* argv[])
{
	GLFWController c("Mountain Village");

	house_sIF = new ShaderIF("shaders/House.vsh", "shaders/House.fsh");
	mountain_sIF = new ShaderIF("shaders/Mountain.vsh", "shaders/Mountain.fsh");
	tree_sIF = new ShaderIF("shaders/Tree.vsh", "shaders/Tree.fsh");

	char* useFont = nullptr;
	bool haveScene = false;
	for (int i=1 ; i<argc ; i++)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'f')
				useFont = argv[++i];
		}
		else
		{
			if (useFont == nullptr)
				useFont = defaultFont;
			House::setHouseLabelFont(useFont);
			haveScene = readScene(argv[i], c);
		}
	}
	if (!haveScene)
	{
		if (useFont == nullptr)
			useFont = defaultFont;
		House::setHouseLabelFont(useFont);
		makeDefaultScene(c);
	}
	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	std::cout << "Bounding box: " << xyz[0] << " <= x <= " << xyz[1] << '\n';
	std::cout << "              " << xyz[2] << " <= y <= " << xyz[3] << '\n';
	std::cout << "              " << xyz[4] << " <= z <= " << xyz[5] << "\n\n";
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	delete house_sIF;
	delete mountain_sIF;
	delete tree_sIF;

	return 0;
}
