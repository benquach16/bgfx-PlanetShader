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


int main()
{
	//do everything in a single thread for now
	//because multithreading is hard and setting up everything in the
	//bgfx examples is complicated
	int m_width = 1280;
	int m_height = 800;
	SDL_Init(0
			 | SDL_INIT_VIDEO
			 | SDL_INIT_GAMECONTROLLER
		);

	SDL_Window *window = SDL_CreateWindow("bgfx"
					 , SDL_WINDOWPOS_UNDEFINED
					 , SDL_WINDOWPOS_UNDEFINED
					 , m_width
					 , m_height
					 , SDL_WINDOW_SHOWN
					 | SDL_WINDOW_RESIZABLE
		);

	bgfx::sdlSetWindow(window);
	bgfx::renderFrame();

	bgfx::init();

	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;
	bgfx::setDebug(debug);
	bgfx::reset(m_width, m_height, reset);

	// Set view 0 clear state.
	bgfx::setViewClear(0
					   , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
					   , 0x3030ffff
					   , 1.0f
					   , 0
		);

	
	bool exit = false;
	SDL_Event event;
	while (!exit)
	{
				// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, m_width, m_height);

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
		bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();
		bgfx::renderFrame();
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
			{
				exit=true;
				break;
			}
			}
		}


	}

	bgfx::shutdown();
	return 0;
}
