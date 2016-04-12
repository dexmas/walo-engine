#pragma once

#include "Sound/SoundPlayer.hpp"

class CSoundPlayerAL: public CSoundPlayer
{
public:
	CSoundPlayerAL()
	{
		m_SoundSourceID = 0;
		m_SoundFormat = 0;
		m_CycleBuffer = 0;

		for(u32 i=0; i<cCycleBufferCount; i++)
		{
			m_CycleBuffersID[i] = 0;
		}

		alGenSources(1, &m_SoundSourceID);
        
        TEST_AL_ERROR();
	}

	~CSoundPlayerAL()
	{
		alDeleteSources(1, &m_SoundSourceID);
        
		TEST_AL_ERROR();

		_DeleteBuffers();

		if(m_CycleBuffer)
			delete[] m_CycleBuffer;
	}

	void SetSound(CSoundStream* _sound)
	{
		Stop();

		_DeleteBuffers();
        
        CSoundPlayer::SetSound(_sound);

		if(!m_SoundStream)
			return;

		m_SoundFormat = _SoundFormat(m_SoundStream->GetChannelsCount(), m_SoundStream->GetBitsPerSample());
		
		if(m_Streaming)
		{
			if(!m_CycleBuffer)
				m_CycleBuffer = new char[cCycleBufferSize * cCycleBufferCount];

			alGenBuffers(cCycleBufferCount, m_CycleBuffersID);
			TEST_AL_ERROR();
		}
		else
		{
			u32 size = m_SoundStream->Size();

			if(!m_CycleBuffer)
				delete[] m_CycleBuffer;

			m_CycleBuffer = new char[size];

			m_SoundStream->Seek(0);
			m_SoundStream->Read(m_CycleBuffer, size);
			
			alGenBuffers(1, m_CycleBuffersID);
			alBufferData(m_CycleBuffersID[0], m_SoundFormat, m_CycleBuffer, size, m_SoundStream->GetSamplesPerSec());

			alSourcei(m_SoundSourceID, AL_BUFFER, m_CycleBuffersID[0]);

			TEST_AL_ERROR();

			delete[] m_CycleBuffer;
			m_CycleBuffer = 0;
		}
	}

	void Play(bool _loop) 
	{
		Stop();

		if(!m_SoundStream)
			return;

		m_Looping = _loop;

		if(m_Streaming)
		{
			m_SoundStream->Seek(0);

			for(u32 i=0; i<cCycleBufferCount; i++)
			{
				u32 offset = i * cCycleBufferSize;
				u32 size = m_SoundStream->Read(&m_CycleBuffer[offset], cCycleBufferSize);
				alBufferData(m_CycleBuffersID[i], m_SoundFormat, &m_CycleBuffer[offset], size, m_SoundStream->GetSamplesPerSec());
			}

			m_StreamPos = m_SoundStream->Pos();
			m_CycleID = 0;

			alSourceQueueBuffers(m_SoundSourceID, cCycleBufferCount, m_CycleBuffersID);
			
            TEST_AL_ERROR();
		}
		else
		{
			alSourcei(m_SoundSourceID, AL_LOOPING, m_Looping ? AL_TRUE : AL_FALSE);
            
            TEST_AL_ERROR();
		}

		alSourcePlay(m_SoundSourceID);
		TEST_AL_ERROR();

		m_State = EPS_PLAYING;
	}

	void Pause() 
	{
		if(m_State == EPS_PLAYING)
		{
			alSourcePause(m_SoundSourceID);
			
            TEST_AL_ERROR();

			m_State = EPS_PAUSED;
		}
	}

	void Resume() 
	{
		if(m_State == EPS_PAUSED)
		{
			alSourcePlay(m_SoundSourceID);
			
            TEST_AL_ERROR();

			m_State = EPS_PLAYING;
		}
	}

	void Stop() 
	{    
		if(m_State != EPS_STOPPED)
		{
			alSourceStop(m_SoundSourceID);
			alSourcei(m_SoundSourceID, AL_BUFFER, AL_NONE);
			
            TEST_AL_ERROR();

			if(m_Streaming)
			{
				ALint queued;
				alGetSourcei(m_SoundSourceID, AL_BUFFERS_QUEUED, &queued);
				alSourceUnqueueBuffers(m_SoundSourceID, queued, m_CycleBuffersID);
				TEST_AL_ERROR();
			}

			m_State = EPS_STOPPED;
			m_Looping = false;
		}
	}

	void SetVolume(f32 _vol)
	{
		alSourcef(m_SoundSourceID, AL_GAIN, _vol);
		TEST_AL_ERROR();
	}

	void SetPosition(const CVector3& _pos)
	{
		ALfloat pos[] = {_pos.X, _pos.Y, _pos.Z};
		alSourcefv(m_SoundSourceID, AL_POSITION, pos);
		TEST_AL_ERROR();
	}

	void Update()
	{
		if(m_State != EPS_PLAYING)
			return;

		ALint processed;
		alGetSourcei(m_SoundSourceID, AL_BUFFERS_PROCESSED, &processed);
		TEST_AL_ERROR();

		if(processed && !m_Streaming)
		{
			m_State = EPS_STOPPED;
			return;
		}

		bool complt = processed == cCycleBufferCount;
		while(processed--)
		{
			m_SoundStream->Seek(m_StreamPos);

			u32 offset = m_CycleID * cCycleBufferSize;
			u32 size = m_SoundStream->Read(&m_CycleBuffer[offset], cCycleBufferSize);

			if(size > 0)
			{
				alSourceUnqueueBuffers(m_SoundSourceID, 1, &m_CycleBuffersID[m_CycleID]);
				alBufferData(m_CycleBuffersID[m_CycleID], m_SoundFormat, &m_CycleBuffer[offset], size, m_SoundStream->GetSamplesPerSec());
				alSourceQueueBuffers(m_SoundSourceID, 1, &m_CycleBuffersID[m_CycleID]);
				TEST_AL_ERROR();

				if(size < cCycleBufferSize)
				{
					if(m_Looping)
						m_StreamPos = 0;
				}
				else
				{
					m_StreamPos = m_SoundStream->Pos();
				}
			}
			else
			{
				ALint queued;
				alGetSourcei(m_SoundSourceID, AL_BUFFERS_QUEUED, &queued);
				TEST_AL_ERROR();

				if(!queued)
				{
					if(m_Looping)
						m_StreamPos = 0;
					else
						Stop();
				}
			}

			m_CycleID = (m_CycleID + 1) % cCycleBufferCount;
		}

		if(complt)
		{
			alSourcePlay(m_SoundSourceID);
			TEST_AL_ERROR();
		}
	}

private:
	void _DeleteBuffers()
	{
		if(m_SoundStream)
		{
			if(m_Streaming)
				alDeleteBuffers(cCycleBufferCount, m_CycleBuffersID);
			else
				alDeleteBuffers(1, m_CycleBuffersID);

			for(u32 i=0; i<cCycleBufferCount; i++)
			{
				m_CycleBuffersID[i] = 0;
			}

			TEST_AL_ERROR();
		}
	}

	ALuint _SoundFormat(u32 _numChannels, u32 _bitsPerSample)
	{
		if(_numChannels == 1)
			return _bitsPerSample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;

		if(_numChannels == 2)
			return _bitsPerSample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
		
		if(_numChannels == 4)
			if(_bitsPerSample == 16)
				return alGetEnumValue("AL_FORMAT_QUAD16");
			else
				return alGetEnumValue("AL_FORMAT_QUAD8");
			
		if(_numChannels == 6)
			if(_bitsPerSample == 16)
				return alGetEnumValue("AL_FORMAT_51CHN16");
			else
				return alGetEnumValue("AL_FORMAT_51CHN8");
			
		return 0;
	}

	ALuint	m_SoundFormat;
	ALuint	m_SoundSourceID;
	ALuint	m_CycleBuffersID[cCycleBufferCount];
};