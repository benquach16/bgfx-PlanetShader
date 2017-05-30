#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <stdio.h>
#include <bgfx/platform.h>

#include <bgfx/bgfx.h>
#include <bx/handlealloc.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <bx/uint32_t.h>

//memory load functions
const bgfx::Memory* loadMem(bx::FileReaderI* reader, const char* filePath);
void* loadMem(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);

#endif
