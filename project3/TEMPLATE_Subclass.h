// TEMPLATE_Subclass.h

#ifndef TEMPLATE_SUBCLASS_H
#define TEMPLATE_SUBCLASS_H

#include "SceneElement.h"
#include "ShaderIF.h"

class TEMPLATE_Subclass : public SceneElement
{
public:
	// Your subclasses will use one or more PhongMaterial instances.
	// This constructor illustrates one possible way to do this by
	// passing one (or more) PhongMaterial instances to the constructor.
	TEMPLATE_Subclass(ShaderIF* sIF, PhongMaterial& matlIn);
	virtual ~TEMPLATE_Subclass();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderXxx();
private:
	// IMPORTANT NOTE:
	// The ShaderIF will be stored with the SceneElement piece of this
	// object instance. Do not store one here. You only need to add
	// instance variables here that are unique to the new subclass
	// you are creating. For example, you may want one (or more)
	// PhongMaterial instances:
	PhongMaterial matl;
};

#endif
