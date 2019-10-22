//  ImageReader.c++ -- read an Image file and allow it to be queried.
// This software was developed by James R. Miller and is open source
//
// Acknowledgments:
// Various public domain and/or open source image reading utilities written by
// other people are used in files associated with the ImageReader class.
// Appropriate attribution appears in those files obtained from other sources.

#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "ImageReader.h"

// Known subclasses (needed by factory method "create"):
#include "BMPImageReader.h"
#include "JPEGImageReader.h"
#include "PNGImageReader.h"
#include "TGAImageReader.h"

// class variables

bool ImageReader::ensureAlphaChannel = false;
bool ImageReader::promoteSingleChannelToGray = true;

// The copy constructor cannot be used.

ImageReader::ImageReader(const ImageReader&)
{
	std::cerr << "***** ERROR: ImageReader copy constructor called!!\n";
}

// Here are the supported constructors

ImageReader::ImageReader(const std::string& fileName) :
	theImage(nullptr), fullFileName(fileName), readFailed(false),
	userData(0)
{
}

// Methods....

ImageReader* ImageReader::create(std::string fileName) // CLASS METHOD
{
	std::ifstream seeIfExists(fileName.c_str());
	if (seeIfExists.good())
		seeIfExists.close();
	else
	{
		std::cerr << "ImageReader::create could not open " << fileName
		          << " for reading.\n";
		return nullptr;
	}
	ImageReader* p = guessFileType(fileName);
	if (p == nullptr)
		return nullptr;

	if (p->readFailed)
	{
		delete p;
		return nullptr;
	}

	if ((p->theImage->getDim3() == 1) && ImageReader::promoteSingleChannelToGray)
	{
		int nRows = p->theImage->getDim1();
		int nCols = p->theImage->getDim2();
		cryph::Packed3DArray<unsigned char>* grayImage =
			new cryph::Packed3DArray<unsigned char>(nRows, nCols, 3);
		for (int i=0 ; i<nRows ; i++)
			for (int j=0 ; j<nCols ; j++)
			{
				unsigned char b = p->theImage->getDataElement(i,j,0);
				for (int k=0 ; k<3 ; k++)
					grayImage->setDataElement(i,j,k,b);
			}
		delete p->theImage;
		p->theImage = grayImage;
	}
	if ((p->theImage->getDim3() == 3) && ImageReader::ensureAlphaChannel)
	{
		int nRows = p->theImage->getDim1();
		int nCols = p->theImage->getDim2();
		cryph::Packed3DArray<unsigned char>* imageWithAlpha =
			new cryph::Packed3DArray<unsigned char>(nRows, nCols, 4);
		for (int i=0 ; i<nRows ; i++)
			for (int j=0 ; j<nCols ; j++)
			{
				for (int k=0 ; k<3 ; k++)
				{
					unsigned char b = p->theImage->getDataElement(i,j,k);
					imageWithAlpha->setDataElement(i,j,k,b);
				}
				imageWithAlpha->setDataElement(i,j,3,255);
			}
		delete p->theImage;
		p->theImage = imageWithAlpha;
	}
	return p;
}

int ImageReader::getNumChannels() const
{
	return theImage->getDim3();
}

int ImageReader::getHeight() const
{
	return theImage->getDim1();
}

int ImageReader::getWidth() const
{
	return theImage->getDim2();
}

ImageReader* ImageReader::guessFileType(const std::string& fileName) // CLASS METHOD
{
	int dotLoc = fileName.find_last_of('.');
	if (dotLoc != std::string::npos)
	{
		std::string extension = fileName.substr(dotLoc+1);
		if ((extension.compare("bmp") == 0) || (extension.compare("BMP") == 0))
			return new BMPImageReader(fileName);
		if ((extension.compare("jpg") == 0) || (extension.compare("JPG") == 0))
			return new JPEGImageReader(fileName);
		if ((extension.compare("jpeg") == 0) || (extension.compare("JPEG") == 0))
			return new JPEGImageReader(fileName);
		if ((extension.compare("png") == 0) || (extension.compare("PNG") == 0))
			return new PNGImageReader(fileName);
		if ((extension.compare("tga") == 0) || (extension.compare("TGA") == 0))
			return new TGAImageReader(fileName);
	}

	std::cerr << "ImageReader::guessFileType cannot determine file type of: "
		      << fileName << std::endl;
	return nullptr;
}

void ImageReader::readImage()
{
	readFailed = !read();
}
