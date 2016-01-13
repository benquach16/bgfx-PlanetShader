#include <iostream>
#include "baseapplication.h"




BaseApplication::BaseApplication() : m_mainWindow(0)
{
	m_width = 1280;
	m_height = 800;
}

BaseApplication::~BaseApplication()
{
	//destroy sdl stuff
	bgfx::shutdown();
	SDL_DestroyWindow(m_mainWindow);
	SDL_Quit();
}

void BaseApplication::run()
{

	//do everything in a single thread for now
	//because multithreading is hard and setting up everything in the
	//bgfx examples is complicated
	SDL_Init(0 | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	m_mainWindow = SDL_CreateWindow("rendertest"
									, SDL_WINDOWPOS_UNDEFINED
									, SDL_WINDOWPOS_UNDEFINED
									, m_width
									, m_height
									, SDL_WINDOW_SHOWN
									| SDL_WINDOW_RESIZABLE);

	bgfx::sdlSetWindow(m_mainWindow);
	bgfx::renderFrame();
	bgfx::init( bgfx::RendererType::OpenGL);

	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;
	bgfx::setDebug(debug);
	bgfx::reset(m_width, m_height, reset);

	// Set view 0 clear state.
	bgfx::setViewClear(0
					   , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
					   , 0x303030ff
					   , 1.0f
					   , 0);
	
	bool exit = false;
	SDL_Event event;

	bgfx::ProgramHandle planet_program = m_programloader.loadProgram("vs_planet", "fs_planet");
	bgfx::ProgramHandle atmo_program = m_programloader.loadProgram("vs_atmo", "fs_atmo");
	Mesh *mesh = meshLoad("sphere.bin");
	Mesh *atmo = meshLoad("sphere.bin");



	float t = 0;
	while (!exit)
	{
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, m_width, m_height);

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x4f, "test");
		bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

		
		float at[3]  = { 0.0f, 0.0f,  0.0f };
		float eye[3] = { 0.0f, 0.0f, -5.0f };

		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, m_width, m_height);


		bgfx::touch(0);

		//transform for planet
		float mtx[16];
		bx::mtxRotateXY(mtx, 0, t);
		bx::mtxScale(mtx, 1.6, 1.6, 1.6);
		t+=0.01f;

		float atmoMtx[16];
		//bx::mtxTranslate(atmoMtx, -1,0,0);
		bx::mtxScale(atmoMtx, 3, 3, 3);
		//transform for atmosphere
		uint64_t state = 0 
			| BGFX_STATE_RGB_WRITE 
			| BGFX_STATE_ALPHA_WRITE 
			| BGFX_STATE_DEPTH_TEST_LESS 
			| BGFX_STATE_DEPTH_WRITE 
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA;
		mesh->submit(0, atmo_program, atmoMtx, state);
		atmo->submit(0, planet_program, mtx, state);

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();
		
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
	delete mesh;
	delete atmo;
	bgfx::destroyProgram(planet_program);
	bgfx::destroyProgram(atmo_program);
}
