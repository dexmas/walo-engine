#pragma once

#include "Scene/Component.hpp"
#include "Scene/Sound/SoundSystem.hpp"

class CSoundListener: public CComponentImpl<CSoundSystem>
{
public:
	CSoundListener():CComponentImpl(ECT_SOUNDLISTENER)
	{
	}
};