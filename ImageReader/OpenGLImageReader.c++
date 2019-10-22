//  OpenGLImageReader.c++ -- Wrapper class for an ImageReader to facilitate
//                           its use in OpenGL programs.
// This software was developed by James R. Miller and is open source

#include <stdlib.h>
#include <string.h>

#include "OpenGLImageReader.h"

// The copy constructor cannot be used.

OpenGLImageReader::OpenGLImageReader(const OpenGLImageReader&)
{
	std::cerr << "***** ERROR: OpenGLImageReader copy constructor called!!\n";
}

// Here are the supported constructors

OpenGLImageReader::OpenGLImageReader(ImageReader* ir) :
	imageReader(ir), deleteImageReaderInDestructor(false), border(0), textureID(0)
{
}

OpenGLImageReader::~OpenGLImageReader()
{
	if ((deleteImageReaderInDestructor) && (imageReader != nullptr))
		delete imageReader;
}

// Methods....

OpenGLImageReader* OpenGLImageReader::create(std::string fileName) // CLASS METHOD
{
	ImageReader* ir = ImageReader::create(fileName);
	if (ir == nullptr)
		return nullptr;
	OpenGLImageReader* oglIR = new OpenGLImageReader(ir);
	// We allocated it here, so we delete it here:
	oglIR->deleteImageReaderInDestructor = true;
	return oglIR;
}

GLenum OpenGLImageReader::getFormat() const
{
	int nChannels = imageReader->getNumChannels();
	if (nChannels == 1)
		return GL_RED;
	if (nChannels == 3)
		return GL_RGB;
	if (nChannels == 4)
		return GL_RGBA;

	// probably an error:
	std::cout << "OpenGLImageReader::getFormat(), nChannels = " << nChannels << std::endl;
	return GL_RGB;
}

GLint OpenGLImageReader::getInternalFormat() const
{
	int nChannels = imageReader->getNumChannels();
	if (nChannels == 1)
		return GL_RED;
	if (nChannels == 3)
		return GL_RGB;
	if (nChannels == 4)
		return GL_RGBA;
	
	// probably an error:
	std::cout << "OpenGLImageReader::getInternalFormat(), nChannels = " << nChannels << std::endl;
	return GL_RGB;
}

const void* OpenGLImageReader::getTexture() const
{
	return imageReader->getInternalPacked3DArrayImage()->getData();
}

GLenum OpenGLImageReader::getType() const
{
	return GL_UNSIGNED_BYTE;
}

void OpenGLImageReader::setTextureID(GLuint tID)
{
	textureID = tID;
}
