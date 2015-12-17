#ifndef __BASEAPPLICATION_H__
#define __BASEAPPLICATION_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfxplatform.h>


#include <stdio.h>
#include <bx/thread.h>
#include <bx/handlealloc.h>
#include <tinystl/allocator.h>
#include <tinystl/string.h>

#include <bgfx/bgfx.h>
#include <bx/uint32_t.h>

class BaseApplication
{
public:
	BaseApplication();
	~BaseApplication();

	void run();

protected:
	SDL_Window *m_mainWindow;
	int m_width;
	int m_height;
};


#endif
