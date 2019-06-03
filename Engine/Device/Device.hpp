#pragma once

#include "Core/Types.hpp"
#include "Core/Vector.hpp"

class CDevice
{
public:
	enum EDeviceType
	{
		EDT_NULL = 0,
		EDT_WIN32,
		EDT_LINUX,
		EDT_OSX,
		EDT_IOS,
		EDT_ANDROID,
		EDT_WINRT,

		EDT_COUNT
	};

	CDevice();

	virtual ~CDevice();

	static CDevice* CreateDevice();

	virtual bool	Init() = 0;
	virtual void	Update() = 0;
	virtual bool	Flush() = 0;
	virtual bool	SetContext() = 0;

	CVector2 GetScreenSize() const 
	{
		return m_ScreenSize;
	}

	void SetScreenSize(const CVector2& _size)
	{
		m_ScreenSize = _size;
	}

	EDeviceType GetType(){return m_Type;}
	bool GetClosed() {return m_Close;}
	bool GetActive() {return m_Active;}

protected:
	bool					m_Close;
	EDeviceType				m_Type;
	bool					m_Active;

	CVector2				m_ScreenSize;
	bool					m_Landscape;
	u32						m_Multisamples;
};
