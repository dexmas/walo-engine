#include "Scene/Input/InputSystem.hpp"
#include "Scene/Input/InputListener.hpp"
#include "Scene/Input/Gesturer.hpp"

CInputSystem::CInputSystem()
{
	m_Gesturer = new CGesturer();
}

CInputSystem::~CInputSystem()
{
	delete m_Gesturer;
}

void CInputSystem::AddComponent(CComponent* _comp)
{
	CInputListener* lstnr = (CInputListener*)_comp;

	CInputListener* head = GetHead();

	PushFront(lstnr);
}

void CInputSystem::RemoveComponent(CComponent* _comp)
{
	CInputListener* lstnr = (CInputListener*)_comp;

	if(m_Iterator && lstnr == m_Iterator)
	{
		m_Iterator = m_Iterator->GetNext();
	}

	Remove(lstnr);
}

void CInputSystem::HandleInput(CInputEvent* _event)
{
	if(_event->Type != CInputEvent::EIT_GESTURE)
	{
		if(m_Gesturer->HandleInput(_event))
		{
			return;
		}
	}

	m_Iterator = GetHead();
	m_DropEvent = false;

	while (m_Iterator)
	{
		m_Iterator->HandleInput(_event);

		if(m_DropEvent || !m_Iterator)
		{
			break;
		}

		m_Iterator = m_Iterator->GetNext();
	}
}

void CInputSystem::DropEvent()
{
	m_DropEvent = true;
}