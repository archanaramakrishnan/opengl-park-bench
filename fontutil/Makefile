CPP = g++ -std=c++11
C_FLAGS = -fPIC -O -c -DGL_GLEXT_PROTOTYPES -I../cryphutil

LINK = g++ -fPIC

OBJS = CFont.o CGLString.o

../lib/libfont.so: $(OBJS)
	$(LINK) -shared -o libfont.so $(OBJS)
	cp libfont.so ../lib/

CFont.o: CFont.h CFont.c++
	$(CPP) $(C_FLAGS) CFont.c++

CGLString.o: CGLString.h CGLString.c++
	$(CPP) $(C_FLAGS) CGLString.c++
