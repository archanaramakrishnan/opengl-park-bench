//  BMPImageReader.c++ -- subclass of ImageReader for reading image files in
//                        BMP format
// This software was developed by James R. Miller and is OPEN SOURCE.

#include "BMPLoader.h"
#include "BMPImageReader.h"

BMPImageReader::BMPImageReader(const BMPImageReader& b) : ImageReader(b)
{
}

// Here are the supported constructors

BMPImageReader::BMPImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

bool BMPImageReader::read()
{
	int			widthOut, heightOut, nChannelsOut;
	unsigned char*	pixels;

	LOAD_TEXTUREBMP_RESULT res = loadBMPData(fullFileName.c_str(),&pixels,
										widthOut, heightOut, nChannelsOut);
	if ( (pixels == nullptr) || (res != LOAD_TEXTUREBMP_SUCCESS) )
		return false;

	theImage = new cryph::Packed3DArray<unsigned char>(
							heightOut, widthOut, nChannelsOut);
	int nextPixel = 0;
	for (int i=0 ; i<heightOut ; i++)
		for (int j=0 ; j<widthOut ; j++)
			for (int k=0 ; k<nChannelsOut ; k++)
				theImage->setDataElement(i,j,k,pixels[nextPixel++]);
	delete [] pixels;

    return true;
}
