CPP = g++ -std=c++11
INC = -I../cryphutil
C_FLAGS = -fPIC -c $(INC)

LINK = g++ -fPIC

OBJS = ImageReader.o BMPImageReader.o BMPLoader.o JPEGImageReader.o TGAImageReader.o PNGImageReader.o OpenGLImageReader.o

libImageReader.so: $(OBJS)
	$(LINK) -shared -o libImageReader.so $(OBJS) -ljpeg -lpng
	cp libImageReader.so ../lib/

ImageReader.o: ImageReader.h ImageReader.c++
	$(CPP) $(C_FLAGS) ImageReader.c++

BMPImageReader.o: BMPImageReader.h BMPImageReader.c++
	$(CPP) $(C_FLAGS) BMPImageReader.c++

BMPLoader.o: BMPLoader.h BMPLoader.c++
	$(CPP) $(C_FLAGS) BMPLoader.c++

JPEGImageReader.o: JPEGImageReader.h JPEGImageReader.c++
	$(CPP) $(C_FLAGS) JPEGImageReader.c++

PNGImageReader.o: PNGImageReader.h PNGImageReader.c++
	$(CPP) $(C_FLAGS) PNGImageReader.c++

TGAImageReader.o: TGAImageReader.h TGAImageReader.c++
	$(CPP) $(C_FLAGS) TGAImageReader.c++

OpenGLImageReader.o: OpenGLImageReader.h OpenGLImageReader.c++
	$(CPP) $(C_FLAGS) OpenGLImageReader.c++
