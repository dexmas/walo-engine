#pragma once

#include "Sound/SoundStream.hpp"
#include "Core/Vector.hpp"

enum EPlayerState
{
	EPS_PLAYING,
	EPS_PAUSED,
	EPS_STOPPED,

	EPS_COUNT
};

class CSoundManager;

class CSoundPlayer
{
public:
	static const u32 cCycleBufferSize = 65536;
	static const u32 cCycleBufferCount = 2;

	CSoundPlayer()
	{
		m_SoundStream = 0;
		m_StreamPos = 0;

		m_Streaming = false;

		m_Looping = false;
		m_Streaming = false;
		m_CycleBuffer = 0;
		m_CycleID = 0;
		m_State = EPS_STOPPED;
	}

	virtual ~CSoundPlayer()
	{
	}

	virtual void SetSound(CSoundStream* _sound)
	{
		Stop();

		m_SoundStream = _sound;
		
		if(m_SoundStream)
			m_Streaming = (m_SoundStream->Size() >= (cCycleBufferSize * cCycleBufferCount));
	}

	EPlayerState GetState()
	{
		return m_State;
	}

	virtual void Play(bool _loop) 
	{
		m_Looping = _loop;
		m_State = EPS_PLAYING;
	}

	virtual void Pause() 
	{
		m_State = EPS_PAUSED;
	}

	virtual void Resume() 
	{
		m_State = EPS_PLAYING;
	}

	virtual void Stop() 
	{
		m_State = EPS_STOPPED;
		m_Looping = false;
	}

	virtual void SetVolume(f32 _vol){}
	virtual void SetPosition(const CVector3& _pos){}

	virtual void Update(){}

protected:
	bool					m_Looping;
	bool					m_Streaming;
	char*					m_CycleBuffer;
	CSoundStream*			m_SoundStream;
	u32						m_StreamPos;
	u32						m_CycleID;

	EPlayerState			m_State;
};