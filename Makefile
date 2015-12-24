#include dirs go here
#this should point to BGFX directory
BGFX_DIRECTORY=/home/ben/bgfx/
BX_DIRECTORY=/home/ben/bx/


INCLUDES=-I$(BGFX_DIRECTORY)include/ -I$(BX_DIRECTORY)include/ -I$(BGFX_DIRECTORY)3rdparty
LIBRARIES=-L$(BGFX_DIRECTORY).build/linux64_gcc/obj/x64/Release/bgfx/src
GCC=g++
SHADERC=$(BGFX_DIRECTORY)/tools/bin/linux/shaderc
USERCXXFLAGS= -g

#make sure we have XLIB and SDL2 packages first!!!
#if you do not please use your package manager to install them
LIBS= -lrt -ldl -lX11 -lGL -lpthread -lSDL2
#linking each file cause there are dx11 references
BGFX_ALL=$(BGFX_DIRECTORY).build/linux64_gcc/obj/x64/Release/bgfx/src/*.o
LDFLAGS += $(LIBRARIES)
LDFLAGS += -static 

all: baseapplication.o
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL)  baseapplication.o main.cpp -o renderer

baseapplication.o: baseapplication.h baseapplication.cpp
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) baseapplication.cpp -c

makeshaders: makeshadervert makeshaderfrag

makeshadervert: vs.sc
	$(SHADERC) -f vs.sc -o shaders/glsl/vs.bin --type v --platform linux --varyingdef varying.def.sc --verbose --bin2c

makeshaderfrag: fs.sc
	$(SHADERC) -f fs.sc -o shaders/glsl/fs.bin --type f --platform linux --varyingdef varying.def.sc --verbose --bin2c

clean:
	rm *.o renderer



