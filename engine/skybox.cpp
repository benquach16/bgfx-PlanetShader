#include "skybox.h"


struct PosColorTexCoord0Vertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_rgba;
	float m_u;
	float m_v;

	static void init()
		{
			ms_decl
				.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}

	static bgfx::VertexDecl ms_decl;
};



bgfx::VertexDecl PosColorTexCoord0Vertex::ms_decl;


static PosColorTexCoord0Vertex s_cubeVertices[8] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeIndices[36] =
{
	2, 1, 0, // 0
	2, 3, 1,
	5, 6, 4, // 2
	7, 6, 5,
	4, 2, 0, // 4
	6, 2, 4,
	3, 5, 1, // 6
	3, 7, 5,
	1, 4, 0, // 8
	1, 5, 4,
	6, 3, 2, // 10
	7, 3, 6,
};

void Skybox::setupSkybox()
{
	PosColorTexCoord0Vertex::init();
	vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
		, PosColorTexCoord0Vertex::ms_decl
		);

	ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices) )
		);


}


void Skybox::renderSkybox(bgfx::ProgramHandle program)
{

	//render skybox code here
	bgfx::touch(0);
	float mtx[16];
	bx::mtxScale(mtx, 10, 10, 10);
	bgfx::setTransform(mtx);
	bgfx::setVertexBuffer(vbh);
	bgfx::setIndexBuffer(ibh);
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(0, program);
}
