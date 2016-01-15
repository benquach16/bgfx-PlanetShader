#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>


#include "memory.h"


class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture();

protected:
	bgfx::TextureHandle m_texture;
	bgfx::UniformHandle m_uniform;
	int m_stage;
};

bgfx::TextureHandle loadTexture(const char* _name, uint32_t _flags, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL);



#endif
