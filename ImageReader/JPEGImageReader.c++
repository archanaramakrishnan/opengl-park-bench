//  JPEGImageReader.c++ -- subclass of ImageReader for reading jpeg image files

// This software was developed by James R. Miller and is OPEN SOURCE.

#include <stdio.h>

#include "jpeglib.h"

#include "JPEGImageReader.h"

using namespace std;

// The copy constructor cannot be used.

JPEGImageReader::JPEGImageReader(const JPEGImageReader& j) : ImageReader(j)
{
}

// Here are the supported constructors

JPEGImageReader::JPEGImageReader(std::string fileName) :
	ImageReader(fileName)
{
	readImage();
}

bool JPEGImageReader::read()
{
    FILE *fp = fopen(fullFileName.c_str(), "rb");
    if (fp == nullptr)
	{
		cerr << "JPEGImageReader:: read - could not open: '" << fullFileName
		     << "'\n";
        return false;
	}

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, fp);
#ifdef __APPLE__
	jpeg_read_header(&cinfo, TRUE);
#else
	jpeg_read_header(&cinfo, true);
#endif
	jpeg_start_decompress(&cinfo);
/*
	if (debug)
	{
		cout << "Reading: " << fullFileName << "; cinfo fields:\n";
		cout << "(image_width,image_height) = (" << cinfo.image_width << ','
		     << cinfo.image_height
		     << "), num_components = " << cinfo.num_components
		     << ", jpeg_color_space: " << cinfo.jpeg_color_space
		     << "\n\nout_color_space: " << cinfo.out_color_space
		     << ", scale_num = " << cinfo.scale_num
		     << ", scale_denom = " << cinfo.scale_denom
		     << "\n\n(output_width,output_height) = (" << cinfo.output_width
		     << ',' << cinfo.output_height
		     << ")\nout_color_components = "
		     << cinfo.out_color_components
		     << "; output_components = " << cinfo.output_components
		     << "\n\nrec_outbuf_height = "
		     << cinfo.rec_outbuf_height
		     << '\n';
	}
*/
	// storage for OpenGL image
	theImage = new cryph::Packed3DArray<unsigned char>
				(cinfo.image_height, cinfo.image_width, cinfo.out_color_components);

	// JDIMENSION is unsigned int
	// JSAMPLE is short

	// JSAMPROW   is JSAMPLE*    { one image row of pixel samples }
	// JSAMPARRAY is JSAMPROW*   { array of sample rows }
	// JSAMPIMAGE is JSAMPARRAY* { 3D array: [row][col][color] }

	// JSAMPLE - one channel (r, g, b; or gray)
	// JSAMPLE* (JSAMPROW) - one scan line
	// JSAMPLE** (JSAMPARRAY or JSAMPROW*) -- one channel of an image

	// Main read routine is:
	// JDIMENSION jpeg_read_scanlines (j_decompress_ptr cinfo,
    //                 JSAMPARRAY scanlines, JDIMENSION max_lines);

	JSAMPARRAY scanlines = new JSAMPROW[cinfo.rec_outbuf_height];
	int nCols = theImage->getDim2();
	int nChannels = theImage->getDim3();
	for (int i=0 ; i<cinfo.rec_outbuf_height ; i++)
		scanlines[i] = new JSAMPLE[nCols*nChannels];

	JDIMENSION totalLinesRead = 0;
	int setRow = theImage->getDim1() - 1;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		JDIMENSION res = jpeg_read_scanlines(&cinfo,scanlines,
			cinfo.rec_outbuf_height);
		for (int ii=0 ; ii<res ; ii++)
		{
			for (int jj=0 ; jj<nChannels*nCols ; jj+=nChannels)
			{
				int setCol = jj / nChannels;
				for (int kk=0 ; kk<nChannels ; kk++)
					theImage->setDataElement(setRow,setCol,kk,
							scanlines[ii][jj+kk]);
			}
			setRow--;
		}
		totalLinesRead += res;
	}

	jpeg_finish_decompress(&cinfo);
	fclose(fp);
	jpeg_destroy_decompress(&cinfo);

	for (int i=0 ; i<cinfo.rec_outbuf_height ; i++)
		delete [] scanlines[i];
	delete [] scanlines;

	return true;
}
