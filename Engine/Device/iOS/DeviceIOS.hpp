#pragma once

#include "Device/Device.hpp"

class CDeviceIOS: public CDevice
{
public:
    CDeviceIOS();
    
    bool Init();
    void Update();
    bool Flush();
    bool SetContext();
};