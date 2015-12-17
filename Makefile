#include dirs go here
INCLUDES=-I/home/ben/bgfx/include/ -I/home/ben/bx/include/ -I/home/ben/bgfx/3rdparty
LIBRARIES=-L/home/ben/bgfx/.build/linux64_gcc/obj/x64/Release/bgfx/src
GCC=g++
USERCXXFLAGS= 

#make sure we have XLIB and SDL2 packages first!!!
#if you do not please use your package manager to install them
LIBS= -lrt -ldl -lX11 -lGL -lpthread -lSDL2
BGFX_DIRECTORY=/home/ben/bgfx/.build/linux64_gcc/obj/x64/Release/bgfx/src/
#linking each file cause there are dx11 references
BGFX_ALL=$(BGFX_DIRECTORY)*.o
LDFLAGS += $(LIBRARIES)
LDFLAGS += -static 

all: baseapplication.o
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL)  baseapplication.o main.cpp -o renderer


baseapplication.o: baseapplication.h baseapplication.cpp
	$(GCC) $(USERCXXFLAGS) $(LIBS) $(INCLUDES) $(BGFX_ALL) baseapplication.cpp -c  
