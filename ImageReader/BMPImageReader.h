// BMPImageReader.h -- subclass of ImageReader for reading BMP image files
//
// This software was developed by James R. Miller (jrmiller@ku.edu) and is
// OPEN SOURCE.

#ifndef BMPIMAGEREADER_H
#define BMPIMAGEREADER_H

#include "ImageReader.h"

class BMPImageReader : public ImageReader
{
public:
	BMPImageReader(std::string fileName);

protected:
	BMPImageReader(const BMPImageReader& s);

	virtual bool read();
};

#endif
