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
