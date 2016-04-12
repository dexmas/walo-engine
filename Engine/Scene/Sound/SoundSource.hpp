#pragma once

#include "Scene/Component.hpp"

#include "Sound/SoundPlayer.hpp"
#include "Sound/SoundManager.hpp"
#include "Scene/Sound/SoundSystem.hpp"
#include "Core/LinkedList.hpp"
#include "Game.hpp"

class CSoundSource:public CComponentImpl<CSoundSystem>, public CLinkedListNode<CSoundSource>
{
	friend class CSoundSystem;
public:
	CSoundSource():CComponentImpl(ECT_SOUNDSOURCE)
	{
		m_SoundPlayer = CSoundManager::CreatePlayer();
	}

	~CSoundSource()
	{
		delete m_SoundPlayer;
	}

	void OnTransformed()
	{
		CMatrix4 mtrx = m_Node->GetTransform();
		CVector3 spos;

		mtrx.TransformVector(spos);

		m_SoundPlayer->SetPosition(spos);
	}

	void SetSound(CSoundStream* _sound)
	{
		m_SoundPlayer->SetSound(_sound);
	}

	void Play(bool _looped)
	{
		m_SoundPlayer->Play(_looped);
	}

	void Stop()
	{
		m_SoundPlayer->Stop();
	}

	void Pause()
	{
		m_SoundPlayer->Pause();
	}

	void Resume()
	{
		m_SoundPlayer->Resume();
	}

	void SetVolume(f32 _volume)
	{
		m_SoundPlayer->SetVolume(_volume);
	}

	void UpdateSound()
	{
		m_SoundPlayer->Update();
	}

private:
	CSoundPlayer* m_SoundPlayer;
	CSoundStream* m_SoundStream;
};