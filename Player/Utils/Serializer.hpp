#pragma once

#include "Core/String.hpp"
#include "Core/Log.hpp"

#include <sqrat.h>

#ifdef GetObject
#	undef GetObject
#endif

class CSerializer
{
public:
	static bool Serealize(const Sqrat::Object& _object, CString& _output, bool _encryption = false)
	{
		if(_object.GetType() == OT_TABLE)
		{
			sq_pushobject(_object.GetVM(), _object.GetObject());
			_IterateWriteTable(_object.GetVM(), _output);
			sq_pop(_object.GetVM(), 1);
		}
		else
		if(_object.GetType() == OT_ARRAY)
		{
			sq_pushobject(_object.GetVM(), _object.GetObject());
			_IterateWriteArray(_object.GetVM(), _output);
			sq_pop(_object.GetVM(), 1);
		}
		else
		{
			LOG("SerealizeObject: Wrong type.\n");
			return false;
		}
        
        if (_encryption)
        {
            char * strToEncrypt = const_cast<char*>(_output.CStr());
            for (u32 i = 0; i < _output.Size(); ++i)
            {
                strToEncrypt[i] ^= 'Z';
            }
        }

		return true;
	}

	static bool Deserealize(const CString& _input, HSQUIRRELVM _vm, bool _encryption = false)
	{
        if (_encryption)
        {
            char * strToDecrypt = const_cast<char*>(_input.CStr());
            for (u32 i = 0; i < _input.Size(); ++i)
            {
                strToDecrypt[i] ^= 'Z';
            }
        }
		CString buffer = "return ";
		buffer.Append(_input);
		buffer.Append(";");

		if(SQ_FAILED(sq_compilebuffer(_vm, buffer.CStr(), buffer.Size() + 1, "", true)))
		{
			LOG("DeserealizeObject: Wrong format.\n");
			 return false;
		}

		sq_pushroottable(_vm);
		
		if(SQ_FAILED(sq_call(_vm, 1, SQTrue, SQTrue)))
		{
			LOG("DeserealizeObject: Script error.\n");
			 return false;
		}

		return true;
	}
private:
	static bool _IterateWriteTable(HSQUIRRELVM _vm, CString& _output)
	{
		if(sq_gettype(_vm, -1) != OT_TABLE)
			return false;

		_output.Append("{");

		sq_pushnull(_vm);						
		while(SQ_SUCCEEDED(sq_next(_vm, -2)))				
		{
			const SQChar* key_str;
			const SQChar* val_str;

			SQObjectType type = sq_gettype(_vm, -2);

			if(type == OT_INTEGER || type == OT_NULL || type == OT_FLOAT || type == OT_BOOL || type == OT_STRING)
			{
				if(SQ_FAILED(sq_tostring(_vm, -2)))
				{
					sq_pop(_vm, 2);
					continue;
				}

				if(SQ_FAILED(sq_getstring(_vm, -1, &key_str)))
				{
					sq_pop(_vm, 3);
					continue;
				}

				_output.Append(key_str);
				_output.Append("=");

				sq_pop(_vm, 1);
			}
			else
			{
				LOG("SerealizeTable: Invalid key table data.\n");
				sq_pop(_vm, 2);
				continue;
			}

			type = sq_gettype(_vm, -1);

			if(type == OT_INTEGER || type == OT_NULL || type == OT_FLOAT || type == OT_BOOL || type == OT_STRING)
			{
				if(SQ_FAILED(sq_tostring(_vm, -1)))
				{
					sq_pop(_vm, 2);
					continue;
				}

				if(SQ_FAILED(sq_getstring(_vm, -1, &val_str)))
				{
					sq_pop(_vm, 3);
					continue;
				}

				if(type == OT_STRING)
					_output.Append("\"");
				_output.Append(val_str);
				if(type == OT_STRING)
					_output.Append("\"");

				sq_pop(_vm, 1);
			}
			else
			if(type == OT_TABLE)
			{
				_IterateWriteTable(_vm, _output);
			}
			else
			if(type == OT_ARRAY)
			{
				_IterateWriteArray(_vm, _output);
			}
			else
			{
				LOG("SerealizeTable: Invalid value table data.\n");
				sq_pop(_vm, 2);
				continue;
			}

			_output.Append(",");
			sq_pop(_vm, 2);
		}
		sq_pop(_vm, 1);

		if(_output[_output.Size()-1] == ',')
			_output.Erase(_output.Size() - 1);
		_output.Append("}");

		return true;
	}

	static bool _IterateWriteArray(HSQUIRRELVM _vm, CString& _output)
	{
		if(sq_gettype(_vm, -1) != OT_ARRAY)
			return false;

		_output.Append("[");

		sq_pushnull(_vm);						
		while(SQ_SUCCEEDED(sq_next(_vm, -2)))				
		{
			const SQChar* val_str;

			SQObjectType type = sq_gettype(_vm, -2);

			if(type != OT_INTEGER)
			{
				LOG("SerealizeTable: Invalid array index data.\n");
				sq_pop(_vm, 2);
				continue;
			}

			type = sq_gettype(_vm, -1);

			if(type == OT_INTEGER || type == OT_NULL || type == OT_FLOAT || type == OT_BOOL || type == OT_STRING)
			{
				if(SQ_FAILED(sq_tostring(_vm, -1)))
				{
					sq_pop(_vm, 2);
					continue;
				}

				if(SQ_FAILED(sq_getstring(_vm, -1, &val_str)))
				{
					sq_pop(_vm, 3);
					continue;
				}

				if(type == OT_STRING)
					_output.Append("\"");
				_output.Append(val_str);
				if(type == OT_STRING)
					_output.Append("\"");

				sq_pop(_vm, 1);
			}
			else
			if(type == OT_TABLE)
			{
				_IterateWriteTable(_vm, _output);
			}
			else
			if(type == OT_ARRAY)
			{
				_IterateWriteArray(_vm, _output);
			}
			else
			{
				LOG("SerealizeTable: Invalid value array data.\n");
				sq_pop(_vm, 2);
				continue;
			}

			_output.Append(",");
			sq_pop(_vm, 2);
		}
		sq_pop(_vm, 1);

		if(_output[_output.Size()-1] == ',')
			_output.Erase(_output.Size() - 1);
		_output.Append("]");

		return true;
	}

};