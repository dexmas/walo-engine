#pragma once

#include "Sound/SoundManager.hpp"

#if defined(WALO_PLATFORM_IOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#ifdef _DEBUG
	static void TestALError()
	{
        ALenum errCode = alGetError();
        
		if(errCode != AL_NO_ERROR)
		{
			LOG("! OpenAL Error (%s).\n", (char*)alGetString(errCode));
		}
	}
	#define TEST_AL_ERROR TestALError
#else
	#define TEST_AL_ERROR()
#endif

#include "Sound/SoundAL/SoundPlayerAL.hpp"

class CSoundManagerAL: public CSoundManager
{
public:
	CSoundManagerAL()
	{
		m_ALDevice  = 0;
		m_ALContext = 0;

		m_ALDevice = alcOpenDevice(0);
		if(!m_ALDevice)
		{
			LOG("CSoundManagerAL Error: No sound device.\n");
			return;
		}

		m_ALContext = alcCreateContext(m_ALDevice, 0);
		if(!m_ALContext)
		{
			LOG("CSoundManagerAL Error : No sound context.\n");
			return;
		}
		alcMakeContextCurrent(m_ALContext);

		TEST_AL_ERROR();

		const f32 fSoundSpeed = 343.0f;
		alDopplerFactor(1.0); // 1.2 = exaggerate the pitch shift by 20%
		alDopplerVelocity(fSoundSpeed); // m/s this may need to be scaled at some point

		f32 vector[6];

		// Initial orientation of the listener = direction + direction up
		vector[0] = 0.0;
		vector[1] = 0.0;
		vector[2] = -4.0;
		alListenerfv(AL_POSITION, vector);

		vector[2] = 0.0;
		alListenerfv(AL_VELOCITY, vector);

		vector[2] = -1.0;
		vector[3] = 0.0;
		vector[4] = 1.0;
		vector[5] = 0.0;
		alListenerfv(AL_ORIENTATION, vector);

		alListenerf(AL_GAIN, 1.0f);
		TEST_AL_ERROR();
	}

	~CSoundManagerAL()
	{
		TEST_AL_ERROR();

		m_ALContext = alcGetCurrentContext();
		m_ALDevice = alcGetContextsDevice(m_ALContext);
		alcMakeContextCurrent(0);
		alcDestroyContext(m_ALContext);

		if(m_ALDevice)
		{
			alcCloseDevice(m_ALDevice);
		}
	}

	void SetListenerVolume(f32 _volume)
	{
		alListenerf(AL_GAIN, _volume);
		TEST_AL_ERROR();
	}

	void SetListenerPosition(const CVector3& _pos)
	{
		ALfloat pos[] = {_pos.X, _pos.Y, _pos.Z};
		alListenerfv(AL_POSITION, pos);
		TEST_AL_ERROR();
	}

private:
	ALCdevice*  m_ALDevice;
	ALCcontext* m_ALContext;
};