#include "Device/OSX/DeviceOSX.hpp"
#include "Device/OSX/AppDelegate.h"

#include <Cocoa/Cocoa.h>

CDeviceOSX::CDeviceOSX()
{
    m_Type = EDT_OSX;
}

void CDeviceOSX::SetCaption(const char* _str)
{
    AppDelegate *appDelegate = (AppDelegate *)[NSApp delegate];
    
    [appDelegate.Window setTitle:[NSString stringWithUTF8String:_str]];
}

bool CDeviceOSX::Init()
{
    m_Active = true;
    
    return true;
}

void CDeviceOSX::Update()
{

}

bool CDeviceOSX::Flush()
{
    return true;
}

bool CDeviceOSX::SetContext()
{
    return true;
}

