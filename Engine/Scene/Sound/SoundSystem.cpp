#include "Scene/Sound/SoundSystem.hpp"
#include "Scene/Sound/SoundSource.hpp"

static void* _ThreadRoutine(void* _param)
{
	CSoundSystem* sSystem = (CSoundSystem*)_param;
	while(1)
	{
		sSystem->UpdateSound();

#ifdef WALO_PLATFORM_WIN
		Sleep(CSoundSystem::cSoundThreadSleep);
#else
		usleep(CSoundSystem::cSoundThreadSleep * 1000);
#endif
	}
}

CSoundSystem::CSoundSystem()
{
	m_SoundManager = CSoundManager::CreateManager();
	m_SoundUpdateThread.Start(&_ThreadRoutine, this);
	m_Iterator = 0;
}

CSoundSystem::~CSoundSystem()
{
	m_SoundUpdateThread.Stop();

	if(m_SoundManager)
	{
		delete m_SoundManager;
	}
}

CSoundManager* CSoundSystem::GetSoundManager() 
{
	return m_SoundManager;
}

void CSoundSystem::AddComponent(CComponent* _comp)
{
	CSoundSource* snd = (CSoundSource*)_comp;
	PushBack(snd);
}

void CSoundSystem::RemoveComponent(CComponent* _comp)
{
	CSoundSource* snd = (CSoundSource*)_comp;

	snd->Stop();

	Remove(snd);

	if(m_Iterator && snd == m_Iterator)
	{
		m_Iterator = m_Iterator->GetNext();
	}
}

void CSoundSystem::UpdateSound()
{
	m_Iterator = GetHead();

	while(m_Iterator)
	{
		m_Iterator->UpdateSound();

		if(m_Iterator)
		{
			m_Iterator = m_Iterator->GetNext();
		}
	}
}