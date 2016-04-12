#pragma once

#include "NodeWrapper.hpp"
#include <Scene/Scene3D/Object3D.hpp>

#include "Scene/Sound/SoundSource.hpp"
#include "Assets/AssetsDB.hpp"

class CNode3DWrapper: public CNodeWrapper
{
public:

	CNode3DWrapper()
	{
		m_Object3D = 0;
	}

	template<class T>
	T* GetObject3D() 
	{
		if(!m_Object3D)
		{
			m_Object3D = new CObject3D();
			AddComponent(m_Object3D);
		}

		return (T*)m_Object3D;
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CNode3DWrapper, CNodeWrapper, Sqrat::NoCopy<CNode3DWrapper> > cl(_vm);

		cl.Func("SetPosition", &CNode3DWrapper::sqSetPosition);
		cl.Func("SetRotation", &CNode3DWrapper::sqSetRotation);
		cl.Func("SetScale", &CNode3DWrapper::sqSetScale);

		Sqrat::RootTable(_vm).Bind("CNode3D", cl);
	}

	void sqSetPosition(f32 _x, f32 _y, f32 _z)
	{
		SetPosition(CVector3(_x, _y, _z));
	}

	void sqSetRotation(f32 _x, f32 _y, f32 _z)
	{
		SetRotation(CVector3(_x, _y, _z));
	}

	void sqSetScale(f32 _x, f32 _y, f32 _z)
	{
		SetScale(CVector3(_x, _y, _z));
	}

protected:
	CObject3D* m_Object3D;
};