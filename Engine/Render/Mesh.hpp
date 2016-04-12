#pragma once

#include "Assets/Asset.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/IndexBuffer.hpp"
#include "Render/MeshBuffer.hpp"
#include "Render/Material.hpp"

#include "Core/Map.hpp"

class CMesh: public CAsset
{
public:
	CMesh();
	~CMesh();

	bool Load(CStream* _stream);
	void Unload();

	void SetMaterial(CMaterial* _mat);

	const CBBox& GetBBox() const {return m_BBox;}

	void Render();

private:
	void _ReadSkelleton(CStream* _stream);

	CMap<u32,CVertexBuffer*> m_VertexBuffers;
	CMap<u32,CIndexBuffer*>  m_IndexBuffers;
	CMap<u32,CMaterial*>	 m_Materials;
	CMap<u32,CMeshBuffer*>   m_MeshBuffers;

	CBBox m_BBox;
};