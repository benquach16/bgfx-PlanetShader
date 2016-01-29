#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <bgfx/bgfxdefines.h>

#include <bx/handlealloc.h>
#include <bx/fpumath.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

//create a box here
//lace camera inside it


class Skybox
{
public:
	void setupSkybox();
	void renderSkybox(bgfx::ProgramHandle program);

protected:
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
};


#endif
