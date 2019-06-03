#include "Scene/Node.hpp"
#include "Scene/Component.hpp"

#include "Scene/Scene3D/StaticModel.hpp"
#include "Scene/Scene3D/Camera3D.hpp"
#include "Scene/Scene2D/Object2D.hpp"

CNode::CNode()
{
	m_Base = 0;

	m_Enabled = true;
	m_HeirarhyEnabled = false;

	m_Position = CVector3(0.0, 0.0, 0.0);
	m_Scale = CVector3(1.0, 1.0, 1.0);
	m_Rotation = CVector3(0.0, 0.0, 0.0);

	static u32 sNodeID = 0;

	m_Name = "Node#";
	m_Name += sNodeID++;
}

CNode::~CNode()
{
	OnEnabled(false); 
	
	if (m_Base)
	{
		m_Base->m_Childs.Erase(this);
	}

	CList<CComponent*>::CIterator it = m_Components.Begin();
	while(it != m_Components.End())
	{
		CComponent* cmp = *it;
		it = m_Components.Erase(cmp);

		RemoveComponent(cmp);

		delete cmp;
	}

	CList<CNode*>::CIterator nit = m_Childs.Begin();
	while(nit != m_Childs.End())
	{
		(*nit)->m_Base = 0;
		++nit;
	}
}

void CNode::AddChild(CNode* _child)
{
	if(_child->m_Base)
	{
		_child->m_Base->m_Childs.Erase(_child);
	}

	_child->m_Base = this;
	_child->OnTransformed();

	bool henbl = _child->m_Enabled && m_HeirarhyEnabled;
	if (henbl != _child->m_HeirarhyEnabled)
	{
		_child->m_HeirarhyEnabled = henbl;
		_child->OnEnabled(henbl);
	}

	m_Childs.PushBack(_child);
}

void CNode::RemoveChild(CNode* _child)
{
	if(_child->m_HeirarhyEnabled)
	{
		_child->m_HeirarhyEnabled = false;
		_child->OnEnabled(false);
	}

	_child->m_Base = 0;

	m_Childs.Erase(_child);
}

void CNode::AddComponent(CComponent* _comp)
{
	_comp->OnAssigned(this);

	m_Components.PushBack(_comp);

	bool henbl = _comp->m_Enabled && m_HeirarhyEnabled;
	if (henbl != _comp->m_HeirarhyEnabled)
	{
		_comp->m_HeirarhyEnabled = henbl;
		_comp->OnEnabled(henbl);
	}

	CList<CComponent*>::CIterator it = m_Components.Begin();
	while(it != m_Components.End())
	{
		(*it)->OnAttached(_comp);
		++it;
	}
}

void CNode::RemoveComponent(CComponent* _comp)
{
	CList<CComponent*>::CIterator it = m_Components.Begin();
	while(it != m_Components.End())
	{
		(*it)->OnDetached(_comp);
		++it;
	}

	_comp->OnEnabled(false);
	_comp->OnRemoved();

	m_Components.Erase(_comp);
}

void CNode::GetComponents(EComponentType _type, CList<CComponent*>& _list)
{
	CList<CComponent*>::CIterator it = m_Components.Begin();
	while(it != m_Components.End())
	{
		if((*it)->GetType() == _type)
		{
			_list.PushBack(*it);
		}
		++it;
	}
}

void CNode::GetComponents(CList<CComponent*>& _list)
{
	CList<CComponent*>::CIterator it = m_Components.Begin();
	while(it != m_Components.End())
	{
		_list.PushBack(*it);
		++it;
	}
}

void CNode::SetPosition(const CVector3& _position)
{
	if (m_Position != _position)
	{
		m_Position = _position;
		OnTransformed();
	}
}

void CNode::SetScale(const CVector3& _scale)
{
	if (m_Scale != _scale)
	{
		m_Scale = _scale;
		OnTransformed();
	}
}

void CNode::SetRotation(const CVector3& _rotation)
{
	if (m_Rotation != _rotation)
	{
		m_Rotation = _rotation;
		OnTransformed();
	}
}

void CNode::OnTransformed()
{
	if(m_Base)
	{
		m_Transform = m_Base->GetTransform();
	}
	else
	{
		m_Transform = CMatrix4::IDENTY;
	}

	CMatrix4 mRotate = CMatrix4::IDENTY;
	mRotate.Rotate(m_Rotation);

	CMatrix4 mTranslate = CMatrix4::IDENTY;
	mTranslate.Translate(m_Position);
            
	CMatrix4 mScale = CMatrix4::IDENTY;
	mScale.Scale(m_Scale);

	m_Transform *= mTranslate;
	m_Transform *= mRotate;
	m_Transform *= mScale;

	CList<CComponent*>::CIterator it = m_Components.Begin();
	while (it != m_Components.End())
	{
		(*it)->OnTransformed();
		++it;
	}

	CList<CNode*>::CIterator jt = m_Childs.Begin();
	while (jt != m_Childs.End())
	{
		CNode* nd = *jt;

		nd->OnTransformed();

		++jt;
	}
}

void CNode::SetEnabled(bool _enabled)
{
	m_Enabled = _enabled;

	bool henbl = false;

	if (m_Base)
	{
		henbl = m_Enabled && m_Base->m_HeirarhyEnabled;
	}

	if (henbl != m_HeirarhyEnabled)
	{
		m_HeirarhyEnabled = henbl;
		OnEnabled(henbl);
	}
}

void CNode::OnEnabled(bool _enable)
{
	CList<CComponent*>::CIterator it = m_Components.Begin();
	while (it != m_Components.End())
	{
		CComponent* cmp = *it;
		bool henbl = cmp->m_Enabled && _enable;
		if (cmp->m_HeirarhyEnabled != henbl)
		{
			cmp->m_HeirarhyEnabled = henbl;
			cmp->OnEnabled(henbl);
		}

		++it;
	}

	CList<CNode*>::CIterator jt = m_Childs.Begin();
	while (jt != m_Childs.End())
	{
		CNode* node = *jt;
		bool henbl = node->m_Enabled && _enable;
		if (node->m_HeirarhyEnabled != henbl)
		{
			node->m_HeirarhyEnabled = henbl;
			node->OnEnabled(henbl);
		}

		++jt;
	}
}

void CNode::HandleEvent(CEvent* _event)
{
	OnEvent(_event);

	CList<CComponent*>::CIterator it = m_Components.Begin();
	while (it != m_Components.End())
	{
		CComponent* cmp = *it;
		cmp->OnEvent(_event);

		++it;
	}

	CList<CNode*>::CIterator jt = m_Childs.Begin();
	while (jt != m_Childs.End())
	{
		CNode* node = *jt;
		node->HandleEvent(_event);

		++jt;
	}
}