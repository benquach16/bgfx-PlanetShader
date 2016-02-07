#ifndef __MESH_H__
#define __MESH_H__

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <bgfx/bgfxdefines.h>
#include <bx/commandline.h>
#include <bx/fpumath.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <vector>
#include <string>
#include "texture.h"
#include "../libraries/ib-compress/readbitstream.h"
#include "../libraries/ib-compress/indexbufferdecompression.h"

struct Aabb
{
	float m_min[3];
	float m_max[3];
};

struct Obb
{
	float m_mtx[16];
};

struct Sphere
{
	float m_center[3];
	float m_radius;
};

struct Primitive
{
	uint32_t m_startIndex;
	uint32_t m_numIndices;
	uint32_t m_startVertex;
	uint32_t m_numVertices;

	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
};



typedef std::vector<Primitive> PrimitiveArray;

struct Group
{
	Group()
		{
			reset();
		}

	void reset()
		{
			m_vbh.idx = bgfx::invalidHandle;
			m_ibh.idx = bgfx::invalidHandle;
			m_prims.clear();
		}

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
	PrimitiveArray m_prims;
};


class Mesh
{
public:
	~Mesh();
	void load(bx::ReaderSeekerI* _reader);
	void submit(uint8_t _id, bgfx::ProgramHandle _program, const float* _mtx, uint64_t _state = BGFX_STATE_MASK);
	void addTexture(const char* _name, uint32_t _flags);
	void setTexture(int _index, const char* _name, uint32_t _flags);
	//todo: move vector3d into here
	//do all matrix multiplcation within classes
protected:
	bgfx::VertexDecl m_decl;
	typedef std::vector<Group> GroupArray;
	GroupArray m_groups;
	//use this eventually for OOP
	float m_mtx[16];
	std::vector<Texture*> m_textures;

};

Mesh* meshLoad(bx::ReaderSeekerI* _reader);

Mesh* meshLoad(const char* _filePath);


#endif
