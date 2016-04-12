#pragma once

#include "Core/Types.hpp"
#include "Core/Allocator.hpp"

struct SNode;

template <class T>
class CList
{
private:
	struct SNode
	{
		SNode(const T& _e): 
			Next(0), 
			Prev(0), 
			Element(_e) 
		{
		}

		SNode* Next;
		SNode* Prev;

		T Element;
	};
public:
	
	class CIterator
	{
		friend class CList<T>;

	public:
		CIterator():
			Current(0) 
		{
		}

		CIterator& operator ++ ()    {Current = Current?Current->Next:0; return *this;}
		CIterator& operator -- ()    {Current = Current?Current->Prev:0; return *this;}

		bool operator == (const CIterator& other) const {return Current == other.Current;}
		bool operator != (const CIterator& other) const {return Current != other.Current;}

		T & operator * () {return Current->Element;}
		T * operator ->() {return &(Current->Element);}

	private:
		explicit CIterator(SNode* _begin):
			Current(_begin)
		{
		}

		SNode* Current;
	};

	CList(): 
		m_First(0), 
		m_Last(0), 
		m_Size(0) 
	{
	}

	~CList()
	{
		Clear();
	}

	u32 Size() const
	{
		return m_Size;
	}

	void Clear()
	{
		while(m_First)
		{
			SNode * next = m_First->Next;

			m_Allocator.Destruct(m_First);
			m_Allocator.Deallocate(m_First);

			m_First = next;
		}

		m_Last = 0;
		m_Size = 0;
	}

	bool Empty() const
	{
		return (m_First == 0);
	}

	void PushBack(const T& _element)
	{
		SNode* node = m_Allocator.Allocate(1);
		m_Allocator.Construct(node, _element);

		++m_Size;

		if(m_First == 0)
			m_First = node;

		node->Prev = m_Last;

		if(m_Last != 0)
			m_Last->Next = node;

		m_Last = node;
	}

	void PushFront(const T& _element)
	{
		SNode* node = m_Allocator.Allocate(1);
		m_Allocator.Construct(node, _element);

		++m_Size;

		if(m_First == 0)
		{
			m_Last = node;
			m_First = node;
		}
		else
		{
			node->Next = m_First;
			m_First->Prev = node;
			m_First = node;
		}
	}

	CIterator Begin()
	{
		return CIterator(m_First);
	}

	CIterator End()
	{
		return CIterator(0);
	}

	CIterator Last()
	{
		return CIterator(m_Last);
	}

	CIterator Find(const T& _element)
	{
		CIterator it = Begin();

        while(it != End() && *it != _element)
            ++it;

        return it;
	}

	void InsertAfter(const CIterator& _it, const T& _element)
	{
		SNode* node = m_Allocator.Allocate(1);
		m_Allocator.Construct(node, _element);

		node->Next = _it.Current->Next;

		if(_it.Current->Next)
			_it.Current->Next->Prev = node;

		node->Prev = _it.Current;
		_it.Current->Next = node;
		++m_Size;

		if(_it.Current == m_Last)
			m_Last = node;
	}

	void InsertBefore(const CIterator& _it, const T& _element)
	{
		SNode* node = m_Allocator.Allocate(1);
		m_Allocator.Construct(node, _element);

		node->Prev = _it.Current->Prev;

		if(_it.Current->Prev)
			_it.Current->Prev->Next = node;

		node->Next = _it.Current;
		_it.Current->Prev = node;
		++m_Size;

		if(_it.Current == m_First)
			m_First = node;
	}

	CIterator Erase(CIterator& _it)
	{
		CIterator returnIterator(_it);
		++returnIterator;

		if(_it.Current == m_First)
		{
			m_First = _it.Current->Next;
		}
		else
		{
			_it.Current->Prev->Next = _it.Current->Next;
		}

		if(_it.Current == m_Last)
		{
			m_Last = _it.Current->Prev;
		}
		else
		{
			_it.Current->Next->Prev = _it.Current->Prev;
		}

		m_Allocator.Destruct(_it.Current);
		m_Allocator.Deallocate(_it.Current);

		_it.Current = 0;
		--m_Size;

		return returnIterator;
	}

	CIterator Erase(const T& _element)
    {
        CIterator it = Find(_element);
        if(it != End())
        {
            return Erase(it);
        }
        else
            return End();
    }

private:
	SNode*					 m_First;
	SNode*					 m_Last;
	u32						 m_Size;
	CAllocator<SNode>		 m_Allocator;
};