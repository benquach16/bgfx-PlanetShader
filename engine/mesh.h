#ifndef __MESH_H__
#define __MESH_H__

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <bx/commandline.h>
#include <bx/fpumath.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <vector>
#include <string>
#include "readbitstream.h"
#include "indexbufferdecompression.h"



struct Mesh;



Mesh* meshLoad(bx::ReaderSeekerI* _reader);

Mesh* meshLoad(const char* _filePath);

void meshSubmit(Mesh* _mesh, uint8_t _id, bgfx::ProgramHandle _program, const float* _mtx, uint64_t _state = BGFX_STATE_MASK);


#endif
