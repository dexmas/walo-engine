#include "Sound/SoundManager.hpp"

#include "Sound/SoundAL/SoundManagerAL.hpp"

CSoundManager* CSoundManager::m_Instance = 0;

CSoundManager* CSoundManager::CreateManager()
{
	return new CSoundManagerAL();
}

CSoundPlayer* CSoundManager::CreatePlayer()
{
	return new CSoundPlayerAL();
}