#pragma once

#include "Scene/System.hpp"
#include "Scene/Node.hpp"
#include "Core/LinkedList.hpp"
#include "Sound/SoundManager.hpp"

namespace tthread
{
class thread;
class mutex;
}

class CSoundSource;

class CSoundSystem: public CSystem, public CLinkedList<CSoundSource>
{
public:
	static const u32 cSoundThreadSleep = 50;

	CSoundSystem();
	~CSoundSystem();

	CSoundManager* GetSoundManager();

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);

	void UpdateSound();
	void OnSuspend();
	void OnResume();

private:
	
	tthread::thread* m_Thread;
	tthread::mutex* m_Mutex;
	CSoundManager* m_SoundManager;

	CSoundSource* m_Iterator;
};