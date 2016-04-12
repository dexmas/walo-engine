#pragma once

#include "Sound/SoundStream.hpp"
#include "Sound/SoundPlayer.hpp"

#include "Core/List.hpp"
#include "Core/Log.hpp"

class CSoundManager
{
public:
	virtual ~CSoundManager(){}

	virtual void SetListenerVolume(f32 _volume) = 0;
	virtual void SetListenerPosition(const CVector3& _pos) = 0;

	static CSoundPlayer* CreatePlayer();
	static CSoundManager* CreateManager();

private:
	static CSoundManager* m_Instance;
};