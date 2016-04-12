#pragma once

#include "Game.hpp"
#include "Core/LinkedList.hpp"
#include "Scene/Component.hpp"
#include "Scene/Input/InputSystem.hpp"

class CInputListener: public CComponentImpl<CInputSystem>, public CLinkedListNode<CInputListener>
{
	friend class CInputSystem;

public:
	CInputListener():CComponentImpl(ECT_INPUT)
	{
		int i = 0;
	}

	virtual ~CInputListener()
	{
		int i = 0;
	}

	void HandleInput(CInputEvent* _event)
	{
		m_Node->OnEvent(_event);
	}
};