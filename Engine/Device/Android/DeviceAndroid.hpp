#pragma once

#include "Device/Device.hpp"
#include "Core/String.hpp"

#include <jni.h>
#include <android/asset_manager.h>

class CDeviceAndroid: public CDevice
{
public:
	CDeviceAndroid();
	~CDeviceAndroid();

	static void InitFromJava(JNIEnv* _env, jobject _assetManager, const char* _appPath, const char* _savePath)
	{
		s_JNIEnv = _env;
		s_assetManager = _assetManager;
		s_appPath = _appPath;
		s_savePath = _savePath;
	}

	static JNIEnv* GetJNIEnv()
	{
		return s_JNIEnv;
	}

	static jobject GetAssetManager()
	{
		return s_assetManager;
	}

	static CString& GetAppPath()
	{
		return s_appPath;
	}

	static CString& GetSavePath()
	{
		return s_savePath;
	}

	bool Init();
	void Update();
	bool Flush();
	bool SetContext();

private:
	static JNIEnv* s_JNIEnv;
	static jobject s_assetManager;
	static CString s_appPath;
	static CString s_savePath;
};
