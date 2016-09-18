#ifndef __PROGRAMHANDLER_H__
#define __PROGRAMHANDLER_H__

#include "programloader.h"
#include <vector>

//class to wrap shaders with their uniforms
class ProgramHandler
{
public:
	ProgramHandler(bgfx::ProgramHandle _handle);
	void addUniforms();
	
protected:
	bgfx::ProgramHandle m_handle;
	std::vector<bgfx::UniformHandle> m_uniforms;
};

#endif
