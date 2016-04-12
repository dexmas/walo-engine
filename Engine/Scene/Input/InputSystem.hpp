#pragma once

#include "Core/Array.hpp"
#include "Core/LinkedList.hpp"

#include "Scene/System.hpp"
#include "Scene/Node.hpp"

class CInputListener;
class CGesturer;

class CInputSystem: public CSystem, public CLinkedList<CInputListener>
{
public:
	CInputSystem();
	~CInputSystem();

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);

	void HandleInput(CInputEvent* _event);

	void DropEvent();

private:
	bool			m_DropEvent;
	CInputListener* m_Iterator;
	CGesturer*		m_Gesturer;
};