#include <Device/Win32/DeviceWin32.hpp>
#include <Scene/Input/InputSystem.hpp>
#include <Game.hpp>

#include <glew.h>
#include <wglew.h>

CDeviceWin32* CDeviceWin32::m_Self = 0;

CDeviceWin32::CDeviceWin32()
{
	m_WindowHandler = 0;
	m_Fullscreen = false;
	m_Resized = false;
	m_ExternalWindow = false;
	m_Type = EDT_WIN32;

	m_ScreenSize = CVector2(960.0, 640.0);

	m_Multisamples = 0;
	m_MSAAPixelFormat = 0;

	m_Self = this;
}

CDeviceWin32::~CDeviceWin32()
{
	if(!m_WindowHandler)
		return;

	wglMakeCurrent(0, 0);

	if(m_DeviceContext)
		ReleaseDC(m_WindowHandler, m_DeviceContext);

	MSG msg;
	PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	PostQuitMessage(0);
	PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	DestroyWindow(m_WindowHandler);
}

static bool sPrevMouseLButtonState = false;

LRESULT CALLBACK CDeviceWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static s32 ClickCount=0;
	if(GetCapture() != hWnd && ClickCount > 0)
		ClickCount = 0;

	struct messageMap
	{
		s32		group;
		UINT	winMessage;
		s32		engMessage;
	};

	static messageMap mouseMap[] =
	{
		{0, WM_LBUTTONDOWN, CMouseEvent::EME_LMOUSE_DOWN},
		{1, WM_LBUTTONUP,   CMouseEvent::EME_LMOUSE_UP},
		{0, WM_RBUTTONDOWN, CMouseEvent::EME_RMOUSE_DOWN},
		{1, WM_RBUTTONUP,   CMouseEvent::EME_RMOUSE_UP},
		{0, WM_MBUTTONDOWN, CMouseEvent::EME_MMOUSE_DOWN},
		{1, WM_MBUTTONUP,   CMouseEvent::EME_MMOUSE_UP},
		{2, WM_MOUSEMOVE,   CMouseEvent::EME_MOUSE_MOVED},
		{3, WM_MOUSEWHEEL,  CMouseEvent::EME_MOUSE_WHEEL},
		{-1, 0, 0}
	};

	// handle grouped events
	messageMap * m = mouseMap;
	while(m->group >=0 && m->winMessage != message)
		m += 1;

	if(m->group >= 0)
	{
		if(m->group == 0)	// down
		{
			ClickCount++;
			SetCapture(hWnd);
		}
		else
		if(m->group == 1)	// up
		{
			ClickCount--;
			if (ClickCount<1)
			{
				ClickCount=0;
				ReleaseCapture();
			}
		}

		CMouseEvent mevent((CMouseEvent::EMouseEventType) m->engMessage);

		mevent.X	    = (short)LOWORD(lParam);
		mevent.Y	    = (short)HIWORD(lParam);
		mevent.Shift   = ((LOWORD(wParam) & MK_SHIFT) != 0);
		mevent.Control = ((LOWORD(wParam) & MK_CONTROL) != 0);
		mevent.Buttons = wParam & ( MK_LBUTTON | MK_RBUTTON);

		if (wParam & MK_MBUTTON)
			mevent.Buttons |= CMouseEvent::EMBM_MIDDLE;

		mevent.Wheel = 0.f;

		if( m->group == 3)
		{
			POINT p;
			p.x = 0; p.y = 0;
			ClientToScreen(hWnd, &p);
			mevent.X -= p.x;
			mevent.Y -= p.y;
			mevent.Wheel = ((f32)((short)HIWORD(wParam)))/(f32)WHEEL_DELTA;
		}

		if(CGame::Instance())
		{
			CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&mevent);

			if(sPrevMouseLButtonState || (wParam & MK_LBUTTON))
			{
				bool move = sPrevMouseLButtonState && (wParam & MK_LBUTTON);
				CTouchEvent tevent;
				tevent.TouchCount = 1;
				tevent.Touches[0].ID = 0;
				tevent.Touches[0].TouchType = move?CTouchEvent::ETE_MOVE:(sPrevMouseLButtonState?CTouchEvent::ETE_END:CTouchEvent::ETE_BEGIN);
				tevent.Touches[0].X = mevent.X;
				tevent.Touches[0].Y = mevent.Y;
				tevent.Touches[0].Time = u32(CGame::Instance()->GetTimer()->GetCurentTime());

				sPrevMouseLButtonState = (wParam & MK_LBUTTON);

				CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&tevent);
			}
		}
		return 0;
	}

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			BYTE allKeys[256];

			CKeyboardEvent kevent;

			kevent.Key = (CKeyboardEvent::EKeyCode)wParam;
			kevent.PressedDown = (message==WM_KEYDOWN || message == WM_SYSKEYDOWN);

			const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.

			if(kevent.Key == CKeyboardEvent::KEY_SHIFT)
			{
				// this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
				kevent.Key = (CKeyboardEvent::EKeyCode)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
			}
			if(kevent.Key == CKeyboardEvent::KEY_CONTROL)
			{
				kevent.Key = (CKeyboardEvent::EKeyCode)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
				// some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
				if(lParam & 0x1000000)
					kevent.Key = CKeyboardEvent::KEY_RCONTROL;
			}
			if(kevent.Key == CKeyboardEvent::KEY_MENU )
			{
				kevent.Key = (CKeyboardEvent::EKeyCode)MapVirtualKey( ((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
				if(lParam & 0x1000000)
					kevent.Key = CKeyboardEvent::KEY_RMENU;
			}

			GetKeyboardState(allKeys);

			kevent.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
			kevent.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);

			// allow composing characters like '@' with Alt Gr on non-US keyboards
			if((allKeys[VK_MENU] & 0x80) != 0)
				kevent.Control = 0;

			if(CGame::Instance())
			{
				CGame::Instance()->GetSystem<CInputSystem>()->PushInput(&kevent);
			}

			if(message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
				return DefWindowProc(hWnd, message, wParam, lParam);
			else
				return 0;
		}

	case WM_SIZE:
		{
			// resize
			if(m_Self)
				m_Self->m_Resized = true;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		// prevent screensaver or monitor powersave mode from starting
		if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
			(wParam & 0xFFF0) == SC_MONITORPOWER)
			return 0;
		break;

	case WM_ACTIVATE:
		// we need to take care for screen changes, e.g. Alt-Tab
		if (m_Self)
		{
			if ((wParam&0xFF)==WA_INACTIVE)
			{
				m_Self->m_Active = false;

				if(CGame::Instance())
				{
					CGame::Instance()->Activate(false);
				}
			}
			else
			{
				m_Self->m_Active = true;

				if(CGame::Instance())
				{
					CGame::Instance()->Activate(true);
				}
			}
		}
		break;

	case WM_SETCURSOR:
		// because Windows forgot about that in the meantime
		//if(Self)
		//	Self->SetCursorVisible(Self->GetCursorVisible());
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool CDeviceWin32::Init()
{
	HINSTANCE hInstance = GetModuleHandle(0);

	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = LONG(m_ScreenSize.X);
	clientSize.bottom = LONG(m_ScreenSize.Y);

	DWORD style = WS_POPUP;

	if(!m_Fullscreen)
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&clientSize, style, FALSE);

	const s32 realWidth = clientSize.right - clientSize.left;
	const s32 realHeight = clientSize.bottom - clientSize.top;

	s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if(windowLeft < 0)
		windowLeft = 0;
	if(windowTop < 0)
		windowTop = 0;	// make sure window menus are in screen on creation

	if(m_Fullscreen)
	{
		windowLeft = 0;
		windowTop = 0;
	}

	if(!m_WindowHandler)
	{
		WNDCLASSEX wcex;
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(110));;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= L"WaloWin32Class";
		wcex.hIconSm		= (HICON)LoadImage(hInstance, MAKEINTRESOURCE(110), IMAGE_ICON, 32, 32, 0);

		RegisterClassEx(&wcex);

		m_WindowHandler = CreateWindow(L"WaloWin32Class", L"WaloEngine", style, windowLeft, windowTop, realWidth, realHeight, NULL, NULL, hInstance, NULL);

		if(!m_WindowHandler)
			return false;
	}
	else 
	{
		RECT r;
		GetWindowRect(m_WindowHandler, &r);
		m_ScreenSize.X = f32(r.right - r.left);
		m_ScreenSize.Y = f32(r.bottom - r.top);
		m_Fullscreen = false;
		m_ExternalWindow = true;
		m_Multisamples = 0;
	}

	m_DeviceContext = GetDC(m_WindowHandler);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// choose pixelformat
	GLuint PixelFormat = ChoosePixelFormat(m_DeviceContext, &pfd);

	if(!PixelFormat || !SetPixelFormat(m_DeviceContext, PixelFormat, &pfd))
    {
        ReleaseDC(m_WindowHandler, m_DeviceContext);
		DestroyWindow(m_WindowHandler);

        return false;
    }

	m_RenderContext = wglCreateContext(m_DeviceContext);

	if(!m_RenderContext)
	{
		ReleaseDC(m_WindowHandler, m_DeviceContext);
		DestroyWindow(m_WindowHandler);

		return false;
	}

	if(!SetContext())
	{
		wglDeleteContext(m_RenderContext);
		ReleaseDC(m_WindowHandler, m_DeviceContext);
		DestroyWindow(m_WindowHandler);

		return false;
	}

	if(glewInit() != GLEW_OK || !GLEW_VERSION_2_1)
    {
        wglDeleteContext(m_RenderContext);
		ReleaseDC(m_WindowHandler, m_DeviceContext);
		DestroyWindow(m_WindowHandler);

        return false;
    }

	if(m_Multisamples)
	{
		if(GLEW_ARB_multisample && WGLEW_ARB_pixel_format)
		{
			while(m_Multisamples > 0)
			{
				UINT NumFormats = 0;

				int PFAttribs[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
					WGL_COLOR_BITS_ARB, 32,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB, m_Multisamples,
					0
				};

				if(wglChoosePixelFormatARB(m_DeviceContext, PFAttribs, NULL, 1, &m_MSAAPixelFormat, &NumFormats) == TRUE && NumFormats > 0) 
					break;

				m_Multisamples--;
			}

			wglMakeCurrent(0, 0);

			wglDeleteContext(m_RenderContext);

			ReleaseDC(m_WindowHandler, m_DeviceContext);

			DestroyWindow(m_WindowHandler);

			m_WindowHandler = CreateWindow(L"WaloWin32Class", L"WaloEngine", style, windowLeft, windowTop, realWidth, realHeight, NULL, NULL, hInstance, NULL);

			if(!m_WindowHandler)
				return false;

			m_DeviceContext = GetDC(m_WindowHandler);

			if(!m_MSAAPixelFormat || !SetPixelFormat(m_DeviceContext, m_MSAAPixelFormat, &pfd))
			{
				ReleaseDC(m_WindowHandler, m_DeviceContext);
				DestroyWindow(m_WindowHandler);

				return false;
			}

			m_RenderContext = wglCreateContext(m_DeviceContext);

			if(!m_RenderContext)
			{
				ReleaseDC(m_WindowHandler, m_DeviceContext);
				DestroyWindow(m_WindowHandler);

				return false;
			}

			if(!SetContext())
			{
				wglDeleteContext(m_RenderContext);
				ReleaseDC(m_WindowHandler, m_DeviceContext);
				DestroyWindow(m_WindowHandler);

				return false;
			}

			if(glewInit() != GLEW_OK || !GLEW_VERSION_2_1)
			{
				wglDeleteContext(m_RenderContext);
				ReleaseDC(m_WindowHandler, m_DeviceContext);
				DestroyWindow(m_WindowHandler);

				return false;
			}
		}
		else
		{
			m_Multisamples = 0;
		}
	}
	else
	{
		m_Multisamples = 0;
	}

	ShowWindow(m_WindowHandler, SW_SHOW);
	UpdateWindow(m_WindowHandler);
	MoveWindow(m_WindowHandler, windowLeft, windowTop, realWidth, realHeight, TRUE);

	SetActiveWindow(m_WindowHandler);
	SetForegroundWindow(m_WindowHandler);

	m_Resized = true;

	if(WGLEW_EXT_swap_control)
    {
        wglSwapIntervalEXT(0);
    }

	SetCaption("WaloEngine");

	return true;
}

void CDeviceWin32::Update()
{
	if(m_Close)
		return;

	MSG msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// No message translation because we don't use WM_CHAR and it would conflict with our
		// deadkey handling.

		if (m_ExternalWindow && msg.hwnd == m_WindowHandler)
			WndProc(m_WindowHandler, msg.message, msg.wParam, msg.lParam);
		else
			DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			m_Close = true;
	}

	if(!m_Close && m_Resized)	
	{

	}

	if(!m_Active)
		Sleep(1);
}

bool CDeviceWin32::Flush()
{
	return (SwapBuffers(m_DeviceContext) == TRUE);
}

bool CDeviceWin32::SetContext()
{
	return (wglMakeCurrent(m_DeviceContext, m_RenderContext) == TRUE);
}

void CDeviceWin32::SetCaption(const char* _str)
{
	SetWindowTextA(m_WindowHandler, _str);
}
