// SceneElement.c++

#include "SceneElement.h"
#include "OpenGLImageReader.h"

float SceneElement::lightPos[4*MAX_NUM_LIGHTS] =
	{
		0.25, 0.5, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
		-0.25, 0.5, 1.0, 0.0
	};

// Are coordinates in "lightPos" stored in MC or EC? (You usually want them
// defined in MC, unless you are trying to simulate something like the viewer
// holding a flashlight while moving around the scene.)
bool SceneElement::posInModelCoordinates[MAX_NUM_LIGHTS] =
	{ true, true, true };
// The following is the buffer actually sent to GLSL. It will contain a copy of
// the (x,y,z,w) for light sources defined in EC; it will contain the coordinates
// after transformation to EC if the position was originally specified in MC.
float posToGLSL[4*MAX_NUM_LIGHTS];

float SceneElement::lightStrength[3*MAX_NUM_LIGHTS] =
	{
		0.8, 0.8, 0.8,
		0.5, 0.5, 0.5,
		0.6, 0.6, 0.6
	};

float SceneElement::globalAmbient[] = { 0.2, 0.2, 0.2 };

SceneElement::SceneElement(ShaderIF* sIF) : shaderIF(sIF)
{
}

SceneElement::~SceneElement()
{
}

// NOTE: You may want to modify the interface to this method so that you
//       can pass parameters in case you want to establish lights differently
//       for different parts of the scene.
void SceneElement::establishLightingEnvironment( /* ... parameters? ... */ )
{
	// This should set all light source parameters, including:
	// "actualNumLights", "ecLightPosition", "lightStrength", "globalAmbient"
	// When setting "ecLightPosition", be sure to transform the coordinates
	// from MC to EC using the current mc_ec matrix if the position was specified
	// in MC.
}

void SceneElement::establishMaterial(const PhongMaterial& matl)
{
	// This should set all material property parameters, including:
	// "ka", "kd", "ks", "m", and (for project 4) "alpha"
}

// NOTE: You may want to modify the interface to this method so that you
//       can pass parameters to help you specify some of the parameters
//       mentioned in the comments that follow.
void SceneElement::establishTexture( /* ... parameters? ... */ )
{
	// Set texture-related parameters:
	// 1. OpenGL ones including glActiveTexture, glBindTexture, glTexParameter*
	// 2. Parameters communicated via per-primitive uniform variables defined in
	//    your shader program that allow you to control whether a texture is to
	//    to be used, and, if so, where the texture comes from (e.g., procedural,
	//    a sampler2D, etc.) and how to combine its color with with the color
	//    computed by your Phong local lighting model color.
	//
	// (If you are texture-mapping onto faces of BasicShape instances,
	// see that documentation for specific additional details.)
}

void SceneElement::establishView()
{
	// Line of sight, dynamic view controls, 3D-2D projection, & mapping to LDS:
	cryph::Matrix4x4 mc_ec, ec_lds;
	ModelView::getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));
}

GLuint SceneElement::readTextureImage(const std::string& imgFileName)
{
	OpenGLImageReader* oglIR = OpenGLImageReader::create(imgFileName.c_str());
	if (oglIR == nullptr)
	{
		std::cerr << "Could not open '" << imgFileName << "' for texture map.\n";
		return 0;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texID[1];
	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	float white[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint level = 0;
	int pw = oglIR->getWidth(), ph = oglIR->getHeight();
	GLint iFormat = oglIR->getInternalFormat();
	GLenum format = oglIR->getFormat();
	GLenum type = oglIR->getType();
	const GLint border = 0; // must be zero (only present for backwards compatibility)
	const void* pixelData = oglIR->getTexture();
	glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData);
	delete oglIR;
	return texID[0];
}
