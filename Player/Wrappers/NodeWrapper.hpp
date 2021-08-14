#pragma once

#include <Scene/Node.hpp>
#include <sqrat.h>

#include "Scene/Input/InputListener.hpp"
#include "Scene/Update/UpdateComponent.hpp"

#include <squirrel/sqobject.h>

class CNodeWrapper: public CNode
{
public:
	CNodeWrapper()
	{
		m_InputComponent = 0;
		sq_getstackobj(Sqrat::DefaultVM::Get(), -1, &m_Object);

		m_UpdateHandler = !Sqrat::Function(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "Update").IsNull();

		if(m_UpdateHandler)
		{
			AddComponent(new CUpdateComponent());
		}

		u32 icntr = 0;

		m_InputHandler_Touch = !Sqrat::Function(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputTouch").IsNull();

		if(m_InputHandler_Touch)
		{
			icntr++;
		}

		m_InputHandler_Mouse = !Sqrat::Function(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputMouse").IsNull();

		if(m_InputHandler_Mouse)
		{
			icntr++;
		}

		m_InputHandler_Keyboard	= !Sqrat::Function(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputKeyboard").IsNull();

		if(m_InputHandler_Keyboard)
		{
			icntr++;
		}

		m_InputHandler_Gesture = !Sqrat::Function(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputGesture").IsNull();

		if(m_InputHandler_Gesture)
		{
			icntr++;
		}

		if(icntr > 0)
		{
			m_InputComponent = new CInputListener();
			AddComponent(m_InputComponent);
		}
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CNodeWrapper, Sqrat::NoCopy<CNodeWrapper> > cl(_vm, "CNode");

		cl.Func("AddChild", &CNodeWrapper::sqAddChild);
		cl.Func("RemoveChild", &CNodeWrapper::sqRemoveChild);

		cl.Func("AddComponent", &CNodeWrapper::sqAddComponent);
		cl.Func("RemoveComponent", &CNodeWrapper::sqRemoveComponent);

		cl.Func("SetEnabled", &CNodeWrapper::SetEnabled);
		cl.Func("GetEnabled", &CNodeWrapper::GetEnabled);

		cl.Func("SetName", &CNodeWrapper::SetName);
		cl.Func("GetName", &CNodeWrapper::GetName);

		cl.Prop("Input", &CNodeWrapper::GetInputComponent);

		Sqrat::RootTable(_vm).Bind("CNode", cl);
	}

	void sqAddChild(CNodeWrapper* _child) { AddChild(_child); }
	void sqRemoveChild(CNodeWrapper* _child) { RemoveChild(_child); }

	void sqAddComponent(const Sqrat::Object& _obj)
	{
		//#TODO: Dirty hack for avoid deleting component twice: first in CNode destructor call and then when delete Squirell class
		sq_addref(Sqrat::DefaultVM::Get(), &(_obj.GetObject()));
		CComponent* cmp = _obj.Cast<CComponent*>();

		AddComponent(cmp);
	}

	void sqRemoveComponent(CComponent* _cmp)
	{
		RemoveComponent(_cmp);
	}

	void OnUpdate(f32 _dt)
	{
		if(m_UpdateHandler)
		{
			Sqrat::Function hndlr(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "Update");
			hndlr.Execute<f32>(_dt);
		}
	}

	void OnEvent(CEvent* _event)
	{
		if(_event->Type == EET_INPUT)
		{
			CInputEvent* ievent = (CInputEvent*)_event;

			if(ievent->InputType == CInputEvent::EIT_TOUCH)
			{
				CTouchEvent* tevent = (CTouchEvent*)ievent;

				Sqrat::Function hndlr(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputTouch");

				for(u32 i = 0; i < tevent->TouchCount; i++)
				{
					hndlr.Execute<u32, u32, f32, f32>(tevent->Touches[i].ID, tevent->Touches[i].TouchType, tevent->Touches[i].X, tevent->Touches[i].Y);
				}
			}
			else
			if(ievent->InputType == CInputEvent::EIT_MOUSE)
			{
				CMouseEvent* mevent = (CMouseEvent*)ievent;

				Sqrat::Function hndlr(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputMouse");

				hndlr.Execute<u32, f32, f32, f32, u32>(mevent->Event, mevent->X, mevent->Y, mevent->Wheel, mevent->Buttons);
			}
			else
			if(ievent->InputType == CInputEvent::EIT_KEYBOARD)
			{
				CKeyboardEvent* kevent = (CKeyboardEvent*)ievent;

				Sqrat::Function hndlr(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputKeyboard");

				hndlr.Execute<u32, bool, u32>(kevent->Key, kevent->PressedDown, ((u8)kevent->Control << 1) | (u8)kevent->Shift);
			}
			else
			if(ievent->InputType == CInputEvent::EIT_GESTURE)
			{
				CGestureEvent* gevent = (CGestureEvent*)ievent;
				
				Sqrat::Function hndlr(Sqrat::Object(m_Object, Sqrat::DefaultVM::Get()), "InputGesture");

				hndlr.Execute<u32, f32, f32>(gevent->Gesture, gevent->Param1, gevent->Param2);
			}
		}
	}

	CComponent* GetInputComponent(){return m_InputComponent;}

private:
	HSQOBJECT m_Object;

	bool m_UpdateHandler;
	bool m_InputHandler_Touch;
	bool m_InputHandler_Mouse;
	bool m_InputHandler_Keyboard;
	bool m_InputHandler_Gesture;

	CComponent* m_InputComponent;
};