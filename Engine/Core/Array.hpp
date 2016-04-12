#pragma once

#include "Core/Types.hpp"
#include "Core/Allocator.hpp"

template <class T, typename TAlloc = CAllocator<T> >
class CArray
{
public:
    CArray():m_Data(0),m_Allocated(0),m_Used(0),m_FreeWhenDestroyed(true)
    {
		Reallocate(1);
    }

    CArray(u32 _size):m_Data(0),m_Allocated(0),m_Used(0),m_FreeWhenDestroyed(true)
    {
        Reallocate(_size);
    }

    ~CArray()
    {
        Clear();
    }

    void Reallocate(u32 _size)
    {
        T* old_data = m_Data;
        
        m_Data = m_Allocator.Allocate(_size);
        m_Allocated = _size;
        
        // copy old data
        s32 end = m_Used < _size ? m_Used : _size;
        
        for(s32 i=0; i<end; i++)
        {
            m_Allocator.Construct(&m_Data[i], old_data[i]);
        }
        
        // destruct old data
        for(u32 j=0; j<m_Used; j++)
            m_Allocator.Destruct(&old_data[j]);
        
        if(m_Allocated < m_Used)
            m_Used = m_Allocated;
        
        m_Allocator.Deallocate(old_data);
    }

	void Insert(const T& _element, u32 _index = -1)
    {
		if(_index > m_Used)
			_index = m_Used;
        
        if(m_Used + 1 > m_Allocated)
        {
			Reallocate(m_Used + 1);
            
            // move array content and construct new element
            // first move end one up
            for(u32 i = m_Used; i > _index; i--)
            {
                if(i < m_Used)
                    m_Allocator.Destruct(&m_Data[i]);
                m_Allocator.Construct(&m_Data[i], m_Data[i-1]); // data[i] = data[i-1];
            }

			const T e(_element);

            // then add new element
            if(m_Used > _index)
                m_Allocator.Destruct(&m_Data[_index]);
            m_Allocator.Construct(&m_Data[_index], e); // data[index] = e;
        }
        else
        {
            // element inserted not at end
            if(_index < m_Used)
            {
                // create one new element at the end
                m_Allocator.Construct(&m_Data[m_Used], m_Data[m_Used - 1]);
                
                // move the rest of the array content
                for(u32 i = m_Used - 1; i > _index; i--)
                {
                    m_Data[i] = m_Data[i - 1];
                }
                // insert the new element
                m_Data[_index] = _element;
            }
            else
            {
				const T e(_element);

                // insert the new element to the end
                m_Allocator.Construct(&m_Data[_index], e);
            }
        }

        m_Used++;
    }

	void Insert(const T* _array, u32 _count, u32 _index = -1)
    {
		if(_index > m_Used)
			_index = m_Used;
        
        if(m_Used + _count > m_Allocated)
        {
			Reallocate(m_Used + _count);
            
            // move array content and construct new element
            // first move end one up
            for(u32 i = m_Used; i > _index; i--)
            {
				u32 dst = i + _count - 1;
                if(dst < m_Used)
                    m_Allocator.Destruct(&m_Data[dst]);
                m_Allocator.Construct(&m_Data[dst], m_Data[i - 1]); // data[id] = data[i];
            }

            // then add new elements
			for(u32 i = 0; i < _count; i++)
			{
				u32 dst = _index + i;
				if(dst < m_Used)
					m_Allocator.Destruct(&m_Data[dst]);
				m_Allocator.Construct(&m_Data[dst], _array[i]); // data[index] = _array[i];
			}
        }
        else
        {
            // element inserted not at end
            if(_index < m_Used)
            {
                // create elements at the end
				for(u32 i = 0; i < _count; i++)
				{
					m_Allocator.Construct(&m_Data[m_Used + i], m_Data[m_Used + i - 1]);
				}
                
                // move the rest of the array content
                for(u32 i = m_Used - 1; i > _index; i--)
                {
                    m_Data[i + _count - 1] = m_Data[i - 1];
                }
                // insert the new elements
				for(u32 i = 0; i < _count; i++)
				{
					m_Data[_index + i] = _array[i];
				}
            }
            else
            {
                // insert the new elements to the end
				for(u32 i = 0; i < _count; i++)
				{
					m_Allocator.Construct(&m_Data[_index + i], _array[i]);
				}
            }
        }

		m_Used += _count;
    }

    void Clear()
    {
        if(m_FreeWhenDestroyed)
        {
            for(u32 i=0; i<m_Used; ++i)
                m_Allocator.Destruct(&m_Data[i]);
            
            m_Allocator.Deallocate(m_Data);
        }
        m_Data = 0;
        m_Used = 0;
        m_Allocated = 0;
    }

    void SetPointer(T* _pointer, u32 _size, bool _freeWhenDestroyed=true)
    {
        Clear();
        m_Data = _pointer;
        m_Allocated = _size;
        m_Used = _size;
        m_FreeWhenDestroyed = _freeWhenDestroyed;
    }

    void SetFreeWhenDestroyed(bool f)
    {
        m_FreeWhenDestroyed = f;
    }
    
    void SetUsed(u32 _used)
    {
        if(m_Allocated < _used)
            Reallocate(_used);
        
        m_Used = _used;
    }

    inline T& operator [](u32 _index)
    {
        return m_Data[_index];
    }

    T& GetLast()
    {
        return m_Data[m_Used-1];
    }

    T* Pointer()
    {
        return m_Data;
    }

    u32 Size() const
    {
        return m_Used;
    }

    u32 Allocated() const
    {
        return m_Allocated;
    }

    bool Empty() const
    {
        return m_Used == 0;
    }

    void Erase(u32 _index)
    {
        for (u32 i=_index+1; i<m_Used; ++i)
        {
            m_Allocator.Destruct(&m_Data[i-1]);
            m_Allocator.Construct(&m_Data[i-1], m_Data[i]);
        }
        
        m_Allocator.Destruct(&m_Data[m_Used-1]);
        
        --m_Used;
    }

    void Erase(u32 _index, s32 _count)
    {
        if(_index >= m_Used || _count < 1)
            return;
        if(_index + _count > m_Used)
            _count = m_Used - _index;
        
        u32 i;
        for(i=_index; i<_index+_count; ++i)
            m_Allocator.Destruct(&m_Data[i]);
        
        for (i=_index+_count; i<m_Used; ++i)
        {
            if(i > _index+_count)
                m_Allocator.Destruct(&m_Data[i-_count]);
            
            m_Allocator.Construct(&m_Data[i-_count], m_Data[i]);
            
            if(i >= m_Used-_count)
                m_Allocator.Destruct(&m_Data[i]);
        }
        
        m_Used -= _count;
    }

private:
	CArray(const CArray<T, TAlloc>& _other);

    T*      m_Data;
    u32     m_Allocated;
    u32     m_Used;
    TAlloc  m_Allocator;
    bool    m_FreeWhenDestroyed;
};

template <class T, typename TAlloc = CAllocator<T> >
class CPODArray
{
public:
    CPODArray():m_Data(0),m_Allocated(0),m_Used(0)
    {
		Reallocate(1);
    }

    CPODArray(u32 _size):m_Data(0),m_Allocated(0),m_Used(0)
    {
        Reallocate(_size);
    }

    ~CPODArray()
    {
        Clear();
    }

    void Reallocate(u32 _size)
    {
        T* old_data = m_Data;
        
        m_Data = m_Allocator.Allocate(_size);
        m_Allocated = _size;
        
        s32 size = m_Used < _size ? m_Used : _size;

		if(size)
			memcpy(m_Data, old_data, sizeof(T) * size);
        
        if(m_Allocated < m_Used)
            m_Used = m_Allocated;
        
        m_Allocator.Deallocate(old_data);
    }

	void Insert(const T& _element, u32 _index = -1)
    {
		if(_index > m_Used)
			_index = m_Used;
        
        if(m_Used + 1 > m_Allocated)
        {
			Reallocate(m_Used + 1);
		}
		if(_index < m_Used)
		{
			memmove(m_Data + _index + 1, m_Data + _index, (m_Used - _index) * sizeof(T));
		}

		m_Data[_index] = _element;

        m_Used++;
    }

	void Insert(const T* _array, u32 _count, u32 _index = -1)
    {
		if(_index > m_Used)
			_index = m_Used;
        
        if(m_Used + _count > m_Allocated)
        {
			Reallocate(m_Used + _count);
		}  
        
		if(_index < m_Used)
		{
			memmove(m_Data + _index + _count, m_Data + _index, (m_Used - _index + _count) * sizeof(T));
		}

		memcpy(m_Data + _index, _array, _count * sizeof(T));

		m_Used += _count;
    }

    void Clear()
    {
        m_Allocator.Deallocate(m_Data);

        m_Data = 0;
        m_Used = 0;
        m_Allocated = 0;
    }

    void SetPointer(T* _pointer, u32 _size)
    {
        Clear();

        m_Data = _pointer;
        m_Allocated = _size;
        m_Used = _size;
    }

    void SetUsed(u32 _used)
    {
        if(m_Allocated < _used)
            Reallocate(_used);
        
        m_Used = _used;
    }

    inline T& operator [](u32 _index)
    {
        return m_Data[_index];
    }

    T& GetLast()
    {
        return m_Data[m_Used-1];
    }

    T* Pointer()
    {
        return m_Data;
    }

    u32 Size() const
    {
        return m_Used;
    }

    u32 Allocated() const
    {
        return m_Allocated;
    }

    bool Empty() const
    {
        return m_Used == 0;
    }

    void Erase(u32 _index)
    {
		if(_index < m_Used)
		{
			memmove(m_Data + _index, m_Data + _index + 1, (m_Used - _index) * sizeof(T));
		}
        
        --m_Used;
    }

    void Erase(u32 _index, s32 _count)
    {
        if(_index + _count > m_Used)
            _count = m_Used - _index;
      
        if(_index < m_Used)
		{
			memmove(m_Data + _index, m_Data + _index + _count, (m_Used - _index + _count) * sizeof(T));
		}
        
        m_Used -= _count;
    }

private:
	CPODArray(const CArray<T, TAlloc>& _other);

    T*      m_Data;
    u32     m_Allocated;
    u32     m_Used;
    TAlloc  m_Allocator;
};