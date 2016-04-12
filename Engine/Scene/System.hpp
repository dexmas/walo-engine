#pragma once

#include "Core/String.hpp"
#include "Core/List.hpp"

class CComponent;
class CInputEvent;

class CSystem
{
	friend class CGame;
public:
	CSystem(){}
	virtual ~CSystem(){}

	virtual void AddComponent(CComponent* _comp){}
	virtual void RemoveComponent(CComponent* _comp){}

private:
	template<class T>
	static T* Instance(bool _create = true) 
	{
        static T* instance = 0;

		if(_create && !instance)
			instance = new T();

        return instance;
    }
};