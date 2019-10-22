// TEMPLATE_Subclass.c++

#include "TEMPLATE_Subclass.h"

TEMPLATE_Subclass::TEMPLATE_Subclass(ShaderIF* sIF, PhongMaterial& matlIn) :
	SceneElement(sIF), matl(matlIn)
{
}

TEMPLATE_Subclass::~TEMPLATE_Subclass()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void TEMPLATE_Subclass::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0; // xmin  Give real values!
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -3000.0; // zmin         |
	xyzLimits[5] = -2000.0; // zmax        \_/
}

void TEMPLATE_Subclass::render()
{
	// 1. Save current and establish new current shader program
	// ...

	// 2. Establish "mc_ec" and "ec_lds" matrices
	//    call SceneElement::establishView(); to do this

	// 3. Establish Lighting environment
	//    complete the implementation of SceneElement::establishLightingEnvironment
	//    and then call it here.

	renderXxx();

	// 6. Reestablish previous shader program
	// ...
}

void TEMPLATE_Subclass::renderXxx()
{
	// 4. Establish material property parameters.
	//    complete the implementation of SceneElement::establishMaterial
	//    and then call it here.

	// 5. Establish any other attributes and make one or more calls to
	//    glDrawArrays and/or glDrawElements
	//    If all or part of this model involves texture mapping (project 4
	//    only), complete the implementation of SceneElement::establishTexture
	//    and call it from here as needed immediately before any glDrawArrays
	//    and/or glDrawElements calls to which texture is to be applied.
}
