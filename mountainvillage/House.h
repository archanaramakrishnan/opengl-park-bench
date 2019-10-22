// House.h - concrete subclass of ModelView that represents a House

#ifndef HOUSE_H
#define HOUSE_H

#include "ShaderIF.h"

#include <string>

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"

class CFont;
class CGLString;

class House : public ModelView
{
public:
	// A house is defined with center of base at (xb,yb). The main part of the
	// house has dimensions width x height. The roof is a triangle with (base,
	// height) = (roofWidth, roofHeight).
	// REQUIREMENT: roofWidth >= width.
	House(ShaderIF* sIF, double xbIn, double ybIn, double widthIn, double heightIn,
		double roofWidthIn, double roofHeightIn, const std::string& label);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderHouse() const;

	// House labels use the current font
	static void setHouseLabelFont(std::string font);
private:
	ShaderIF* shaderIF;
	// structures to convey geometry to OpenGL/GLSL:
	GLuint vao[1];
	GLuint vbo[1]; // Stores points for roof, house, and door
	// original defining data
	double xb, yb, width, height, roofWidth, roofHeight;
	// house label information
	CGLString* houseLabel;

	// House labels use the current font:
	static CFont* houseLabelFont;

	void buildHouseLabel(const std::string& label);
	void defineModel();
	void renderHouseLabel() const;
	void validateData();
};

#endif
