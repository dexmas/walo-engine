#pragma once

#include <Game.hpp>
#include <assert.h>
#include <sqrat.h>

#include "Wrappers/MathWrapper.hpp"
#include "Wrappers/VectorWrapper.hpp"
#include "Wrappers/RectWrapper.hpp"
#include "Wrappers/ColorWrapper.hpp"
#include "Wrappers/ComponentWrapper.hpp"
#include "Wrappers/AssetWrapper.hpp"
#include "Wrappers/AssetsDBWrapper.hpp"
#include "Wrappers/TextureWrapper.hpp"
#include "Wrappers/FontWrapper.hpp"
#include "Wrappers/MaterialWrapper.hpp"
#include "Wrappers/MeshWrapper.hpp"
#include "Wrappers/SoundStreamWrapper.hpp"
#include "Wrappers/NodeWrapper.hpp"
#include "Wrappers/SoundSourceWrapper.hpp"
#include "Wrappers/Node2DWrapper.hpp"
#include "Wrappers/Sprite2DWrapper.hpp"
#include "Wrappers/Progress2DWrapper.hpp"
#include "Wrappers/Text2DWrapper.hpp"
#include "Wrappers/RigidBody2DWrapper.hpp"
#include "Wrappers/CollisionShape2DWrapper.hpp"
#include "Wrappers/CollisionBox2DWrapper.hpp"
#include "Wrappers/Node3DWrapper.hpp"
#include "Wrappers/Model3DWrapper.hpp"
#include "Wrappers/Camera3DWrapper.hpp"
#include "Wrappers/InputSystemWrapper.hpp"
#include "Wrappers/SaveFileWrapper.hpp"

#include "Wrappers/PathMap.hpp"

class CPlayer: public CGame
{
public:
	CPlayer();

	static bool RunScript(const char* _fname);

	bool OnInit();
	void OnFree();
	void OnSuspend();
	void OnResume();

	void Configure();

	CAssetsDB* sqGetAssetsDB();
	CInputSystem* sqGetInputSystem();

	void sqSetScreenSize(u32 _w, u32 _h);
    u32 sqGetScreenWidth();
    u32 sqGetScreenHeight();
	void sqEnableDebug(bool _enable);
	void sqEnableConsole(bool _enable);
	void sqSetFPSLimit(u32 _fps);

private:
	static void		  _PrintHandler(HSQUIRRELVM _vm, const SQChar* _text,...);
	static SQInteger  _RuntimeErrorHandler(HSQUIRRELVM _vm);
	static void		  _CompileErrorHandler(HSQUIRRELVM _vm, const SQChar* _desc, const SQChar* _source, SQInteger _line, SQInteger _column);
	static SQRESULT   _ScriptImport(HSQUIRRELVM _vm);
	static bool		  _ExecuteFile(HSQUIRRELVM _vm, const char* _fname);
	static void		  _RegisterScriptClasses(HSQUIRRELVM _vm);

	static HSQUIRRELVM	m_SquirrelVirtualMachine;

	HSQOBJECT m_Object;

	Sqrat::Function m_ConfHandler;
	Sqrat::Function m_InitHandler;
	Sqrat::Function m_FreeHandler;
	Sqrat::Function m_SuspendHandler;
	Sqrat::Function m_ResumeHandler;
};