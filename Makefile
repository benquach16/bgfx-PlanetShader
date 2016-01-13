#include dirs go here
#this should point to BGFX directory
#theoretically the only thing you need to change
BGFX_DIRECTORY=/home/ben/bgfx/
BX_DIRECTORY=/home/ben/bx/


INCLUDES=-I$(BGFX_DIRECTORY)include/ -I$(BX_DIRECTORY)include/ -I$(BGFX_DIRECTORY)3rdparty
LIBRARIES=-L$(BGFX_DIRECTORY).build/linux64_gcc/obj/x64/Release/bgfx/src
GCC=g++
SHADERC=$(BGFX_DIRECTORY)/tools/bin/linux/shaderc
GEOMETRYC=$(BGFX_DIRECTORY)/toosl/bin/linux/geometryc
USERCXXFLAGS= -g -std=c++14

#make sure we have XLIB and SDL2 packages first!!!
#if you do not please use your package manager to install them
LIBS= -lrt -ldl -lX11 -lGL -lGLU -lpthread -lSDL2 
#linking each file cause there are dx11 references
BGFX_ALL=$(BGFX_DIRECTORY).build/linux64_gcc/obj/x64/Release/bgfx/src/*.o
LDFLAGS += $(LIBRARIES)
LDFLAGS += -static 

all: baseapplication.o programloader.o mesh.o makeshaders indexbufferdecompression.o
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) programloader.o baseapplication.o mesh.o indexbufferdecompression.o main.cpp -o renderer

baseapplication.o: baseapplication.h baseapplication.cpp 
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) baseapplication.cpp -c

programloader.o: programloader.h programloader.cpp
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) programloader.cpp -c

indexbufferdecompression.o: engine/indexbufferdecompression.h engine/indexbufferdecompression.cpp
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) engine/indexbufferdecompression.cpp -c

mesh.o: engine/mesh.h engine/mesh.cpp
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) engine/mesh.cpp -c


makeshaders: makeshadervert makeshaderfrag

makeshadervert: vs_planet.glsl vs_atmo.glsl
	$(SHADERC) -f vs_planet.glsl -o shaders/glsl/vs_planet.bin --type vertex --platform linux -p 120 --varyingdef varying.def.sc --verbose
	$(SHADERC) -f vs_atmo.glsl -o shaders/glsl/vs_atmo.bin --type vertex --platform linux -p 120 --varyingdef varying.def.sc --verbose

makeshaderfrag: fs_planet.glsl fs_atmo.glsl
	$(SHADERC) -f fs_planet.glsl -o shaders/glsl/fs_planet.bin --type fragment --platform linux -p 120 --varyingdef varying.def.sc --verbose
	$(SHADERC) -f fs_atmo.glsl -o shaders/glsl/fs_atmo.bin --type fragment --platform linux -p 120 --varyingdef varying.def.sc --verbose 

clean:
	rm *.o renderer



