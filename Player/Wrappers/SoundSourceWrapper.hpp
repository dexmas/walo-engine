#pragma once

#include <Scene/Sound/SoundSource.hpp>
#include <sqrat.h>

class CSoundSourceWrapper: public CSoundSource
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CSoundSource, CComponent, Sqrat::NoCopy<CSoundSource> > cl(_vm);

		cl.Func("SetSound", &CSoundSourceWrapper::SetSound);
		cl.Func("Play", &CSoundSourceWrapper::Play);
		cl.Func("Stop", &CSoundSourceWrapper::Stop);
		cl.Func("Pause", &CSoundSourceWrapper::Pause);
		cl.Func("Resume", &CSoundSourceWrapper::Resume);
		cl.Func("SetVolume", &CSoundSourceWrapper::SetVolume);

		Sqrat::RootTable(_vm).Bind("CSoundSource", cl);
	}
};