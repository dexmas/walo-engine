#pragma once

#include "Node3DWrapper.hpp"
#include <Scene/Scene3D/Camera3D.hpp>

class CCamera3DWrapper: public CNode3DWrapper
{
public:
	CCamera3DWrapper()
	{
		m_Camera = new CCamera3D();
		AddComponent(m_Camera);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CCamera3DWrapper, CNode3DWrapper, Sqrat::NoCopy<CCamera3DWrapper> > cl(_vm);
		
		Sqrat::RootTable(_vm).Bind("CCamera3D", cl);
	}

private:
	CCamera3D* m_Camera;
};