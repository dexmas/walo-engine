#pragma once

#include <Filesystem/FileSystem.hpp>
#include <Core/String.hpp>

#include "../Utils/Serializer.hpp"

#include <sqrat.h>

class CSaveFileWrapper
{
public:
	CSaveFileWrapper()
	{
		m_Stream = 0;
	}

	CSaveFileWrapper(const char* _fname, bool _write)
	{
		m_Stream = CFileSystem::Instance()->OpenSaveFile(_fname, _write);
	}

	~CSaveFileWrapper()
	{
		delete m_Stream;
	}

	const Sqrat::Object Read()
	{
		HSQOBJECT obj;

		if(!m_Stream || !m_Stream->Size())
		{
			sq_pushnull(Sqrat::DefaultVM::Get());
			sq_getstackobj(Sqrat::DefaultVM::Get(), -1, &obj);
			Sqrat::Object ret(obj, Sqrat::DefaultVM::Get());
			sq_pop(Sqrat::DefaultVM::Get(), 1);

			return ret;
		}

		char* buffer = new char[m_Stream->Size()];

		m_Stream->Read(buffer, m_Stream->Size());
 
		if(!CSerializer::Deserealize(CString(buffer), Sqrat::DefaultVM::Get(), false))
		{
			sq_pushnull(Sqrat::DefaultVM::Get());
			sq_getstackobj(Sqrat::DefaultVM::Get(), -1, &obj);
			Sqrat::Object ret(obj, Sqrat::DefaultVM::Get());
			sq_pop(Sqrat::DefaultVM::Get(), 1);

			return ret;
		}
		
		sq_getstackobj(Sqrat::DefaultVM::Get(), -1, &obj);
		Sqrat::Object ret(obj, Sqrat::DefaultVM::Get());
		sq_pop(Sqrat::DefaultVM::Get(), 1);

		delete buffer;

		return ret;
	}

	bool Write(const Sqrat::Object& _obj)
	{
		if(m_Stream)
		{
			CString str;

			if(!CSerializer::Serealize(_obj, str, false))
			{
				return false;
			}

			if(m_Stream->Write((void*)str.CStr(), str.Size() + 1) == str.Size())
				return true;
		}

		return false;
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::ImprovedClass<CSaveFileWrapper, Sqrat::ImprovedAllocator<CSaveFileWrapper> > cl(_vm);

		cl.Ctor<char*, bool>();
		cl.Func("Read", &CSaveFileWrapper::Read);
		cl.Func("Write", &CSaveFileWrapper::Write);

		Sqrat::RootTable(_vm).Bind("CSaveFile", cl);
	}

private:
	CFileStream* m_Stream;
};