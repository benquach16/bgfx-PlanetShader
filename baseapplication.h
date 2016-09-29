#ifndef __BASEAPPLICATION_H__
#define __BASEAPPLICATION_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfxplatform.h>
#include <bgfx/bgfxdefines.h>

#include <stdio.h>
#include <bx/handlealloc.h>
#include <bx/fpumath.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include <GL/gl.h>

#include <bgfx/bgfx.h>
#include <bx/uint32_t.h>

#include "engine/mesh.h"
#include "engine/texture.h"
#include "engine/skybox.h"
#include "engine/programloader.h"

class BaseApplication
{
public:
	bgfx::ProgramHandle loadProgram(const char* _vsName, const char* _fsName);
	BaseApplication();
	~BaseApplication();
	void setupWindow();
	void setupViews();
	void run();

protected:
	SDL_Window *m_mainWindow;
	int m_width;
	int m_height;
	float m_distance;
	float m_rotate;
	ProgramLoader m_programloader;
	
	bgfx::UniformHandle cameraPosition;
};


#endif
