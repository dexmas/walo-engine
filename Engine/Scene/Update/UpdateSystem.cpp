#include "Scene/Update/UpdateSystem.hpp"
#include "Scene/Update/UpdateComponent.hpp"

void CUpdateSystem::AddComponent(CComponent* _comp)
{
	CUpdateComponent* ucmp = (CUpdateComponent*)_comp;
	
	PushBack(ucmp);
}

void CUpdateSystem::RemoveComponent(CComponent* _comp)
{
	CUpdateComponent* ucmp = (CUpdateComponent*)_comp;

	Remove(ucmp);
}

void CUpdateSystem::Update(f32 _dt)
{
	CUpdateComponent* lstnr = GetHead();
	CUpdateComponent* next = 0;
	while (lstnr)
	{
		next = lstnr->GetNext();

		lstnr->Update(_dt);

		lstnr = next;
	}
}