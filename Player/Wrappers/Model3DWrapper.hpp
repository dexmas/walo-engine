#pragma once

#include "Node3DWrapper.hpp"
#include <Scene/Scene3D/StaticModel.hpp>

class CModel3DWrapper: public CNode3DWrapper
{
public:
	CModel3DWrapper()
	{
		m_Object3D = new CStaticModel();
		AddComponent(m_Object3D);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CModel3DWrapper, CNode3DWrapper, Sqrat::NoCopy<CModel3DWrapper> > cl(_vm, "CModel3D");

		cl.Func("SetMesh", &CModel3DWrapper::sqSetMesh);
		
		Sqrat::RootTable(_vm).Bind("CModel3D", cl);
	}

	void sqSetMesh(CMesh* _mesh)
	{
		GetObject3D<CStaticModel>()->SetMesh(_mesh);
	}
};