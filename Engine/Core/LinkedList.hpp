#pragma once

template<class TNode>
class CLinkedListNode
{
public:
	CLinkedListNode()
	{
		m_Next = 0;
	}

	inline void SetNext(TNode* _next)
	{
		m_Next = _next;
	}

	inline TNode* GetNext()
	{
		return m_Next;
	}

private:
	TNode* m_Next;
};

template<class TNode>
class CLinkedList
{
public:
	CLinkedList()
	{
		m_Head = 0;
		m_Tail = 0;
		m_ItemCount = 0;
	}

	inline void PushBack(TNode* _item)
	{
		if (m_Tail)
		{
			m_Tail->SetNext(_item);
			m_Tail = _item;
			m_Tail->SetNext(0);
		}
		else
		{
			_item->SetNext(0);
			m_Head = _item;
			m_Tail = _item;
		}
		m_ItemCount++;
	}

	inline void PushFront(TNode* _item)
	{
		if (m_Head)
		{
			_item->SetNext(m_Head);
			m_Head = _item;
		}
		else
		{
			_item->SetNext(0);

			m_Head = _item;
			m_Tail = _item;
		}
		m_ItemCount++;
	}

	inline void Remove(TNode* _item)
	{
		TNode* node = m_Head;
		TNode* prev = 0;
		while (node)
		{
			if (node == _item)
			{
				TNode* next = node->GetNext();

				if (next)
				{
					if (prev)
					{
						prev->SetNext(next);
					}
					else
					{
						m_Head = next;
					}
				}
				else
				{
					if (prev)
					{
						m_Tail = prev;
						m_Tail->SetNext(0);
					}
					else
					{
						m_Tail = 0;
						m_Head = 0;
					}
				}

				m_ItemCount--;

				return;
			}

			prev = node;
			node = node->GetNext();
		}
	}

	void Clean()
	{
		m_Head = 0;
		m_Tail = 0;
	}

	TNode* GetHead()
	{
		return m_Head;
	}

private:
	TNode* m_Head;
	TNode* m_Tail;

	s32 m_ItemCount;
};