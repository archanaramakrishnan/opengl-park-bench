// Table.h

#ifndef TABLE_H
#define TABLE_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif
#include <string>

#include "SceneElement.h"
#include "AffPoint.h"
#include "AffVector.h"
#include "BasicShapeRenderer.h"

class Table : public SceneElement
{
public:
	Table(ShaderIF* sIF, const PhongMaterial& matlIn, const std::string& texImage,
		cryph::AffPoint corner, cryph::AffVector u,
		double legHeight, double legRadius,
		double tableWidth, double tableDepth, double tableThickness);
	virtual ~Table();

	void getMCBoundingBox(double* xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	void render();
	void renderTable();

private:
	BasicShape* pieces[5]; // top and four legs
	BasicShapeRenderer* piecesR[5];
	int currentlyDrawingPiece; // used only in context of "prepareForFace"
	GLuint texID;
	PhongMaterial matl;

	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
		double legHeight, double legRadius,
		double tableWidth, double tableDepth, double tableThickness);

	static void prepareForFace(void* caller, int faceIndex);
};

#endif
