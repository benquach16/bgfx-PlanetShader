#ifndef __PROGRAMHANDLER_H__
#define __PROGRAMHANDLER_H__

#include "programloader.h"
#include <vector>
#include <string>

//class to wrap shaders with their uniforms
class ProgramHandler
{
public:
	ProgramHandler(bgfx::ProgramHandle _handle);
	template <typename T>;
	void addUniform(std::string _name, T _type);
	void setUniform();
	
protected:
	bgfx::ProgramHandle m_handle;
	std::vector<bgfx::UniformHandle> m_uniforms;
};

#endif
