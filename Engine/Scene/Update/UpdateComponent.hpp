#pragma once

#include "Game.hpp"
#include "Core/LinkedList.hpp"
#include "Scene/Component.hpp"
#include "Scene/Update/UpdateSystem.hpp"

class CUpdateComponent: public CComponentImpl<CUpdateSystem>, public CLinkedListNode<CUpdateComponent>
{
	friend class CUpdateSystem;
public:
	CUpdateComponent():CComponentImpl(ECT_UPDATE)
	{
		int i = 0;
	}

	virtual ~CUpdateComponent()
	{
		int i = 0;
	}

	void Update(f32 _dt)
	{
		m_Node->OnUpdate(_dt);
	}
};