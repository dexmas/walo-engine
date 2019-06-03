#include "Scene/Input/InputSystem.hpp"
#include "Scene/Input/InputListener.hpp"
#include "Scene/Input/Gesturer.hpp"

#include <tinythread.h>

CInputSystem::CInputSystem()
{
	m_Mutex = new tthread::mutex();
	m_Gesturer = new CGesturer();
}

CInputSystem::~CInputSystem()
{
	delete m_Mutex;
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

void CInputSystem::PushInput(CInputEvent* _event)
{
	tthread::lock_guard<tthread::mutex> guard(*m_Mutex);

	if(_event->InputType == CInputEvent::EIT_TOUCH)
	{
		CTouchEvent e = *((CTouchEvent*)_event);
		m_TouchEvents.Insert(e);
	}
	else
	if(_event->InputType == CInputEvent::EIT_KEYBOARD)
	{
		CKeyboardEvent e = *((CKeyboardEvent*)_event);
		m_KeyboardEvents.Insert(e);
	}
	else
	{
		HandleInput(_event);
	}
}

void CInputSystem::HandleInput(CInputEvent* _event)
{
	if(_event->InputType != CInputEvent::EIT_GESTURE)
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

void CInputSystem::Update(f32 _dt)
{
	{
	tthread::lock_guard<tthread::mutex> guard(*m_Mutex);

	for(u32 i=0; i < m_TouchEvents.Size(); i++)
	{
		HandleInput(&(m_TouchEvents[i]));
	}
	m_TouchEvents.Clear();
	for(u32 i=0; i < m_KeyboardEvents.Size(); i++)
	{
		HandleInput(&(m_KeyboardEvents[i]));
	}
	m_KeyboardEvents.Clear();
	}

	m_Gesturer->Update(_dt);
}

void CInputSystem::DropEvent()
{
	m_DropEvent = true;
}
