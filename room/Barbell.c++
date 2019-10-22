// Barbell.c++

#include "Barbell.h"

Barbell::Barbell(ShaderIF* sIF, const PhongMaterial& matlIn,
		cryph::AffPoint P1, cryph::AffPoint P2,
		double radiusBar, double radiusBell) : SceneElement(sIF), matl(matlIn)
{
	defineInitialGeometry(P1, P2, radiusBar, radiusBell);
	xyz[0] = 1.0; xyz[1] = 0.0;
	for (int i=0 ; i<3 ; i++)
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

Barbell::~Barbell()
{
	for (int i=0 ; i<3 ; i++)
	{
		if (pieces[i] != nullptr)
			delete pieces[i];
		if (piecesR[i] != nullptr)
			delete piecesR[i];
	}
}

void Barbell::defineInitialGeometry(cryph::AffPoint P1, cryph::AffPoint P2,
		double radiusBar, double radiusBell)
{
	pieces[0] = BasicShape::makeBoundedCylinder(P1, P2,
		radiusBar, 20, 2, BasicShape::CAP_AT_NEITHER);
	pieces[1] = BasicShape::makeSphere(P1, radiusBell, 20, 20);
	pieces[2] = BasicShape::makeSphere(P2, radiusBell, 20, 20);
}

void Barbell::getMCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = xyz[i];
}

void Barbell::renderBarbell()
{
	establishMaterial(matl);
	for (int i=0 ; i<3 ; i++)
		if (piecesR[i] != nullptr)
			piecesR[i]->renderShape();
}

void Barbell::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	establishLightingEnvironment();
	establishView();

	renderBarbell();

	glUseProgram(pgm);
}
