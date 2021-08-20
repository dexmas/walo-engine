#include "Scene/Sound/SoundSystem.hpp"
#include "Scene/Sound/SoundSource.hpp"

#include <tinythread.h>

static void _ThreadRoutine(void* _param)
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
	m_Thread = new tthread::thread(_ThreadRoutine, this);
	m_Mutex = new tthread::mutex();
	m_Iterator = 0;
}

CSoundSystem::~CSoundSystem()
{
 	if (m_Thread->joinable())
	{
 		m_Thread->join();
	}

	delete m_Thread;
	delete m_Mutex;
	delete m_SoundManager;
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
	tthread::lock_guard<tthread::mutex> guard(*m_Mutex);

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

void CSoundSystem::OnSuspend()
{
	tthread::lock_guard<tthread::mutex> guard(*m_Mutex);

	m_Iterator = GetHead();

	while (m_Iterator)
	{
		m_Iterator->Pause();

		if (m_Iterator)
		{
			m_Iterator = m_Iterator->GetNext();
		}
	}
}

void CSoundSystem::OnResume()
{
	tthread::lock_guard<tthread::mutex> guard(*m_Mutex);

	m_Iterator = GetHead();

	while (m_Iterator)
	{
		m_Iterator->Resume();

		if (m_Iterator)
		{
			m_Iterator = m_Iterator->GetNext();
		}
	}
}