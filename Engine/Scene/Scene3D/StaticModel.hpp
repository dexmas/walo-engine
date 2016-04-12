#pragma once

#include "Scene/Scene3D/Object3D.hpp"
#include "Render/Mesh.hpp"

#include "Assets/AssetsDB.hpp"

class CStaticModel: public CObject3D
{
public:
	CStaticModel():CObject3D(ECT_STATICMODEL)
	{
		m_Mesh = 0;
	}
	
	~CStaticModel()
	{
	}

	void SetMesh(CMesh* _mesh)
	{
		m_Mesh = _mesh;
		
		if(m_Mesh)
		{
			m_LocalBound = m_Mesh->GetBBox();
			_UpdateBound();
		}
	}

	CMesh* GetMesh()
	{
		return m_Mesh;
	}

	virtual void Render()
	{
		m_Render->SetTransform(ETT_MODEL, m_Transform);

		if(m_Mesh)
			m_Mesh->Render();
	}

private:
	CMesh* m_Mesh;
};