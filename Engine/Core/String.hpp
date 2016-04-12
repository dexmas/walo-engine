#pragma once

#include "Core/Types.hpp"
#include "Core/Math.hpp"
#include "Core/Allocator.hpp"

#include <stdio.h>

static inline u32 locale_lower(u32 _char)
{
	return _char >= 'A' && _char <= 'Z' ? _char + 0x20 : _char;
}

static inline u32 locale_upper(u32 _char)
{
	return _char >= 'a' && _char <= 'z' ? _char + ( 'A' - 'a' ) : _char;
}

template <typename T = char, typename TAlloc = CAllocator<T> >
class CStringClass
{
public:
	CStringClass()
	: m_Array(0), m_Allocated(1), m_Used(1)
	{
		m_Array = m_Allocator.Allocate(1);
		m_Array[0] = 0x0;
	}

	CStringClass(const CStringClass<T,TAlloc>& _other)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		*this = _other;
	}

	template <class B, class A>
	CStringClass(const CStringClass<B, A>& _other)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		*this = _other;
	}

	explicit CStringClass(const double _number)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		u8 tmpbuf[255];
        snprintf((char*)tmpbuf, 255, "%0.6f", _number);
		*this = tmpbuf;
	}

	explicit CStringClass(int _number)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		// store if negative and make positive
		bool negative = false;
		if(_number < 0)
		{
			_number *= -1;
			negative = true;
		}

		// temporary buffer for 16 numbers
		u8 tmpbuf[16]={0};
		u32 idx = 15;

		// special case '0'
		if(!_number)
		{
			tmpbuf[14] = '0';
			*this = &tmpbuf[14];
			return;
		}

		// add numbers
		while(_number && idx)
		{
			--idx;
			tmpbuf[idx] = (u8)('0' + (_number % 10));
			_number /= 10;
		}

		// add sign
		if(negative)
		{
			--idx;
			tmpbuf[idx] = '-';
		}

		*this = &tmpbuf[idx];
	}


	//! Constructs a string from an unsigned int
	explicit CStringClass(u32 _number)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		// temporary buffer for 16 numbers

		u8 tmpbuf[16]={0};
		u32 idx = 15;

		// special case '0'

		if (!_number)
		{
			tmpbuf[14] = '0';
			*this = &tmpbuf[14];
			return;
		}

		// add numbers

		while(_number && idx)
		{
			--idx;
			tmpbuf[idx] = (u8)('0' + (_number % 10));
			_number /= 10;
		}

		*this = &tmpbuf[idx];
	}

	template <class B>
	CStringClass(const B* const _buffer, u32 _length)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		if (!_buffer)
		{
			// correctly init the string to an empty one
			*this="";
			return;
		}

		m_Allocated = m_Used = _length+1;
		m_Array = m_Allocator.Allocate(m_Used); // new T[used];

		for(u32 l = 0; l<_length; ++l)
			m_Array[l] = (T)_buffer[l];

		m_Array[_length] = 0;
	}

	template <class B>
	CStringClass(const B* const _buffer)
	: m_Array(0), m_Allocated(0), m_Used(0)
	{
		*this = _buffer;
	}

	~CStringClass()
	{
		m_Allocator.Deallocate(m_Array);
	}

	CStringClass<T,TAlloc>& operator=(const CStringClass<T,TAlloc>& _other)
	{
		if(this == &_other)
			return *this;

		m_Used = _other.Size()+1;
		if(m_Used>m_Allocated)
		{
			m_Allocator.Deallocate(m_Array);
			m_Allocated = m_Used;
			m_Array = m_Allocator.Allocate(m_Used);
		}

		const T* p = _other.CStr();
		for(u32 i=0; i<m_Used; ++i, ++p)
			m_Array[i] = *p;

		return *this;
	}

	template <class B, class A>
	CStringClass<T,TAlloc>& operator=(const CStringClass<B,A>& _other)
	{
		*this = _other.CStr();
		return *this;
	}

	template <class B>
	CStringClass<T,TAlloc>& operator=(const B* const _buffer)
	{
		if(!_buffer)
		{
			if (!m_Array)
			{
				m_Array = m_Allocator.Allocate(1); //new T[1];
				m_Allocated = 1;
			}
			m_Used = 1;
			m_Array[0] = 0x0;
			return *this;
		}

		if ((void*)_buffer == (void*)m_Array)
			return *this;

		u32 len = 0;
		const B* p = _buffer;
		do
		{
			++len;
		} while(*p++);

		// we'll keep the old string for a while, because the new
		// string could be a part of the current string.
		T* oldArray = m_Array;

		m_Used = len;
		if(m_Used>m_Allocated)
		{
			m_Allocated = m_Used;
			m_Array = m_Allocator.Allocate(m_Used);
		}

		for(u32 l = 0; l<len; ++l)
			m_Array[l] = (T)_buffer[l];

		if (oldArray != m_Array)
			m_Allocator.Deallocate(oldArray);

		return *this;
	}

	CStringClass<T,TAlloc> operator+(const CStringClass<T,TAlloc>& _other) const
	{
		CStringClass<T,TAlloc> str(*this);
		str.Append(_other);

		return str;
	}

	template <class B>
	CStringClass<T,TAlloc> operator+(const B* const _buffer) const
	{
		CStringClass<T,TAlloc> str(*this);
		str.Append(_buffer);

		return str;
	}

	T& operator [](const u32 _index) const
	{
		return m_Array[_index];
	}

	//! Equality operator
	bool operator ==(const T* const _buffer) const
	{
		if(!_buffer)
			return false;

		u32 i;
		for(i=0; m_Array[i] && _buffer[i]; ++i)
			if(m_Array[i] != _buffer[i])
				return false;

		return !m_Array[i] && !_buffer[i];
	}


	//! Equality operator
	bool operator ==(const CStringClass<T,TAlloc>& _other) const
	{
		for(u32 i=0; m_Array[i] && _other.m_Array[i]; ++i)
			if (m_Array[i] != _other.m_Array[i])
				return false;

		return m_Used == _other.m_Used;
	}


	//! Is smaller comparator
	bool operator <(const CStringClass<T,TAlloc>& _other) const
	{
		for(u32 i=0; m_Array[i] && _other.m_Array[i]; ++i)
		{
			s32 diff = m_Array[i] - _other.m_Array[i];
			if(diff)
				return (diff < 0);
		}

		return m_Used < _other.m_Used;
	}

	bool operator !=(const T* const _buffer) const
	{
		return !(*this == _buffer);
	}

	bool operator !=(const CStringClass<T,TAlloc>& _other) const
	{
		return !(*this == _other);
	}

	u32 Size() const
	{
		return m_Used-1;
	}

	const T* CStr() const
	{
		return m_Array;
	}

	void ToLower()
	{
		for(u32 i=0; i<m_Used; ++i)
			m_Array[i] = locale_lower(m_Array[i]);
	}

	void ToUpper()
	{
		for(u32 i=0; i<m_Used; ++i)
			m_Array[i] = locale_upper(m_Array[i]);
	}

	bool EqualsIgnoreCase(const CStringClass<T,TAlloc>& _other) const
	{
		for(u32 i=0; m_Array[i] && _other[i]; ++i)
			if(locale_lower(m_Array[i]) != locale_lower(_other[i]))
				return false;

		return m_Used == _other.m_Used;
	}
	
	bool Equals(const CStringClass<T, TAlloc>& _other) const
	{
		return Size() == _other.Size() && !memcmp(CStr(), _other.CStr(), Size() * sizeof(T));
	}

	bool EqualsSubstringIgnoreCase(const CStringClass<T,TAlloc>& _other, const s32 _pos = 0 ) const
	{
		if((u32)_pos > m_Used)
			return false;

		u32 i;
		for( i=0; m_Array[_pos + i] && _other[i]; ++i)
			if(locale_lower(m_Array[_pos + i]) != locale_lower(_other[i]))
				return false;

		return m_Array[_pos + i] == 0 && _other[i] == 0;
	}

	bool LowerIgnoreCase(const CStringClass<T,TAlloc>& _other) const
	{
		for(u32 i=0; m_Array[i] && _other.m_Array[i]; ++i)
		{
			s32 diff = (s32)locale_lower(m_Array[i])-(s32)locale_lower(_other.m_Array[i]);
			if(diff)
				return (diff < 0);
		}

		return (m_Used < _other.m_Used);
	}

	bool EqualsN(const CStringClass<T,TAlloc>& _other, u32 _len) const
	{
		u32 i;
		for(i=0; m_Array[i] && _other[i] && i < _len; ++i)
			if(m_Array[i] != _other[i])
				return false;

		// if one (or both) of the strings was smaller then they
		// are only equal if they have the same length
		return (i == _len) || (m_Used == _other.m_Used);
	}

	bool EqualsN(const T* const _buffer, u32 _len) const
	{
		if(!_buffer)
			return false;
		u32 i;
		for(i=0; m_Array[i] && _buffer[i] && i < _len; ++i)
			if(m_Array[i] != _buffer[i])
				return false;

		// if one (or both) of the strings was smaller then they
		// are only equal if they have the same length
		return (i == _len) || (m_Array[i] == 0 && _buffer[i] == 0);
	}

	void Append(T _char)
	{
		if(m_Used + 1 > m_Allocated)
			Reallocate(m_Used + 1);

		++m_Used;

		m_Array[m_Used-2] = _char;
		m_Array[m_Used-1] = 0;
	}

	void Append(const T* const _other)
	{
		if (!_other)
			return;

		u32 len = 0;
		const T* p = _other;
		while(*p)
		{
			++len;
			++p;
		}

		if(m_Used + len > m_Allocated)
			Reallocate(m_Used + len);

		--m_Used;
		++len;

		for(u32 l=0; l<len; ++l)
			m_Array[l+m_Used] = *(_other+l);

		m_Used += len;
	}

	void Append(const CStringClass<T,TAlloc> & _other)
	{
		--m_Used;
		u32 len = _other.Size()+1;

		if(m_Used + len > m_Allocated)
			Reallocate(m_Used + len);

		for(u32 l=0; l<len; ++l)
			m_Array[m_Used+l] = _other[l];

		m_Used += len;
	}

	void Append(const CStringClass<T,TAlloc> & _other, u32 _length)
	{
		if(_other.Size() < _length)
		{
			Append(_other);
			return;
		}

		if(m_Used + _length > m_Allocated)
			Reallocate(m_Used + _length);

		--m_Used;

		for(u32 l=0; l<_length; ++l)
			m_Array[l+m_Used] = _other[l];
		m_Used += _length;

		// ensure proper termination
		m_Array[m_Used]=0;
		++m_Used;
	}

	void Reserve(u32 _count)
	{
		if(_count < m_Allocated)
			return;

		Reallocate(_count);
	}

	s32 FindFirst(T _char) const
	{
		for(u32 i=0; i<m_Used; ++i)
			if(m_Array[i] == _char)
				return i;

		return -1;
	}

	s32 FindFirstChar(const T* const _buffer, u32 _count) const
	{
		if (!_buffer)
			return -1;

		for (u32 i=0; i<m_Used; ++i)
			for (u32 j=0; j<_count; ++j)
				if (m_Array[i] == _buffer[j])
					return i;

		return -1;
	}

	template <class B>
	s32 FindFirstCharNotInList(const B* const _buffer, u32 _count) const
	{
		for(u32 i=0; i<m_Used-1; ++i)
		{
			u32 j;
			for (j=0; j<_count; ++j)
				if(m_Array[i] == _buffer[j])
					break;

			if (j==_count)
				return i;
		}

		return -1;
	}

	template <class B>
	s32 FindLastCharNotInList(const B* const _buffer, u32 _count) const
	{
		for(s32 i=(s32)(m_Used-2); i>=0; --i)
		{
			u32 j;
			for (j=0; j<_count; ++j)
				if(m_Array[i] == _buffer[j])
					break;

			if(j==_count)
				return i;
		}

		return -1;
	}

	s32 FindNext(T _char, u32 _start) const
	{
		for(u32 i=_start; i<m_Used; ++i)
			if(m_Array[i] == _char)
				return i;

		return -1;
	}

	s32 FindLast(T _char, s32 _start = -1) const
	{
		_start = CMath::Clamp(_start < 0 ? (s32)(m_Used) - 1 : _start, 0, (s32)(m_Used) - 1);
		for(s32 i=_start; i>=0; --i)
			if(m_Array[i] == _char)
				return i;

		return -1;
	}

	s32 FindLastChar(const T* const _buffer, u32 _count) const
	{
		if (!_buffer)
			return -1;

		for(s32 i=m_Used-1; i>=0; --i)
			for (u32 j=0; j<_count; ++j)
				if(m_Array[i] == _buffer[j])
					return i;

		return -1;
	}

	template <class B>
	s32 Find(const B* const _str, const u32 _start = 0) const
	{
		if(_str && *_str)
		{
			u32 len = 0;

			while(_str[len])
				++len;

			if(len > m_Used-1)
				return -1;

			for(u32 i=_start; i<m_Used-len; ++i)
			{
				u32 j=0;

				while(_str[j] && m_Array[i+j] == _str[j])
					++j;

				if(!_str[j])
					return i;
			}
		}

		return -1;
	}

	CStringClass<T,TAlloc> SubString(u32 _begin, s32 _length) const
	{
		// if start after string
		// or no proper substring length
		if ((_length <= 0) || (_begin>=Size()))
			return CStringClass<T,TAlloc>("");
		// clamp length to maximal value
		if ((_length+_begin) > Size())
			_length = Size()-_begin;

		CStringClass<T,TAlloc> o;
		o.Reserve(_length+1);

		for(s32 i=0; i<_length; ++i)
			o.m_Array[i] = m_Array[i+_begin];

		o.m_Array[_length] = 0;
		o.m_Used = o.m_Allocated;

		return o;
	}

	CStringClass<T,TAlloc>& operator += (T _char)
	{
		Append(_char);
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const T* const _buffer)
	{
		Append(_buffer);
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const CStringClass<T,TAlloc>& _other)
	{
		Append(_other);
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const int _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const unsigned int _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const long _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const unsigned long& _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const double _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	CStringClass<T,TAlloc>& operator += (const float _i)
	{
		Append(CStringClass<T,TAlloc>(_i));
		return *this;
	}

	void Replace(T _toReplace, T _replaceWith)
	{
		for(u32 i=0; i<m_Used; ++i)
			if(m_Array[i] == _toReplace)
				m_Array[i] = _replaceWith;
	}

	void Remove(T _char)
	{
		u32 pos = 0;
		u32 found = 0;
		for(u32 i=0; i<m_Used; ++i)
		{
			if(m_Array[i] == _char)
			{
				++found;
				continue;
			}

			m_Array[pos++] = m_Array[i];
		}
		m_Used -= found;
		m_Array[m_Used-1] = 0;
	}

	void RemoveFirst(const CStringClass<T,TAlloc>& _str)
	{
		u32 size = _str.Size();
		if(size == 0)
			return;

		for(u32 i=0; i<m_Used; ++i)
		{
			u32 j = 0;
			while(j < size)
			{
				if(m_Array[i + j] != _str[j])
					break;
				++j;
			}
			if(j == size)
			{
				while (i + j < m_Used)
                {
					m_Array[i] = m_Array[i + 1 + j];
                    i++;
                }
				break;
			}
		}
		m_Used -= size;
		m_Array[m_Used-1] = 0;
	}

	void Remove(const CStringClass<T,TAlloc>& _str)
	{
		u32 size = _str.Size();
		if(size == 0)
			return;
		u32 pos = 0;
		u32 found = 0;
		for(u32 i=0; i<m_Used; ++i)
		{
			u32 j = 0;
			while(j < size)
			{
				if(m_Array[i + j] != _str[j])
					break;
				++j;
			}
			if(j == size)
			{
				found += size;
				i += size - 1;
				continue;
			}

			m_Array[pos++] = m_Array[i];
		}
		m_Used -= found;
		m_Array[m_Used-1] = 0;
	}

	void RemoveChars(const CStringClass<T,TAlloc> & _characters)
	{
		u32 pos = 0;
		u32 found = 0;
		for(u32 i=0; i<m_Used; ++i)
		{
			// Don't use characters.findFirst as it finds the \0,
			// causing used to become incorrect.
			bool docontinue = false;
			for (u32 j=0; j<_characters.Size(); ++j)
			{
				if(_characters[j] == m_Array[i])
				{
					++found;
					docontinue = true;
					break;
				}
			}
			if(docontinue)
				continue;

			m_Array[pos++] = m_Array[i];
		}
		m_Used -= found;
		m_Array[m_Used-1] = 0;
	}

	CStringClass<T,TAlloc>& Trim(const CStringClass<T,TAlloc> & _whitespace = " \t\n\r")
	{
		// find start and end of the substring without the specified characters
		const s32 begin = FindFirstCharNotInList(_whitespace.CStr(), _whitespace.m_Used);
		if(begin == -1)
			return (*this="");

		const s32 end = FindLastCharNotInList(_whitespace.CStr(), _whitespace.m_Used);

		return (*this = SubString(begin, (end +1) - begin));
	}

	void Erase(u32 _index)
	{
		for(u32 i=_index+1; i<m_Used; ++i)
			m_Array[i-1] = m_Array[i];

		--m_Used;
	}

	void Clear()
	{
		m_Used = 0;
	}

	void Validate()
	{
		// terminate on existing null
		for(u32 i=0; i<m_Allocated; ++i)
		{
			if(m_Array[i] == 0)
			{
				m_Used = i + 1;
				return;
			}
		}

		// terminate
		if(m_Allocated > 0)
		{
			m_Used = m_Allocated;
			m_Array[m_Used-1] = 0;
		}
		else
		{
			m_Used = 0;
		}
	}

	T LastChar() const
	{
		return m_Used > 1 ? m_Array[m_Used-2] : 0;
	}

	template<class TContainer>
	u32 Split(TContainer& _ret, const T* const _buffer, u32 _count=1, bool _ignoreEmptyTokens=true, bool _keepSeparators=false) const
	{
		if (!_buffer)
			return 0;

		const u32 oldSize=_ret.Size();
		u32 lastpos = 0;
		bool lastWasSeparator = false;
		for(u32 i=0; i<m_Used; ++i)
		{
			bool foundSeparator = false;
			for (u32 j=0; j<_count; ++j)
			{
				if (m_Array[i] == _buffer[j])
				{
					if ((!_ignoreEmptyTokens || i - lastpos != 0) &&
							!lastWasSeparator)
						_ret.PushBack(CStringClass<T,TAlloc>(&m_Array[lastpos], i - lastpos));
					foundSeparator = true;
					lastpos = (_keepSeparators ? i : i + 1);
					break;
				}
			}
			lastWasSeparator = foundSeparator;
		}
		if((m_Used - 1) > lastpos)
			_ret.PushBack(CStringClass<T,TAlloc>(&m_Array[lastpos], (m_Used - 1) - lastpos));
		return _ret.Size()-oldSize;
	}

private:

	//! Reallocate the array, make it bigger or smaller
	void Reallocate(u32 _size)
	{
		T* old_array = m_Array;

		m_Array = m_Allocator.Allocate(_size);
		m_Allocated = _size;

		u32 amount = m_Used < _size ? m_Used : _size;
		for(u32 i=0; i<amount; ++i)
			m_Array[i] = old_array[i];

		if(m_Allocated < m_Used)
			m_Used = m_Allocated;

		m_Allocator.Deallocate(old_array);
	}

	T*		m_Array;
	u32		m_Allocated;
	u32		m_Used;
	TAlloc  m_Allocator;
};

typedef CStringClass<char>		CString;
typedef CStringClass<wchar_t>	CWString;

CString UTF16to8(const CWString& _text);
CWString UTF8to16(const CString& _text);