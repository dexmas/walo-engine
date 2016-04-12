#pragma once

#include "Scene/System.hpp"
#include "Scene/Node.hpp"
#include "Core/Array.hpp"
#include "Core/LinkedList.hpp"

class CUpdateComponent;

class CUpdateSystem: public CSystem, public CLinkedList<CUpdateComponent>
{
public:
	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);

	void Update(f32 _dt);
};