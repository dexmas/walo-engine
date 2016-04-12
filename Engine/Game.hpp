#pragma once

#include "Device/Timer.hpp"
#include "Device/Device.hpp"
#include "Render/Render.hpp"
#include "Device/Event.hpp"

#include "Scene/Node.hpp"
#include "Scene/System.hpp"

class CGame: public CNode
{
public:
	CGame();

	bool Init();
	void Free();
	void Activate(bool _act);
	void Run(f32 _dt = 0.0f);

	virtual bool OnInit(){return true;}
	virtual void OnFree(){}
	virtual void OnSuspend(){}
	virtual void OnResume(){}

	CTimer*  GetTimer();
	CDevice* GetDevice();
	CRender* GetRender();

	template <class T>
	T* GetSystem()
	{
		T* system = T::template Instance<T>(false);
		if(!system)
		{
			bool inserted = false;

			system = T::template Instance<T>();

			m_Systems.PushBack(system);
		}

		return system;
	}

	static CGame* Instance();

private:
	static CGame*  m_pInstance;

	CList<CSystem*> m_Systems;

	CTimer*    m_Timer;
	CDevice*   m_Device;
	CRender*   m_Render;

	u32		m_FPSCounter;
	f32		m_FPSTimer;

	u64		m_LastTime;
	u32		m_FPSLimitMax;
	u32		m_FPSLimitMin;

	bool    m_Active;
};
