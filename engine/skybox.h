#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <bgfx/bgfxdefines.h>

//create a box here
//lace camera inside it


class Skybox
{
public:
	void setupSkybox();
	void renderSkybox(bgfx::ProgramHandle program);


};


#endif
