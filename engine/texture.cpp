#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb/stb_image.c"
#include <iostream>

Texture::Texture()
{
}

Texture::Texture(int _stage) : m_stage(_stage)
{
}

Texture::~Texture()
{
	bgfx::destroyTexture(m_texture);
	bgfx::destroyUniform(m_uniform);
}

void Texture::loadTexture(const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info)
{
	m_uniform = bgfx::createUniform("tex", bgfx::UniformType::Int1);
	char filePath[512] = { '\0' };
	if (NULL == strchr(_name, '/') )
	{
		strcpy(filePath, "textures/");
	}

	strcat(filePath, _name);

	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
	bx::CrtAllocator allocator;
	bx::CrtFileReader _reader;
	uint32_t size = 0;
	void* data = loadMem(&_reader, &allocator, filePath, &size);
	if (NULL != data)
	{
		int width  = 0;
		int height = 0;
		int comp   = 0;

		uint8_t* img = NULL;
		
		img = stbi_load_from_memory( (uint8_t*)data, size, &width, &height, &comp, 4);
		BX_FREE(&allocator, data);

		if (NULL != img)
		{
			handle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), 1
										   , bgfx::TextureFormat::RGBA8
										   , _flags
										   , bgfx::copy(img, width*height*4)
				);

			free(img);

			if (NULL != _info)
			{
				bgfx::calcTextureSize(*_info
									  , uint16_t(width)
									  , uint16_t(height)
									  , 0
									  , false
									  , 1
									  , bgfx::TextureFormat::RGBA8
					);
			}
		}
	}
	else
	{
		std::cout << "FAILED" << std::endl;
	}

	m_texture = handle;
}

void Texture::setTexture() const
{
	bgfx::setTexture(m_stage, m_uniform, m_texture);
}



