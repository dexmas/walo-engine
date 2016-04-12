#pragma once

#include "Game.hpp"

class CNode;
class CSystem;

enum EComponentType
{
	ECT_OBJECT2D,
	ECT_SPRITE2D,
	ECT_PROGRESS2D,
	ECT_TEXT2D,
	ECT_CAMERA3D,
	ECT_OBJECT3D,
	ECT_STATICMODEL,
	ECT_RIGIDBODY2D,
	ECT_COLLISIONBOX2D,
	ECT_SOUNDLISTENER,
	ECT_SOUNDSOURCE,
	ECT_INPUT,
	ECT_UPDATE
};

class CComponent
{
	friend class CNode;
public:
	CComponent(EComponentType _type):m_Type(_type)
	{
		m_Node = 0;
		m_Enabled = true;
		m_HeirarhyEnabled = false;
	}

	virtual ~CComponent()
	{
		if (m_HeirarhyEnabled)
		{
			OnEnabled(false);
		}
	}

	EComponentType GetType()
	{
		return m_Type;
	}

	bool GetEnabled()
	{
		return m_Enabled;
	}

	void SetEnabled(bool _enabled)
	{
		m_Enabled = _enabled;

		bool henbl = false;

		if (m_Node)
		{
			henbl = m_Enabled && m_Node->m_HeirarhyEnabled;
		}

		if (henbl != m_HeirarhyEnabled)
		{
			m_HeirarhyEnabled = henbl;
			OnEnabled(henbl);
		}
	}

	CNode* GetNode()
	{
		return m_Node;
	}

	virtual void OnAssigned(CNode* _node)
	{
		m_Node = _node;
	}

	virtual void OnRemoved()
	{
		m_Node = 0;
	}

	virtual void OnAttached(CComponent* _component){}
	virtual void OnDetached(CComponent* _component){}
	virtual void OnTransformed(){}
	virtual void OnEnabled(bool _enabled){}
	virtual void OnEvent(CEvent* _event){}

protected:

	CNode*			m_Node;
	EComponentType	m_Type;
	bool			m_Enabled;
	bool			m_HeirarhyEnabled;
};

template<class TSystem>
class CComponentImpl: public CComponent
{
public:
	CComponentImpl(EComponentType _type):CComponent(_type)
	{
		m_System = CGame::Instance()->GetSystem<TSystem>();
	}

	virtual ~CComponentImpl()
	{
		int i = 0;
	}

	virtual void OnEnabled(bool _enabled)
	{
		if(_enabled)
		{
			m_System->AddComponent(this);
		}
		else
		{
			m_System->RemoveComponent(this);
		}
	}

protected:
	TSystem*  m_System;
};