// TGAImageReader.h -- subclass of ImageReader for reading TGA image files

// This software was developed by James R. Miller (jrmiller@ku.edu) and is
// OPEN SOURCE.

#ifndef TGAIMAGEREADER_H
#define TGAIMAGEREADER_H

#include "ImageReader.h"

class TGAImageReader : public ImageReader
{
public:
	TGAImageReader(std::string fileName);

protected:
	TGAImageReader(const TGAImageReader& s);

	virtual bool read();
};

#endif
