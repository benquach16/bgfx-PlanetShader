#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <bgfx/bgfxdefines.h>

#include <bx/handlealloc.h>
#include <bx/fpumath.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include "texture.h"

//create a box here
//lace camera inside it


class Skybox
{
public:
	
	~Skybox();
	void setupSkybox();
	void renderSkybox(bgfx::ProgramHandle program);
	void setTexture(
		const char* north_tex,
		const char* south_tex,
		const char* west_tex,
		const char* east_tex,
		const char* top_tex,
		const char* bottom_tex);
protected:
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	Texture m_texture;
};


#endif
