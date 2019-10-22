//  OpenGLImageReader.h -- Utility class that uses an ImageReader and facilitates
//                         an interface for its use in OpenGL programs.
//
// Quick synopsis of use:
// 1. Create an ImageReader using ImageReader::create.
// 2. Create an OpenGLImageReader byt passing the ImageReader to the constructor.
// 3. The various query methods can be used to retrieve relevant parameters
//    for OpenGL function calls like glTexImage2D.

#ifndef OPENGLIMAGEREADER_H
#define OPENGLIMAGEREADER_H

#ifdef __APPLE__
#include "OpenGL/gl3.h"
#else
#include <GL/gl.h>
#endif

#include "ImageReader.h"

class OpenGLImageReader
{
public:
	OpenGLImageReader(ImageReader* ir);
	virtual ~OpenGLImageReader();

	// Methods to return appropriate values for various parameters to
	// OpenGL functions like glTexImage2D, glDrawPixels, etc.
	int		getBorder() const { return border; }
	GLenum	getFormat() const;
	int		getHeight() const { return imageReader->getHeight(); }
	GLint	getInternalFormat() const;
	int		getNumChannels() const { return imageReader->getNumChannels(); }
	GLenum	getType() const;
	int		getWidth() const { return imageReader->getWidth(); }

	// The following is a wrapper for the ImageReader get/set user data
	// mechanism which allows users to associate user data with ImageReader
	// instances. For example, you may want to store an OpenGL textureID here.
	// An alternate way to do this astrictly for OpenGLImageReader instances
	// is to use the getTextureID/setTextureID methods that immediately follow.
	// The choice depends on the application design. If OpenGLImageReader
	// instances are created on the fly from existing ImageReader instances,
	// then using this ImageReader mechanism will generally be preferred.
	// If instead instances are created and stored directly by the
	// application, then the getTextureID/setTextureID methods are probably
	// a better choice.
	unsigned int getUserData() const { return imageReader->getUserData(); }
	void setUserData(unsigned int ud) { imageReader->setUserData(ud); }

	// convenience interface; allows you to record a textureID with an instance
	GLuint getTextureID() const { return textureID; }
	void setTextureID(GLuint tID);

	// In the "getTexture" call, a pointer to the actual internal image
	// is returned. The caller should NEVER delete this pointer.
	const void* getTexture() const;

	// Actual number of logical channels per pixel; e.g., 3 ==> RGB; 4 ==> RGBA

	std::string getFileName() const { return imageReader->getFileName(); }

	// Alternate way to create an OpenGLImageReader from just a file name:
	static OpenGLImageReader* create(std::string fileName);

protected:
	OpenGLImageReader(const OpenGLImageReader& s); // cannot use the copy constructor

	ImageReader* imageReader;
	bool deleteImageReaderInDestructor;
	int	border; // always '0' in OpenGL 3.3 and beyond

private:
	GLuint textureID;
};

#endif
