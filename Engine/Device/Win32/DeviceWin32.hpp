#pragma once

#include "Device/Device.hpp"

#include <Windows.h>
#include <time.h>

class CDeviceWin32: public CDevice
{
public:
	CDeviceWin32();
	~CDeviceWin32();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool Init();
	void Update();
	bool Flush();
	bool SetContext();

	void SetCaption(const char* _str);

private:
	static CDeviceWin32* m_Self;

	HGLRC m_RenderContext;
	HDC   m_DeviceContext;
	HWND  m_WindowHandler;
	bool  m_Fullscreen;
	bool  m_Resized;
	bool  m_ExternalWindow;
	s32   m_MSAAPixelFormat;
};
