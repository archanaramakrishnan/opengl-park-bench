// Table.c++

#include "Table.h"

Table::Table(ShaderIF* sIF, const PhongMaterial& matlIn, const std::string& imgSrc,
		cryph::AffPoint corner, cryph::AffVector u,
		double legHeight, double legRadius,
		double tableWidth, double tableDepth, double tableThickness)
		 : SceneElement(sIF), texID(0), matl(matlIn)
{
	if (imgSrc.length() > 0)
		texID = readTextureImage(imgSrc);
	defineInitialGeometry(corner, u, legHeight, legRadius,
		tableWidth, tableDepth, tableThickness);
	xyz[0] = 1.0; xyz[1] = 0.0;
	for (int i=0 ; i<5 ; i++)
		if (pieces[i] == nullptr)
			piecesR[i] = nullptr;
		else
		{
			piecesR[i] = new BasicShapeRenderer(sIF, pieces[i]);
			// accumulate bounding box
			if (xyz[0] > xyz[1]) // not yet initialized
				pieces[i]->getMCBoundingBox(xyz);
			else
			{
				double thisxyz[6];
				pieces[i]->getMCBoundingBox(thisxyz);
				for (int j=0 ; j<3 ; j++)
				{
					if (thisxyz[2*j] < xyz[2*j])
						xyz[2*j] = thisxyz[2*j];
					if (thisxyz[2*j+1] > xyz[2*j+1])
						xyz[2*j+1] = thisxyz[2*j+1];
				}
			}
		}
}

Table::~Table()
{
	for (int i=0 ; i<5 ; i++)
	{
		if (pieces[i] != nullptr)
			delete pieces[i];
		if (piecesR[i] != nullptr)
			delete piecesR[i];
	}
}

void Table::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
		double legHeight, double legRadius,
		double tableWidth, double tableDepth, double tableThickness)
{
	// we assume the table is parallel to xy-plane, hence:
	cryph::AffVector uu(u[0], u[1], 0.0), ww(0,0,1);
	uu.normalize();
	cryph::AffVector vv = ww.cross(uu);
	pieces[0] = BasicShape::makeBlock(corner+legHeight*ww,
		uu, tableWidth, vv, tableDepth, ww, tableThickness);
	cryph::AffPoint bottom = corner + 2.0 * legRadius * (uu + vv);
	cryph::AffPoint top = bottom + legHeight * ww;
	pieces[1] = BasicShape::makeBoundedCylinder(bottom, top,
		legRadius, 20, 2, BasicShape::CAP_AT_NEITHER);
	bottom += (tableWidth - 4.0 * legRadius) * uu;
	top = bottom + legHeight * ww;
	pieces[2] = BasicShape::makeBoundedCylinder(bottom, top,
		legRadius, 20, 2, BasicShape::CAP_AT_NEITHER);
	bottom += (tableDepth - 4.0 * legRadius) * vv;
	top = bottom + legHeight * ww;
	pieces[3] = BasicShape::makeBoundedCylinder(bottom, top,
		legRadius, 20, 2, BasicShape::CAP_AT_NEITHER);
	bottom -= (tableWidth - 4.0 * legRadius) * uu;
	top = bottom + legHeight * ww;
	pieces[4] = BasicShape::makeBoundedCylinder(bottom, top,
		legRadius, 20, 2, BasicShape::CAP_AT_NEITHER);
}

void Table::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = xyz[i];
}

void Table::prepareForFace(void* caller, int faceIndex)
{
}

void Table::renderTable()
{
	establishMaterial(matl);
	for (currentlyDrawingPiece=0 ; currentlyDrawingPiece<5 ; currentlyDrawingPiece++)
		if (piecesR[currentlyDrawingPiece] != nullptr)
			piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
}

void Table::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	establishLightingEnvironment();
	establishView();

	renderTable();

	glUseProgram(pgm);
}
