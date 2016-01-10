#include <iostream>
#include "baseapplication.h"



struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;

	uint32_t m_abgr;

	float n_x;
	float n_y;
	float n_z;

	static void init()
		{
			ms_decl
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
				.add(bgfx::Attrib::Normal,   3, bgfx::AttribType::Float)
				.end();
		};

	static bgfx::VertexDecl ms_decl;
};
bgfx::VertexDecl PosColorVertex::ms_decl;
static PosColorVertex s_cubeVertices[8] =
{
	{-1.0f,  1.0f,  1.0f, 0xff0000ff, -0.33f, 0.33f, 0.33f},
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff, 0.33f, 0.33f, 0.33f},
	{-1.0f, -1.0f,  1.0f, 0xff00ff00, -0.33f, -0.33f, 0.33f},
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff, 0.33f, -0.33f, 0.33f},
	{-1.0f,  1.0f, -1.0f, 0xffff0000, -0.33f, 0.33f, -0.33f},
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff, 0.33f, 0.33f, -0.33f},
	{-1.0f, -1.0f, -1.0f, 0xffffff00, -0.33f, -0.33f, -0.33f},
	{ 1.0f, -1.0f, -1.0f, 0xffffffff, 0.33f, -0.33f, -0.33f},
};

static const uint16_t s_cubeIndices[36] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};


static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
{
	if (0 == bx::open(_reader, _filePath) )
	{
		uint32_t size = (uint32_t)bx::getSize(_reader);
		const bgfx::Memory* mem = bgfx::alloc(size+1);
		bx::read(_reader, mem->data, size);
		bx::close(_reader);
		mem->data[mem->size-1] = '\0';
		return mem;
	}

	return NULL;
}

static bgfx::ShaderHandle loadShader(bx::FileReaderI* _reader, const char* _name)
{
	char filePath[512];

	const char* shaderPath = "shaders/dx9/";

	switch (bgfx::getRendererType() )
	{
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		shaderPath = "shaders/dx11/";
		break;

	case bgfx::RendererType::OpenGL:
		shaderPath = "shaders/glsl/";
		break;

	case bgfx::RendererType::Metal:
		shaderPath = "shaders/metal/";
		break;

	case bgfx::RendererType::OpenGLES:
		shaderPath = "shaders/gles/";
		break;

	default:
		break;
	}

	strcpy(filePath, shaderPath);
	strcat(filePath, _name);
	strcat(filePath, ".bin");

	return bgfx::createShader(loadMem(_reader, filePath) );
}

bgfx::ProgramHandle BaseApplication::loadProgram(const char* _vsName, const char* _fsName)
{
	
	bgfx::ShaderHandle vsh = loadShader(m_reader, _vsName);
	bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
	if (NULL != _fsName)
	{
		fsh = loadShader(m_reader, _fsName);
	}

	return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}


BaseApplication::BaseApplication() : m_mainWindow(0)
{
	m_reader = new bx::CrtFileReader;
	m_width = 1280;
	m_height = 800;
}

BaseApplication::~BaseApplication()
{
	//destroy sdl stuff
	bgfx::shutdown();
	SDL_DestroyWindow(m_mainWindow);
	SDL_Quit();
	delete m_reader;
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


	bgfx::ProgramHandle program = loadProgram("vs", "fs");

	Mesh *mesh = meshLoad("sphere.bin");
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
		float eye[3] = { 0.0f, 0.0f, -4.0f };

		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, m_width, m_height);


		bgfx::touch(0);
		float mtx[16];
		bx::mtxRotateXY(mtx, 0, t);
		t+=0.01f;

		meshSubmit(mesh, 0, program, mtx);

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
	bgfx::destroyProgram(program);

}
