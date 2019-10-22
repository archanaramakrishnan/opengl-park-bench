//  ImageReader.h -- Abstract Base Class.
//
//  Read an Image file and give access to its specifications and contents.
//  Currently the following image file types are supported:
//      * BMP format (.bmp)
//      * JPEG format (.jpg; .jpeg) (Assumes a system jpeg library)
//      * PNG format (.png) (Assumes a system png library)
//      * TARGA format (.tga)
//
//  Except as noted in the Acknowledgments below, this code was written by
//  James R. Miller (jrmiller@ku.edu) and is OPEN SOURCE.
//
// Quick synopsis of use:
// 1. Use ImageReader::create to open and read an image file
// 2. The various query methods can be used to retrieve relevant parameters
//    such as its width, height, number of channels, and pixel contents.
//
// Acknowledgments:
// Various public domain and/or open source image reading utilities written by
// other people are used in files associated with the ImageReader class.
// Attribution appears in those files obtained from other sources.

#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include "Packed3DArray.h"

class ImageReader
{
public:
	virtual ~ImageReader() {}

	// Retrieve the specifications and data:
	int		getHeight() const;
	int		getNumChannels() const;
	int		getWidth() const;

	std::string getFileName() const { return fullFileName; }

	// The following allows users to associate user data with this instance.
	// For example, you may want to store an OpenGL textureID here.
	unsigned int getUserData() const { return userData; }
	void setUserData(unsigned int ud) { userData = ud; }

	// public class methods
	// 1. create - dynamically allocates an ImageReader instance of the
	//             appropriate subtype if the type can be determined from the
	//             file name extension.
	//             It returns nullptr if either the file does not exist, or its
	//             type cannot be determined based on the file name extension.
	//             If a non-nullptr ImageReader pointer is returned, the caller is
	//             responsible for deleting it when it is done with it.
	static ImageReader*	create(std::string fileName);

	// 2. Miscellaneous
	static void	setEnsureAlphaChannel(bool b) { ensureAlphaChannel = b; }
	static void	setPromoteSingleChannelToGray(bool b)
		{ promoteSingleChannelToGray = b; }
	// Actual internal data. You can modify this, BUT (i) don't delete it; and
	// (ii) be careful if you modify.
	cryph::Packed3DArray<unsigned char>* getInternalPacked3DArrayImage() const
		{ return theImage; }

protected:
	// Since the class is abstract, you CANNOT use these constructors.
	// Use instead the class method "create" or create instances of
	// concrete subclasses (TGAImageReader, RGBImageReader, etc.)
	ImageReader(const std::string& fileName);
	ImageReader(const ImageReader& s); // cannot use the copy constructor

	virtual bool read() = 0;
	void	readImage();

	// The image read from the file
	cryph::Packed3DArray<unsigned char>*	theImage;

	std::string	fullFileName;
	bool	readFailed;

private:

	unsigned int userData;

	static ImageReader* guessFileType(const std::string& fileName);

	static bool	ensureAlphaChannel, promoteSingleChannelToGray;
};

#endif
