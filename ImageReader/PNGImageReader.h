// PNGImageReader.h -- subclass of ImageReader for reading png image files

// This software was developed by James R. Miller (jrmiller@ku.edu) and is
// OPEN SOURCE.

#ifndef PNGIMAGEREADER_H
#define PNGIMAGEREADER_H

#include "ImageReader.h"

class PNGImageReader : public ImageReader
{
public:
	PNGImageReader(std::string fileName);

protected:
	PNGImageReader(const PNGImageReader& s);

	virtual bool read();
private:
	int numChannelsFromColorType(int cType);
};

#endif
