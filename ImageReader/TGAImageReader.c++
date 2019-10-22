//  TGAImageReader.c++ -- subclass of ImageReader for reading image files in
//                        targa (.tga) format

// This software was developed by James R. Miller (jrmiller@ku.edu) and is
// OPEN SOURCE.

#include <iomanip>

#include "TGAImageReader.h"

// The copy constructor cannot be used.

TGAImageReader::TGAImageReader(const TGAImageReader& t) : ImageReader(t)
{
}

// Here are the supported constructors

TGAImageReader::TGAImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

#include <iostream>

#include <stdio.h>

using namespace std;

/* =============
getRGBA

Reads in RGBA data for a 32bit image.
============= */
void getRGBA (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size * 4, s);

    /* TGA is stored in BGRA, make it RGBA */
    for (int i = 0; i < size * 4; i += 4 )
    {
        unsigned char temp = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = temp;
    }
}
/* =============
getRGB

Reads in RGB data for a 24bit image.
============= */
void getRGB (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size * 3, s);

    /* TGA is stored in BGR, make it RGB */
    for (int i = 0; i < size * 3; i += 3)
    {
        unsigned char temp = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = temp;
    }
}
/* =============
getGray

Gets the grayscale image data.  Used as an alpha channel.
============= */
void getGray (FILE *s, int size, unsigned char* buf)
{
    int bread = fread (buf, sizeof (unsigned char), size, s);
}
/* =============
getData

Gets the image data for the specified bit depth.
============= */
void getData (FILE *s, int sz, int iBits, unsigned char* buf)
{
    if (iBits == 32)
        getRGBA (s, sz,  buf);
    else if (iBits == 24)
        getRGB (s, sz, buf);
    else if (iBits == 8)
        getGray (s, sz, buf);
}
/* =============
loadTGA

Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.
id is the texture ID to bind too.
============= */
bool TGAImageReader::read()
{
    FILE *fp = fopen(fullFileName.c_str(), "rb");
    if (fp == nullptr)
	{
		std::cerr << "TGAImageReader:: read - could not open: '" << fullFileName
		     << "'\n";
        return false;
	}

    unsigned char header[12];
	// Header (12 bytes)
	// byte   0: ignored
	// byte 1-2: color map information and image type
	// byte 3-12: ? (original implementation just fseek-ed over them...)
    fread (header, sizeof (char), 12, fp);
/*
	if (debug)
	{
		std::cout << "Header: ";
		for (int ii=0 ; ii<12 ; ii++)
			std::cout << setw(4) << (int)header[ii];
		std::cout << '\n';
	}
*/
    if (header[1] != 0 || (header[2] != 2 && header[2] != 3))
	{
		fclose(fp);
		return false;
	}

	const int nBytes = 6;
    unsigned char info[nBytes];
    fread (info, sizeof (char), nBytes, fp);
/*
	if (debug)
	{
		std::cout << "  info: ";
		for (int jj=0 ; jj<nBytes ; jj++)
			std::cout << setw(4) << static_cast<int>(info[jj]);
		std::cout << '\n';
	}
*/
    int nCols = info[0] + info[1] * 256;
    int nRows = info[2] + info[3] * 256;
    int imageBits =	info[4];
	int nChannels = imageBits/8;

    int size = nCols * nRows;

    /* make sure we are loading a supported type */
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
	{
		fclose(fp);
		return false;
	}

	theImage = new cryph::Packed3DArray<unsigned char>
								(nRows, nCols, nChannels);
	unsigned char* imageData = theImage->getModifiableData();
	getData (fp, size, imageBits, imageData);
    fclose (fp);

	if (info[5] != 0)
	{
		// rows are inverted
		int nRowsToSwap = nRows / 2;
		int m = nRows - 1;
		for (int i=0 ; i<nRowsToSwap ; i++)
		{
			// swap row 'i' with row 'm'
			for (int j=0 ; j<nCols ; j++)
				for (int k=0 ; k<nChannels ; k++)
				{
					unsigned char from_i = theImage->getDataElement(i,j,k);
					unsigned char from_m = theImage->getDataElement(m,j,k);
					theImage->setDataElement(i,j,k,from_m);
					theImage->setDataElement(m,j,k,from_i);
				}
			m--;
		}
	}

	return true;
}
