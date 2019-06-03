#pragma once

#include "Core/Array.hpp"
#include "Core/LinkedList.hpp"

#include "Scene/System.hpp"
#include "Scene/Node.hpp"

namespace tthread
{
class mutex;
}

class CInputListener;
class CGesturer;

class CInputSystem: public CSystem, public CLinkedList<CInputListener>
{
public:
	CInputSystem();
	~CInputSystem();

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);

	void PushInput(CInputEvent* _event);
	void HandleInput(CInputEvent* _event);

	void Update(f32 _dt);
	void DropEvent();

private:
	bool			m_DropEvent;
	CInputListener* m_Iterator;
	CGesturer*		m_Gesturer;

	tthread::mutex* m_Mutex;

	CArray<CTouchEvent> m_TouchEvents;
	CArray<CKeyboardEvent> m_KeyboardEvents;
};