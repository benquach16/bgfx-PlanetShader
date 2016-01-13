#include "texture.h"



bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader, const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info)
{
	char filePath[512] = { '\0' };
	if (NULL == strchr(_name, '/') )
	{
		strcpy(filePath, "textures/");
	}

	strcat(filePath, _name);

	if (NULL != bx::stristr(_name, ".dds")
		||  NULL != bx::stristr(_name, ".pvr")
		||  NULL != bx::stristr(_name, ".ktx") )
	{
		const bgfx::Memory* mem = loadMem(_reader, filePath);
		if (NULL != mem)
		{
			return bgfx::createTexture(mem, _flags, _skip, _info);
		}

		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
		return handle;
	}

	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
	bx::AllocatorI* allocator = new bx::CrtAllocator;

	uint32_t size = 0;
	void* data = loadMem(_reader, allocator, filePath, &size);
	if (NULL != data)
	{
		int width  = 0;
		int height = 0;
		int comp   = 0;

		uint8_t* img = NULL;
		
		img = stbi_load_from_memory( (uint8_t*)data, size, &width, &height, &comp, 4);

		BX_FREE(allocator, data);

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
		
	}

	return handle;
}

bgfx::TextureHandle loadTexture(const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info)
{
	bx::CrtFileReader reader;
	return loadTexture(&reader, _name, _flags, _skip, _info);
}
