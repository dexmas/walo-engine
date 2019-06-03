#include <Device/Android/DeviceAndroid.hpp>
#include <Scene/Input/InputSystem.hpp>
#include <Game.hpp>

JNIEnv* CDeviceAndroid::s_JNIEnv = 0;
jobject CDeviceAndroid::s_assetManager = 0;
CString CDeviceAndroid::s_appPath = "";
CString CDeviceAndroid::s_savePath = "";

CDeviceAndroid::CDeviceAndroid()
{
	m_Type = EDT_ANDROID;
}

CDeviceAndroid::~CDeviceAndroid()
{
	
}

bool CDeviceAndroid::Init()
{
	m_Active = true;

	return true;
}

void CDeviceAndroid::Update()
{
	
}

bool CDeviceAndroid::Flush()
{
	return true;
}

bool CDeviceAndroid::SetContext()
{
	return true;
}
