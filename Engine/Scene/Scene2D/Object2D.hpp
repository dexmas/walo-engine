#pragma once

#include "Scene/Component.hpp"
#include "Render/Material.hpp"
#include "Core/Rect.hpp"
#include "Scene/Scene2D/Scene2D.hpp"
#include "Core/LinkedList.hpp"
#include "Assets/AssetsDB.hpp"

const u32 cVertex2DFormat = EEM_POSITION|EEM_COLOR|EEM_TEXCOORD1;

struct SVertex2D
{
	f32 X;
	f32 Y;
	f32 Z;
	u32 Color;
	f32 U;
	f32 V;
};

class CQuadtreeNode;

class CObject2D: public CComponentImpl<CScene2D>, public CLinkedListNode<CObject2D>
{
	friend class CScene2D;
	friend class CQuadtreeNode;
	friend class CBatch2D;

public:
	CObject2D(EComponentType _type = ECT_OBJECT2D);

	virtual ~CObject2D();

	void SetSize(const CVector2& _size) 
	{ 
		m_Size = _size; 
		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		if(m_ClipChild && m_Node)
		{
			CClipEvent cevent(m_ClipChild, m_Bound);
			m_Node->HandleEvent(&cevent);
		}

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	CVector2 GetSize() const 
	{
		return m_Size;
	}

	void SetCenter(const CVector2& _center)
	{
		m_Center = _center; 
		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		if(m_ClipChild && m_Node)
		{
			CClipEvent cevent(m_ClipChild, m_Bound);
			m_Node->HandleEvent(&cevent);
		}

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	CVector2 GetCenter() const 
	{
		return m_Center;
	}

	void SetColor(const CColor& _color)
	{
		m_Color = _color; 

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	CColor GetColor() const 
	{
		return m_Color;
	}

	void SetOrder(f32 _order);

	f32 GetOrder() const;

	void SetFlip(const CVector2& _flip) 
	{
		m_Flip = _flip;

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	CVector2 GetFlip() const 
	{
		return m_Flip;
	}

	CRect GetBound() const 
	{
		return m_Bound;
	}

	bool HitTest(const CVector2& _point);

	void SetClipChild(bool _clip)
	{
		if(_clip != m_ClipChild)
		{
			m_ClipChild = _clip;

			if(m_Node)
			{
				CClipEvent cevent(m_ClipChild, m_Bound);
				m_Node->HandleEvent(&cevent);
			}
		}
	}

	bool GetClipChild() const
	{
		return m_ClipChild;
	}

	virtual void OnTransformed()
	{
		m_Transform = m_Node->GetTransform();

		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		if(m_ClipChild && m_Node)
		{
			CClipEvent cevent(m_ClipChild, m_Bound);
			m_Node->HandleEvent(&cevent);
		}

		m_TransformDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	virtual void OnEvent(CEvent* _event)
	{
		if(_event->Type == EIT_CLIP)
		{
			if(m_ClipChild)
				return;

			CClipEvent* cevent = (CClipEvent*)_event;
			m_Clipped = cevent->m_Clip;
			m_ClipRect = cevent->m_ClipRect;
		}
	}

	virtual void OnAssigned(CNode* _node)
	{
		CComponent::OnAssigned(_node);

		if(m_Node)
		{
			m_Transform = m_Node->GetTransform();
			m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
			m_Transform.TransformAARect(m_Bound);

			if(m_ClipChild)
			{
				CClipEvent cevent(m_ClipChild, m_Bound);
				m_Node->HandleEvent(&cevent);
			}
		}
	}

#ifdef _DEBUG
	virtual void DebugRender()
	{
		CDebugRenderer::Instance()->AddRect(m_Bound, CColor::RED);
	}
#endif

	virtual void SetTexture(CTexture* _texture) 
	{ 
		if(m_Texture == _texture)
		{
			return;
		}

		if(m_HeirarhyEnabled)
		{
			m_System->RemoveComponent(this);
		}

		m_Texture = _texture; 

		if(m_Texture)
		{
			SetMaterial(CAssetsDB::Instance()->GetMaterial("render2d.mat"));
		}
		else
		{
			SetMaterial(CAssetsDB::Instance()->GetMaterial("render2d_vc.mat"));
		}

		if(m_HeirarhyEnabled)
		{
			m_System->AddComponent(this);
		}
	}

	virtual void SetMaterial(CMaterial* _material) 
	{
		if(m_Material == _material)
		{
			return;
		}

		if(m_HeirarhyEnabled)
		{
			m_System->RemoveComponent(this);
		}

		m_Material = _material; 

		if(m_HeirarhyEnabled)
		{
			m_System->AddComponent(this);
		}
	}

	CTexture* GetTexture() 
	{
		return m_Texture;
	}

	CMaterial* GetMaterial() 
	{
		return m_Material;
	}

	CPODArray<SVertex2D>& GetVertexes()
	{
		if(m_VertexesDirty)
		{
			_UpdateVertexes();

			for(u32 i = 0; i < m_Positions.Size(); i++)
			{
				CVector3 pos = m_Positions[i];
				m_Transform.TransformVector(pos);

				m_Vertexes[i].X = pos.X;
				m_Vertexes[i].Y = pos.Y;
				m_Vertexes[i].Z = pos.Z;
			}

			m_TransformDirty = false;
			m_VertexesDirty = false;
		}
		else
		if(m_TransformDirty)
		{
			for(u32 i = 0; i < m_Positions.Size(); i++)
			{
				CVector3 pos = m_Positions[i];
				m_Transform.TransformVector(pos);

				m_Vertexes[i].X = pos.X;
				m_Vertexes[i].Y = pos.Y;
				m_Vertexes[i].Z = pos.Z;
			}

			m_TransformDirty = false;
		}

		return m_Vertexes;
	}

protected:
	bool _HitTestPhysics(const CVector2& _point, bool& _processed);

	CRender*   m_Render;

	CRect	   m_Bound;
	CVector2   m_Size;
	CVector2   m_Center;
	CVector2   m_Flip;
	CColor	   m_Color;
	f32		   m_Order;
	bool	   m_ClipChild;
	bool	   m_Clipped;
	CRect	   m_ClipRect;

	CQuadtreeNode* m_QuadtreeNode;
	CBatch2D* m_CurrentBatch;

	CTexture*  m_Texture;
	CMaterial* m_Material;

	CMatrix4   m_Transform;
	bool	   m_TransformDirty;
	bool	   m_VertexesDirty;

	virtual void _UpdateVertexes() {}

	CPODArray<SVertex2D> m_Vertexes;
	CPODArray<CVector3>  m_Positions;
};