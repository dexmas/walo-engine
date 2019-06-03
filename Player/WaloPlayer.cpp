#include "WaloPlayer.hpp"

#include <Core/Log.hpp>
#include <Core/String.hpp>

#include <Filesystem/FileSystem.hpp>

#include <sqstdio.h>
#include <sqstdstring.h>
#include <stdarg.h>

#if defined(WALO_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#import <Device/iOS/AppDelegate.h>
#elif defined(WALO_PLATFORM_OSX)
#import <Cocoa/Cocoa.h>
#import <Device/OSX/AppDelegate.h>
#elif defined(WALO_PLATFORM_ANDROID)
#include <Device/Android/DeviceAndroid.hpp>
#elif defined(WALO_PLATFORM_WIN32)
HANDLE CPlayer::m_hConsole = 0;
#endif

CPlayer::CPlayer()
{
	sq_getstackobj(Sqrat::DefaultVM::Get(), -1, &m_Object);

	m_ConfHandler	 = Sqrat::Function(m_Object, "Configure");
	m_InitHandler	 = Sqrat::Function(m_Object, "Init");
	m_FreeHandler	 = Sqrat::Function(m_Object, "Free");
	m_SuspendHandler = Sqrat::Function(m_Object, "Suspend");
	m_ResumeHandler  = Sqrat::Function(m_Object, "Resume");
}

bool CPlayer::OnInit()
{
	srand((u32)(GetTimer()->GetCurentTime()));

	if(!m_InitHandler.IsNull())
	{
		m_InitHandler.Execute();
	}

	return true;
}

void CPlayer::OnFree()
{
	if(!m_FreeHandler.IsNull())
	{
		m_FreeHandler.Execute();
	}
}

void CPlayer::OnSuspend()
{
	if(!m_SuspendHandler.IsNull())
	{
		m_SuspendHandler.Execute();
	}
}

void CPlayer::OnResume()
{
	if (!m_ResumeHandler.IsNull())
	{
		m_ResumeHandler.Execute();
	}
}

void CPlayer::Configure()
{
	if (!m_ConfHandler.IsNull())
	{
		m_ConfHandler.Execute();
	}
}

bool CPlayer::RunScript(const char* _fname)
{
#if defined(WALO_PLATFORM_WIN32)
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

	HSQUIRRELVM vm = sq_open(1024);

	Sqrat::DefaultVM::Set(vm);

#ifdef _DEBUG
	sq_enabledebuginfo(vm, true);
#else
	sq_enabledebuginfo(vm, false);
#endif

	// Register error handlers
	{
		sq_setprintfunc(vm, &CPlayer::_PrintHandler, &CPlayer::_PrintHandler);

		sq_newclosure(vm, &CPlayer::_RuntimeErrorHandler, 0);
		sq_seterrorhandler(vm);

		sq_setcompilererrorhandler(vm, &CPlayer::_CompileErrorHandler);
	}

	// Register standart libraries
	{
		sq_pushroottable(vm);
		sqstd_register_stringlib(vm);
		sq_pop(vm, 1);
	}

	// Register import function
	{
		sq_pushroottable(vm);
		sq_pushstring(vm, _SC("import"), -1);
		sq_newclosure(vm, &CPlayer::_ScriptImport, 0);
		sq_newslot(vm, -3, 0);
		sq_pop(vm, 1); // pop sqrat table
	}

	_RegisterScriptClasses(vm);

	// Run 'main.nut'
	{
		if(!_ExecuteFile(vm, "main.nut"))
		{
			LOG_RELEASE("WaloPlayer: Can't execute 'main.nut'.\n");
			return false;
		}

		sq_pushroottable(vm);
		sq_call(vm, 1, false, true);

		sq_pop(vm, 1);
	}

	return true;
}

void CPlayer::_PrintHandler(HSQUIRRELVM _vm, const SQChar* _text,...)
{
#if defined(WALO_PLATFORM_WIN32)
	SetConsoleTextAttribute(m_hConsole, FOREGROUND_GREEN);
#endif

	va_list vl;
	va_start(vl, _text);
	VLOG(_text, vl);
	va_end(vl);

	fflush(stdout);
}

SQInteger CPlayer::_RuntimeErrorHandler(HSQUIRRELVM _vm)
{
	const SQChar *sErr = 0;
	if(sq_gettop(_vm) >= 1)
	{
		SQStackInfos si;

		SQChar buf[512];
		buf[0] = 0;

#if defined(WALO_PLATFORM_WIN32)
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED);
#endif

		if(SQ_SUCCEEDED(sq_stackinfos(_vm, 1, &si)))
		{
#ifndef _SQ64
			sprintf(buf, "%s : %d (%s)", si.source, si.line, si.funcname);
#else
			sprintf(buf, "%s : %lld (%s)", si.source, si.line, si.funcname);
#endif
		}

		if(SQ_SUCCEEDED(sq_getstring(_vm, 2, &sErr)))
		{
            LOG_RELEASE("WaloPlayer: [Runtime Error] %s :: Error: %s\n", buf, sErr);
		}
		else
		{
            LOG_RELEASE("WaloPlayer: [Runtime Error] %s :: Unknown Error.\n", buf);
		}
	}

	fflush(stdout);

	return 0;
}

void CPlayer::_CompileErrorHandler(HSQUIRRELVM _vm, const SQChar* _desc, const SQChar* _source, SQInteger _line, SQInteger _column)
{
#if defined(WALO_PLATFORM_WIN32)
	SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED);
#endif
    
#ifndef _SQ64
	LOG_RELEASE("WaloPlayer: [Compile Error] %s(%d:%d): %s\n", _source, _line, _column, _desc);
#else
	LOG_RELEASE("WaloPlayer: [Compile Error] %s(%lld:%lld): %s\n", _source, _line, _column, _desc);
#endif

	fflush(stdout);
}

bool CPlayer::_ExecuteFile(HSQUIRRELVM _vm, const char* _fname)
{
	CFileStream* file = CFileSystem::Instance()->OpenFile(_fname);
    if (file->Size() > 0)
    {
		s32 size = file->Size();
        char* content = new char[size];
        file->Read(content, size);
		if(SQ_FAILED(sq_compilebuffer(_vm, content, size, _fname, SQTrue)))
        {
            delete[] content;
            delete file;

            return false;
        }

        delete[] content;
        delete file;

		return true;
    }

	return false;
}

SQRESULT CPlayer::_ScriptImport(HSQUIRRELVM _vm)
{
	const SQChar* moduleName = 0;
	HSQOBJECT table;
	SQRESULT res = SQ_OK;

	SQInteger args = sq_gettop(_vm);
	if(args == 2) 
	{
		sq_pushroottable(_vm);
	}
	else
	if(args == 3)
	{
		//Table already in the stack
	}
	else
	{
		LOG_RELEASE("WaloPlayer: Script import error. Wrong number of parameters.\n");
		return SQ_ERROR;
	}

	sq_getstring(_vm, -2, &moduleName);
	sq_getstackobj(_vm, -1, &table);
	sq_addref(_vm, &table);

	sq_settop(_vm, 0); // Clear Stack
	sq_pushobject(_vm, table); // Push the target table onto the stack

	if(!moduleName)
	{
		LOG_RELEASE("WaloPlayer: Script import error. Wrong module name.\n");
		return SQ_ERROR;
	}

	if(!_ExecuteFile(_vm, moduleName))
	{
		LOG_RELEASE("WaloPlayer: Script import error. Can't execute file '%s'.\n", moduleName);
		return SQ_ERROR;
	}

	sq_push(_vm, -2);
	sq_call(_vm, 1, false, true);

	sq_settop(_vm, 0); // Clean up the stack (just in case the module load leaves it messy)
	sq_pushobject(_vm, table); // return the target table

	return res;
}

CAssetsDB* CPlayer::sqGetAssetsDB()
{
	return CAssetsDB::Instance();
}

CInputSystem* CPlayer::sqGetInputSystem()
{
	return GetSystem<CInputSystem>();
}

void CPlayer::sqSetScreenSize(u32 _w, u32 _h)
{
	GetDevice()->SetScreenSize(CVector2(_w, _h));
}

u32 CPlayer::sqGetScreenWidth()
{
    return GetDevice()->GetScreenSize().X;
}

u32 CPlayer::sqGetScreenHeight()
{
    return GetDevice()->GetScreenSize().Y;
}

void CPlayer::sqEnableDebug(bool _enable)
{
	//#TODO
}
void CPlayer::sqEnableConsole(bool _enable)
{
	//#TODO
}

void CPlayer::sqSetFPSLimit(u32 _fps)
{
	//#TODO
}

void CPlayer::_RegisterScriptClasses(HSQUIRRELVM _vm)
{
	CMathWrapper::Register(_vm);
	CVector2Wrapper::Register(_vm);
	CVector3Wrapper::Register(_vm);
	CRectWrapper::Register(_vm);
	CColor3Wrapper::Register(_vm);
	CComponentImplWrapper::Register(_vm);
	CAssetWrapper::Register(_vm);
	CAssetsDBWrapper::Register(_vm);
	CTextureWrapper::Register(_vm);
	CFontWrapper::Register(_vm);
	CMaterialWrapper::Register(_vm);
	CMeshWrapper::Register(_vm);
	CSoundStreamWrapper::Register(_vm);
	CNodeWrapper::Register(_vm);
	CSoundSourceWrapper::Register(_vm);
	CNode2DWrapper::Register(_vm);
	CSprite2DWrapper::Register(_vm);
	CProgress2DWrapper::Register(_vm);
	CText2DWrapper::Register(_vm);
	CTileMap2DWrapper::Register(_vm);
	CRigidBody2DWrapper::Register(_vm);
	CCollisionShape2DWrapper::Register(_vm);
	CCollisionBox2DWrapper::Register(_vm);
	CNode3DWrapper::Register(_vm);
	CModel3DWrapper::Register(_vm);
	CCamera3DWrapper::Register(_vm);
	CInputSystemWrapper::Register(_vm);
	CSaveFileWrapper::Register(_vm);

	Sqrat::DerivedClass<CPlayer, CNodeWrapper, Sqrat::NoCopy<CPlayer> > cl(_vm);

	cl.Func("SetScreenSize", &CPlayer::sqSetScreenSize);
	cl.Func("EnableDebug", &CPlayer::sqEnableDebug);
	cl.Func("EnableConsole", &CPlayer::sqEnableConsole);
	cl.Func("SetFPSLimit", &CPlayer::sqSetFPSLimit);

    cl.Prop("ScreenWidth", &CPlayer::sqGetScreenWidth);
    cl.Prop("ScreenHeight", &CPlayer::sqGetScreenHeight);
    
	cl.Prop<CAssetsDB*>("AssetsDB", &CPlayer::sqGetAssetsDB);
	cl.Prop<CInputSystem*>("Input", &CPlayer::sqGetInputSystem);

	Sqrat::RootTable(_vm).Bind("CGame", cl);
}

#if !defined(WALO_PLATFORM_ANDROID)

int main(int _argc, char* _argv[])
{
    if(!CPlayer::RunScript("main.nut"))
    {
        return -1;
    }
    
    CPlayer* pGame = (CPlayer*)CPlayer::Instance();
    
    if(!pGame)
    {
        LOG("WaloPlayer: Game instance not created in script.\n");
        return -1;
    }

	pGame->Configure();

#if defined(WALO_PLATFORM_WIN32)
    
    if(!pGame->Init())
    {
        LOG("WaloPlayer: Game initialization failed.\n");
        return -1;
    }

	while(!pGame->GetDevice()->GetClosed())
	{
		pGame->Run();
	}

	pGame->Free();

	delete pGame;
    
#elif defined(WALO_PLATFORM_IOS)
    
    @autoreleasepool
    {
        return UIApplicationMain(_argc, _argv, nil, NSStringFromClass([AppDelegate class]));
    }
    
#elif defined(WALO_PLATFORM_OSX)
    
    @autoreleasepool
    {
        [NSApplication sharedApplication];
        [NSApp setDelegate:[[AppDelegate alloc] init]];
        [NSApp run];
    }
    
#else

    #error Entry point for WaloPlayer not defined

#endif
    
	return 0;
}

#endif

