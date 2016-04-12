#include "Sound/SoundStream.hpp"
#include "Core/Log.hpp"

#include <vorbis/vorbisfile.h>

#pragma pack (push, 1)
struct SWavFormatHeader
{
	u16 FormatTag; //Encoding
	u16 Channels; //Channels
	u32 SamplesPerSec; //Frequency
	u32 AvgBytesPerSec; //Byterate
	u16 BlockAlign; //BlockAlign
	u16 BitsPerSample; //BitsPerSample
};
#pragma pack (pop)

class CSoundStreamWAV: public CSoundStream
{
public:
	bool Load(CStream* _stream)
	{
		CSoundStream::Load(_stream);

		m_DataOffset = 0;

		s32 magic  = 0;
		s32 length = 0;
		s32 magic2 = 0;

		m_DataStream->Read(&magic, 4);
		m_DataStream->Read(&length, 4);
		m_DataStream->Read(&magic2, 4);

		if(magic != RIFF || magic2 != WAVE)
		{
			LOG("CSoundStreamWAV Error: Wrong wav format.\n");
			return false;
		}

		while(m_DataStream->Pos() < m_DataStream->Size())
		{
			m_DataStream->Read(&magic, 4);
			m_DataStream->Read(&length, 4);

			if(magic == FMT)
			{
				SWavFormatHeader header;
				m_DataStream->Read(&header, sizeof(SWavFormatHeader));
				s32 dsize = length - sizeof(SWavFormatHeader);

				if(dsize > 0)
					m_DataStream->Seek(dsize, true);

				if(header.FormatTag != 1)
				{
					LOG("CSoundStreamWAV Error: Unsuported WAV compression.\n");
					return false;
				}
				else
				if(header.Channels < 1 || header.SamplesPerSec == 0)
				{
					LOG("CSoundStreamWAV Error: Wrong wav format.\n");
					return false;
				}

				m_ChannelsCount = header.Channels;
				m_SamplesPerSec = header.SamplesPerSec;
				m_BitsPerSample = header.BitsPerSample;
			}
			else
			if (magic == DATA)
			{
				m_DataOffset  = m_DataStream->Pos();
				m_DataSize = length;

				break;
			}
			else
				m_DataStream->Seek(length, true);
		}

		if(!m_DataSize)
		{
			LOG("CSoundStreamWAV Error: Wrong wav format.\n");
			return false;
		}

		return true;
	}

	u32 Read(char* _data, u32 _size = -1)
	{
		s32	bytesLeft = m_DataSize - (m_DataStream->Pos() - m_DataOffset);

		if(_size > bytesLeft)
			_size = bytesLeft;

		m_DataStream->Read(_data, _size);

		return _size;
	}

	void Seek(u32 _pos)
	{
		m_DataStream->Seek(m_DataOffset + (int)(_pos * m_ChannelsCount * m_SamplesPerSec * m_BitsPerSample/2), false);
	}

	u32 Pos()
	{
		s32 pos = m_DataStream->Pos() - m_DataOffset;
		return pos < 0 ? 0 : pos;
	}

private:
	enum
	{
		RIFF = 0x46464952,
		WAVE = 0x45564157,
		FMT  = 0x20746D66,
		DATA = 0x61746164,
	};

	u32 m_DataOffset;
};

class CSoundStreamOGG: public CSoundStream
{
public:
	~CSoundStreamOGG()
	{
		ov_clear(&m_OggFile);
	}

	bool Load(CStream* _stream)
	{
		CSoundStream::Load(_stream);

		ov_callbacks	cb;

		cb.read_func  = _ReadFunc;
		cb.seek_func  = _SeekFunc;
		cb.close_func = _CloseFunc;
		cb.tell_func  = _TellFunc;

		if(ov_open_callbacks(m_DataStream, &m_OggFile, NULL, 0, cb) < 0)
		{
			LOG("CSoundStreamOGG: Can't open .ogg file.\n");
			return false;
		}

		m_VorbisInfo  = ov_info(&m_OggFile, -1);
		m_BitsPerSample = 16;
		m_ChannelsCount = m_VorbisInfo->channels;
		m_SamplesPerSec = m_VorbisInfo->rate;

		double	totalTime = ov_time_total(&m_OggFile, -1);
		m_DataSize = u32(totalTime * m_ChannelsCount * m_SamplesPerSec * 2);
		m_CurrentPos = 0;

		return true;
	}

	u32 Read(char* _data, u32 _size = -1)
	{
		if(_size > m_DataSize)
			_size = m_DataSize;

		int	curSection, res;
		s32	bytesRead = 0;

		while(bytesRead < _size)
		{
			res = ov_read(&m_OggFile, (char*)(_data + bytesRead), _size - bytesRead, 0, 2, 1, &curSection);

			if(res <= 0)
				break;

			bytesRead += res;
		}

		m_CurrentPos += bytesRead;

		return bytesRead;
	}

	void Seek(u32 _pos)
	{
		m_CurrentPos = _pos;
		ov_time_seek(&m_OggFile, _pos);
	}

	u32 Pos()
	{
		return m_CurrentPos;
	}

private:
	static size_t _ReadFunc(void* _ptr, size_t _size, size_t _nmemb, void* _datasource)
	{
		CStream* stream = (CStream*)_datasource;

		if (stream->Pos() >= stream->Size())
			return 0;

		stream->Read(_ptr, _size * _nmemb);

		return (size_t)(_size * _nmemb);
	}

	static int _SeekFunc(void* _datasource, ogg_int64_t _offset, int _whence)
	{
		CStream* stream = (CStream*)_datasource;

		if (_whence == SEEK_SET)
			stream->Seek((int)_offset, false);
		else
		if (_whence == SEEK_CUR)
			stream->Seek((int)_offset, true);
		else
		if (_whence == SEEK_END)
			stream->Seek(stream->Size() - (int)_offset, false);

		return (int)stream->Pos();
	}

	static int _CloseFunc(void* _datasource)
	{
		return 0;
	}

	static long _TellFunc(void* _datasource)
	{
		CStream* stream = (CStream*)_datasource;

		return stream->Pos();
	}

	OggVorbis_File	m_OggFile;
	vorbis_info*	m_VorbisInfo;

	u32 m_CurrentPos;
};

CSoundStream::CSoundStream():CAsset(EAT_SOUND)
{
	m_DataStream = 0;
	m_DataSize = 0;
	m_ChannelsCount = 0;
	m_SamplesPerSec = 0;
	m_BitsPerSample = 0;
}

CSoundStream* CSoundStream::CreateStream(ESoundFormat _fmt)
{
	if(_fmt == ESF_WAV)
	{
		return new CSoundStreamWAV();
	}
	else
	if(_fmt == ESF_OGG)
	{
		return new CSoundStreamOGG();
	}
	else
	{
		return 0;
	}
}

bool CSoundStream::Load(CStream* _stream)
{
	m_DataStream = _stream;
	return true;
}

void CSoundStream::Unload()
{
	m_DataStream = 0;
}