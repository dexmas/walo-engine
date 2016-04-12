#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "Core/Types.hpp"

template <class TKey, class TValue>
class CMap
{
private:
	template <class TRBKey, class TRBValue>
	class CRBTree
	{
	public:
		CRBTree(const TRBKey& _key, const TRBValue& _val)
			:LeftChild(0), RightChild(0), Parent(0), Key(_key),
				Value(_val), Red(true) {}

		void SetLeftChild(CRBTree* _item)
		{
			LeftChild=_item;
			if(_item)
				_item->Parent = this;
		}

		void SetRightChild(CRBTree* _item)
		{
			RightChild=_item;
			if(_item)
				_item->Parent = this;
		}

		bool IsRoot() const
		{
			return (Parent == 0);
		}

		bool IsLeftChild() const
		{
			return (Parent != 0) && (Parent->LeftChild==this);
		}

		bool IsRightChild() const
		{
			return (Parent!=0) && (Parent->RightChild==this);
		}

		bool IsLeaf() const
		{
			return (LeftChild==0) && (RightChild==0);
		}

		unsigned int GetLevel() const
		{
			if(IsRoot())
				return 1;
			else
				return Parent->GetLevel() + 1;
		}

		CRBTree*	LeftChild;
		CRBTree*	RightChild;

		CRBTree*	Parent;

		TRBKey		Key;
		TRBValue	Value;

		bool		Red;

	private:
		CRBTree();
	};

public:
	typedef CRBTree<TKey,TValue> TNode;

	//! Normal Iterator
	class CIterator
	{
	public:
		CIterator():Root(0),Cur(0){}

		// Constructor(Node*)
		CIterator(TNode* _root) : Root(_root)
		{
			Reset();
		}

		// Copy constructor
		CIterator(const CIterator& _src):Root(_src.Root), Cur(_src.Cur){}

		void Reset(bool _atLowest = true)
		{
			if(_atLowest)
				Cur = GetMin(Root);
			else
				Cur = GetMax(Root);
		}

		bool AtEnd() const
		{
			return Cur==0;
		}

		TNode* GetNode()
		{
			return Cur;
		}

		CIterator& operator=(const CIterator& _src)
		{
			Root = _src.Root;
			Cur = _src.Cur;
			return (*this);
		}

		void operator++(int)
		{
			Inc();
		}

		CIterator& operator++(void)
		{
			Inc();
			return *this;
		}

		void operator--(int)
		{
			Dec();
		}


		TNode* operator->()
		{
			return GetNode();
		}

		TNode& operator*()
		{
			return *Cur;
		}

	private:
		TNode* GetMin(TNode* _node)
		{
			while(_node && _node->LeftChild)
				_node = _node->LeftChild;
			return _node;
		}

		TNode* GetMax(TNode* _node)
		{
			while(_node && _node->RightChild)
				_node = _node->RightChild;
			return _node;
		}

		void Inc()
		{
			// Already at end?
			if(Cur==0)
				return;

			if(Cur->RightChild)
			{
				// If current node has a right child, the next higher node is the
				// node with lowest key beneath the right child.
				Cur = GetMin(Cur->RightChild);
			}
			else 
			if(Cur->IsLeftChild())
			{
				// No right child? Well if current node is a left child then
				// the next higher node is the parent
				Cur = Cur->Parent;
			}
			else
			{
				// Current node neither is left child nor has a right child.
				// Ie it is either right child or root
				// The next higher node is the parent of the first non-right
				// child (ie either a left child or the root) up in the
				// hierarchy. Root's parent is 0.
				while(Cur->IsRightChild())
					Cur = Cur->Parent;
				Cur = Cur->Parent;
			}
		}

		void Dec()
		{
			// Already at end?
			if(Cur==0)
				return;

			if(Cur->LeftChild)
			{
				// If current node has a left child, the next lower node is the
				// node with highest key beneath the left child.
				Cur = GetMax(Cur->LeftChild);
			}
			else 
			if(Cur->IsRightChild())
			{
				// No left child? Well if current node is a right child then
				// the next lower node is the parent
				Cur = Cur->Parent;
			}
			else
			{
				// Current node neither is right child nor has a left child.
				// Ie it is either left child or root
				// The next higher node is the parent of the first non-left
				// child (ie either a right child or the root) up in the
				// hierarchy. Root's parent is 0.

				while(Cur->IsLeftChild())
					Cur = Cur->Parent;
				Cur = Cur->Parent;
			}
		}

		TNode* Root;
		TNode* Cur;
	};

	class CParentFirstIterator
	{
	public:
		CParentFirstIterator():Root(0),Cur(0)
		{
		}

		explicit CParentFirstIterator(TNode* _root):Root(_root),Cur(0)
		{
			Reset();
		}

		void Reset()
		{
			Cur = Root;
		}


	bool AtEnd() const
	{
		return Cur==0;
	}

	TNode* GetNode()
	{
		return Cur;
	}


	CParentFirstIterator& operator=(const CParentFirstIterator& src)
	{
		Root = src.Root;
		Cur = src.Cur;
		return (*this);
	}


	void operator++(int)
	{
		Inc();
	}


	TNode* operator->()
	{
		return GetNode();
	}

	TNode& operator*()
	{
		return *GetNode();
	}

	private:

	void Inc()
	{
		// Already at end?
		if (Cur==0)
			return;

		// First we try down to the left
		if(Cur->GetLeftChild())
		{
			Cur = Cur->GetLeftChild();
		}
		else 
		if(Cur->GetRightChild())
		{
			// No left child? The we go down to the right.
			Cur = Cur->GetRightChild();
		}
		else
		{
			// No children? Move up in the hierarcy until
			// we either reach 0 (and are finished) or
			// find a right uncle.
			while(Cur!=0)
			{
				// But if parent is left child and has a right "uncle" the parent
				// has already been processed but the uncle hasn't. Move to
				// the uncle.
				if (Cur->IsLeftChild() && Cur->GetParent()->GetRightChild())
				{
					Cur = Cur->GetParent()->GetRightChild();
					return;
				}
				Cur = Cur->GetParent();
			}
		}
	}

	TNode* Root;
	TNode* Cur;

	}; // ParentFirstIterator

	class CParentLastIterator
	{
	public:
		CParentLastIterator():Root(0),Cur(0){}

		explicit CParentLastIterator(TNode* _root):Root(_root),Cur(0)
		{
			Reset();
		}

		void Reset()
		{
			Cur = GetMin(Root);
		}

		bool AtEnd() const
		{
			return Cur==0;
		}

		TNode* GetNode()
		{
			return Cur;
		}

		CParentLastIterator& operator=(const CParentLastIterator& _src)
		{
			Root = _src.Root;
			Cur = _src.Cur;
			return (*this);
		}

		void operator++(int)
		{
			Inc();
		}

		CParentLastIterator& operator++(void)
		{
			Inc();
			return *this;
		}

		TNode* operator->()
		{
			return GetNode();
		}

		TNode& operator*()
		{
			return *GetNode();
		}
	private:

		TNode* GetMin(TNode* _node)
		{
			while(_node!=0 && (_node->LeftChild!=0 || _node->RightChild!=0))
			{
				if(_node->LeftChild)
					_node = _node->LeftChild;
				else
					_node = _node->RightChild;
			}
			return _node;
		}

		void Inc()
		{
			// Already at end?
			if (Cur==0)
				return;

			// Note: Starting point is the node as far down to the left as possible.

			// If current node has an uncle to the right, go to the
			// node as far down to the left from the uncle as possible
			// else just go up a level to the parent.
			if (Cur->IsLeftChild() && Cur->Parent->RightChild)
			{
				Cur = GetMin(Cur->Parent->RightChild);
			}
			else
				Cur = Cur->Parent;
		}

		TNode* Root;
		TNode* Cur;
	};

	class CAccessClass
	{
		// Let map be the only one who can instantiate this class.
		friend class CMap<TKey, TValue>;
	public:
		// Assignment operator. Handles the myTree["Foo"] = 32; situation
		void operator=(const TValue& _value)
		{
			// Just use the Set method, it handles already exist/not exist situation
			Tree.Set(Key,_value);
		}

		// ValueType operator
		operator TValue()
		{
			TNode* node = Tree.Find(Key);

			// Not found
			return node->Value;
		}

	private:

		CAccessClass(CMap& _tree, const TKey& _key):Tree(_tree),Key(_key){}

		CAccessClass();

		CMap& Tree;
		const TKey& Key;
	};

	CMap():m_Root(0),m_Size(0){}

	~CMap()
	{
		Clear();
	}

	bool Insert(const TKey& _key, const TValue& _val)
	{
		// First insert node the "usual" way (no fancy balance logic yet)
		TNode* newNode = new TNode(_key,_val);
		if (!Insert(newNode))
		{
			delete newNode;
			return false;
		}

		// Then attend a balancing party
		while(!newNode->IsRoot() && (newNode->Parent->Red))
		{
			if (newNode->Parent->IsLeftChild())
			{
				// If newNode is a left child, get its right 'uncle'
				TNode* newNodesUncle = newNode->Parent->Parent->RightChild;
				if ( newNodesUncle!=0 && newNodesUncle->Red)
				{
					// case 1 - change the colours
					newNode->Parent->Red = false;
					newNodesUncle->Red = false;
					newNode->Parent->Parent->Red = true;
					// Move newNode up the tree
					newNode = newNode->Parent->Parent;
				}
				else
				{
					// newNodesUncle is a black node
					if ( newNode->IsRightChild())
					{
						// and newNode is to the right
						// case 2 - move newNode up and rotate
						newNode = newNode->Parent;
						RotateLeft(newNode);
					}
					// case 3
					newNode->Parent->Red = false;
					newNode->Parent->Parent->Red = true;
					RotateRight(newNode->Parent->Parent);
				}
			}
			else
			{
				// If newNode is a right child, get its left 'uncle'
				TNode* newNodesUncle = newNode->Parent->Parent->LeftChild;
				if ( newNodesUncle!=0 && newNodesUncle->Red)
				{
					// case 1 - change the colours
					newNode->Parent->Red = false;
					newNodesUncle->Red = false;
					newNode->Parent->Parent->Red = true;
					// Move newNode up the tree
					newNode = newNode->Parent->Parent;
				}
				else
				{
					// newNodesUncle is a black node
					if (newNode->IsLeftChild())
					{
						// and newNode is to the left
						// case 2 - move newNode up and rotate
						newNode = newNode->Parent;
						RotateRight(newNode);
					}
					// case 3
					newNode->Parent->Red = false;
					newNode->Parent->Parent->Red = true;
					RotateLeft(newNode->Parent->Parent);
				}

			}
		}
		// Color the root black
		m_Root->Red = false;
		return true;
	}

	void Set(const TKey& _key, const TValue& _val)
	{
		TNode* p = Find(_key);
		if (p)
			p->Value = _val;
		else
			Insert(_key,_val);
	}

	TNode* Delink(const TKey& _key)
	{
		TNode* p = find(_key);
		if (p == 0)
			return 0;

		// Rotate p down to the left until it has no right child, will get there
		// sooner or later.
		while(p->getRightChild())
		{
			// "Pull up my right child and let it knock me down to the left"
			rotateLeft(p);
		}
		// p now has no right child but might have a left child
		TNode* left = p->getLeftChild();

		// Let p's parent point to p's child instead of point to p
		if (p->isLeftChild())
			p->getParent()->setLeftChild(left);

		else if (p->isRightChild())
			p->getParent()->setRightChild(left);

		else
		{
			// p has no parent => p is the root.
			// Let the left child be the new root.
			setRoot(left);
		}

		// p is now gone from the tree in the sense that
		// no one is pointing at it, so return it.

		--m_Size;
		return p;
	}

	bool Remove(const TKey& _key)
	{
		TNode* p = Find(_key);
		if (p == 0)
		{
			return false;
		}

		// Rotate p down to the left until it has no right child, will get there
		// sooner or later.
		while(p->RightChild)
		{
			// "Pull up my right child and let it knock me down to the left"
			RotateLeft(p);
		}
		// p now has no right child but might have a left child
		TNode* left = p->LeftChild;

		// Let p's parent point to p's child instead of point to p
		if (p->IsLeftChild())
			p->Parent->LeftChild = left;

		else if (p->IsRightChild())
			p->Parent->RightChild = left;

		else
		{
			// p has no parent => p is the root.
			// Let the left child be the new root.
			SetRoot(left);
		}

		// p is now gone from the tree in the sense that
		// no one is pointing at it. Let's get rid of it.
		delete p;

		--m_Size;
		return true;
	}

	void Clear()
	{
		CParentLastIterator i(ParentLastIterator());

		while(!i.AtEnd())
		{
			TNode* p = i.GetNode();
			++i; // Increment it before it is deleted
				// else iterator will get quite confused.
			delete p;
		}
		m_Root = 0;
		m_Size= 0;
	}

	bool Empty() const
	{
		return m_Root == 0;
	}

	bool IsEmpty() const
	{
		return Empty();
	}

	TNode* Find(const TKey& _key) const
	{
		TNode* pNode = m_Root;

		while(pNode!=0)
		{
			if(_key == pNode->Key)
				return pNode;
			else 
			if(_key < pNode->Key)
				pNode = pNode->LeftChild;
			else
				pNode = pNode->RightChild;
		}

		return 0;
	}

	TNode* GetRoot() const
	{
		return m_Root;
	}

	u32 Size() const
	{
		return m_Size;
	}

	CIterator Iterator()
	{
		CIterator it(GetRoot());
		return it;
	}

	CParentFirstIterator ParentFirstIterator()
	{
		CParentFirstIterator it(GetRoot());
		return it;
	}

	CParentLastIterator ParentLastIterator()
	{
		CParentLastIterator it(GetRoot());
		return it;
	}

	CAccessClass operator[](const TKey& _key)
	{
		return CAccessClass(*this, _key);
	}

private:
	explicit CMap(const CMap& _src);
	CMap& operator = (const CMap& src);

	void SetRoot(TNode* _node)
	{
		m_Root = _node;
		if(m_Root != 0)
		{
			m_Root->Parent = 0;
			m_Root->Red = false;
		}
	}

	bool Insert(TNode* _node)
	{
		bool result=true; // Assume success

		if(m_Root==0)
		{
			SetRoot(_node);
			m_Size = 1;
		}
		else
		{
			TNode* pNode = m_Root;
			TKey keyNew = _node->Key;
			while (pNode)
			{
				TKey key(pNode->Key);

				if(keyNew == key)
				{
					result = false;
					pNode = 0;
				}
				else
                if(keyNew < key)
				{
					if (pNode->LeftChild == 0)
					{
						pNode->SetLeftChild(_node);
						pNode = 0;
					}
					else
						pNode = pNode->LeftChild;
				}
				else // keyNew > key
				{
					if (pNode->RightChild==0)
					{
						pNode->SetRightChild(_node);
						pNode = 0;
					}
					else
					{
						pNode = pNode->RightChild;
					}
				}
			}

			if (result)
				++m_Size;
		}

		return result;
	}

	void RotateLeft(TNode* _node)
	{
		TNode* right = _node->RightChild;

		_node->SetRightChild(right->LeftChild);

		if(_node->IsLeftChild())
			_node->Parent->SetLeftChild(right);
		else
        if(_node->IsRightChild())
			_node->Parent->SetRightChild(right);
		else
			SetRoot(right);

		right->SetLeftChild(_node);
	}

	void RotateRight(TNode* _node)
	{
		TNode* left = _node->LeftChild;

		_node->SetLeftChild(left->RightChild);

		if(_node->IsLeftChild())
			_node->Parent->SetLeftChild(left);
		else
        if(_node->IsRightChild())
			_node->Parent->SetRightChild(left);
		else
			SetRoot(left);

		left->SetRightChild(_node);
	}

	TNode*	m_Root;
	u32		m_Size;
};

#endif