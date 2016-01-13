#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include "memory.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb/stb_image.c"

bgfx::TextureHandle loadTexture(const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info);



#endif
