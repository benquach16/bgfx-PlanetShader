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
					   , 0x101010ff
					   , 1.0f
					   , 0);

	bool exit = false;
	SDL_Event event;



	//needed to pass information to shader
	bgfx::UniformHandle skybox = bgfx::createUniform("skybox", bgfx::UniformType::Int1);
	bgfx::UniformHandle cameraPosition = bgfx::createUniform("cameraPosition", bgfx::UniformType::Vec4);
	bgfx::UniformHandle lightPosition = bgfx::createUniform("lightPosition", bgfx::UniformType::Vec4);
	bgfx::UniformHandle resolution = bgfx::createUniform("resolution", bgfx::UniformType::Vec4);

	bgfx::setViewName(0, "skybox");
	bgfx::setViewName(1, "atmosphere");
	bgfx::setViewName(2, "planet");
	
	uint64_t state = 0
		| BGFX_STATE_RGB_WRITE
		| BGFX_STATE_ALPHA_WRITE
		| BGFX_STATE_DEPTH_WRITE
		| BGFX_STATE_BLEND_ALPHA
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA;
	bgfx::ProgramHandle planet_program = m_programloader.loadProgram("vs_planet", "fs_planet");
	bgfx::ProgramHandle atmo_program = m_programloader.loadProgram("vs_atmo", "fs_atmo");
	bgfx::ProgramHandle skybox_program = m_programloader.loadProgram("vs_skybox", "fs_skybox");
	Mesh *mesh = meshLoad("sphere.bin");
	Mesh *atmo = meshLoad("sphere.bin");	
	mesh->addTexture("mars_map.bmp",  0 | BGFX_TEXTURE_U_MIRROR
					  | BGFX_TEXTURE_V_MIRROR
					  | BGFX_TEXTURE_W_MIRROR);

	Skybox sky;
	sky.setupSkybox();
	float t = 1;

	int oldmouseX = 0;
	int oldmouseY = 0;
	int mouseX = 0;
	int mouseY = 0;
	float distance = -7.0f;
	while (!exit)
	{

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x4f, "Planet");
		bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

		
		float at[3]  = { 0.0f, 0.0f,  0.0f };
		float eye[3] = { 0.0f, 0.0f, distance };

		float view[16];

		float mouseMtx[16];
		bx::mtxRotateXY(mouseMtx, 0, t);
		t+=0.01f;
		float temp[4];
		bx::vec3MulMtx(temp, eye, mouseMtx);
		bx::mtxLookAt(view,temp, at);
		//todo : fix cam position
		float eyeUniform[4] = { temp[0], temp[1],temp[2], t};
		bgfx::setUniform(cameraPosition, eyeUniform);
		float resUniform[4] = { m_width, m_height, 0, 0};
		bgfx::setUniform(resolution, resUniform);

		
		float proj[16];
		bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, m_width, m_height);
		bgfx::setViewTransform(0, view, proj);
		bgfx::setViewRect(1, 0, 0, m_width, m_height);
		bgfx::setViewTransform(1,view,proj);
		bgfx::setViewRect(2, 0, 0, m_width, m_height);
		bgfx::setViewTransform(2,view,proj);
   		bgfx::touch(0);

		//transform for planet
		float mtx[16];

		bx::mtxScale(mtx, 3, 3, 3);


		sky.renderSkybox(skybox_program);
		//bx::mtxRotateXY(mtx, 0, t);
		float atmoMtx[16];
		bx::mtxScale(atmoMtx, 3.2, 3.2, 3.2);
		//transform for atmosphere
		atmo->submit(1, atmo_program, atmoMtx, state);
		//bgfx::setTexture(0, s_planet_texture, planet_texture_day);
		mesh->submit(2, planet_program, mtx, state);


		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
			{
				if(event.key.keysym.sym == SDLK_z)
				{
					distance -= 0.5;
				}
				else if(event.key.keysym.sym == SDLK_a)
				{
					distance += 0.5;
				}
				else
				{
					exit = true;
				}
				break;
				
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				std::cout << "mouse pressed!" << std::endl;
				SDL_GetMouseState(&mouseX, &mouseY);

				break;
			}
			case SDL_WINDOWEVENT:
			{
				//needed to resize the rendering window
				
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					m_width = event.window.data1;
					m_height = event.window.data2;
					uint32_t reset = BGFX_RESET_VSYNC;

					bgfx::reset(m_width, m_height, reset);
				}
				
			}
			}
		}


	}
	delete mesh;
	delete atmo;

	bgfx::destroyProgram(planet_program);
	bgfx::destroyProgram(atmo_program);
}
