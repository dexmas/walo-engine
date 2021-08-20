
#include "Game.hpp"
#include "Core/Log.hpp"
#include <stdio.h>

#if defined(WALO_PLATFORM_WIN32)
#include "Device/Win32/DeviceWin32.hpp"
#elif defined(WALO_PLATFORM_OSX)
#include "Device/OSX/DeviceOSX.hpp"
#endif

#ifdef _DEBUG
#include "Scene/DebugRenderer.hpp"
#endif

#include "Scene/Scene2D/Scene2D.hpp"
#include "Scene/Scene3D/Scene3D.hpp"
#include "Scene/Input/InputSystem.hpp"
#include "Scene/Update/UpdateSystem.hpp"
#include "Scene/Physics2D/Physics2D.hpp"
#include "Scene/Sound/SoundSystem.hpp"

CGame* CGame::m_pInstance = 0;

CGame::CGame()
{
	m_pInstance = this;

	SetName("Game");

	m_LastTime = 0;
	m_FPSLimitMax = 1000;
	m_FPSLimitMin = 5;

	m_FPSCounter = 0;
	m_FPSTimer = 0.0f;

	m_Timer = new CTimer();

	m_Device = CDevice::CreateDevice();
	m_Render = CRender::CreateRender();

	m_HeirarhyEnabled = true;
	m_Active = true;
}

CGame* CGame::Instance()
{
	if(m_pInstance)
		return m_pInstance;

	return 0;
}

bool CGame::Init()
{
	bool ret = true;
	
	ret &= m_Device->Init();
	ret &= m_Render->Init();
	
	return OnInit();
}

void CGame::Free()
{
	OnFree();
}

void CGame::Activate(bool _act)
{
	if(_act != m_Active)
	{
		m_Active = _act;
		
		if(m_Active)
		{
			OnResume();
			GetSystem<CSoundSystem>()->OnResume();
		}
		else
		{
			GetSystem<CSoundSystem>()->OnSuspend();
			OnSuspend();
		}
	}
}

void CGame::Run(f32 _dt)
{
    f32 fdt = _dt;
    
#if defined(WALO_PLATFORM_WIN32) || defined(WALO_PLATFORM_OSX)
	u64 ct = m_Timer->GetCurentTime();
	s64 dt = ct - m_LastTime;
	m_LastTime = ct;

	if(dt < 0)
		dt = 0;

	if(m_FPSLimitMin)
	{
		u64 mintime = 1000000LL / m_FPSLimitMin;
		if(dt > mintime)
			dt = mintime;
	}

	fdt = dt / 1000000.0f;
#endif

	m_Device->Update();

	if(m_Device->GetClosed())
		return;

	m_FPSTimer += fdt;
	m_FPSCounter++;

	if(m_FPSTimer >= 1.0f)
	{
#if defined(WALO_PLATFORM_WIN32)
        CString chaption = "WaloEngine [";
        chaption += m_FPSCounter;
        chaption += "FPS]";

		if(m_Device->GetType() == CDevice::EDT_WIN32)
		{
			((CDeviceWin32*)m_Device)->SetCaption(chaption.CStr());
		}
#elif  defined(WALO_PLATFORM_OSX)
        CString chaption = "WaloEngine [";
        chaption += m_FPSCounter;
        chaption += "FPS]";
        
        if(m_Device->GetType() == CDevice::EDT_OSX)
        {
            ((CDeviceOSX*)m_Device)->SetCaption(chaption.CStr());
        }
#endif

		m_FPSCounter = 0;
		m_FPSTimer = 1.0f - m_FPSTimer;
	}

	if(m_Device->GetActive())
	{
		GetSystem<CUpdateSystem>()->Update(fdt);
		GetSystem<CInputSystem>()->Update(fdt);
		GetSystem<CPhysics2D>()->Update(fdt);

		m_Render->BeginFrame();
		{
			m_Render->Clear(true, true, 1.0f);
        
			GetSystem<CScene3D>()->Render();
			GetSystem<CScene2D>()->Render();
			GetSystem<CPhysics2D>()->Render();
		}
		m_Render->EndFrame();
	}

#if defined(WALO_PLATFORM_WIN32)
	if(m_FPSLimitMax)
	{
		u64 maxtime = 1000000 / m_FPSLimitMax;
		u64 strtime = ct;
		u64 elptime = 0;

		while(1)
        {
            elptime = m_Timer->GetCurentTime() - strtime;

            if(elptime >= maxtime)
                break;

            if(maxtime - elptime >= 1000LL)
            {
                unsigned slptime = (u32)((maxtime - elptime) / 1000LL);
                m_Timer->Sleep(slptime);
            }
        }
	}
#endif
}

CTimer* CGame::GetTimer()
{
	return m_Timer;
}

CDevice* CGame::GetDevice()
{
	return m_Device;
}

CRender* CGame::GetRender()
{
	return m_Render;
}
