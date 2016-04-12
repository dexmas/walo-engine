#include "Device/iOS/DeviceIOS.hpp"


CDeviceIOS::CDeviceIOS()
{
    m_Type = EDT_IOS;
}

bool CDeviceIOS::Init()
{
    m_Active = true;
    
    return true;
}

void CDeviceIOS::Update()
{

}

bool CDeviceIOS::Flush()
{
    return true;
}

bool CDeviceIOS::SetContext()
{
    return true;
}

