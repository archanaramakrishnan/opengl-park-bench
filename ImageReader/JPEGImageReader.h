// JPEGImageReader.h -- subclass of ImageReader for reading jpeg image files

// This software was developed by James R. Miller (jrmiller@ku.edu) and is
// OPEN SOURCE.

#ifndef JPEGIMAGEREADER_H
#define JPEGIMAGEREADER_H

#include "ImageReader.h"

class JPEGImageReader : public ImageReader
{
public:
	JPEGImageReader(std::string fileName);

protected:
	JPEGImageReader(const JPEGImageReader& s);

	virtual bool read();
};

#endif
