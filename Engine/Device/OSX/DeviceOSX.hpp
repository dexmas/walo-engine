#pragma once

#include "Device/Device.hpp"

class CDeviceOSX: public CDevice
{
public:
    CDeviceOSX();
    
    void SetCaption(const char* _str);
    
    bool Init();
    void Update();
    bool Flush();
    bool SetContext();
};
