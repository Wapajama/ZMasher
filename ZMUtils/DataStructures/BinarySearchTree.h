#pragma once
#include "GrowArray.h"
#include <Debugging\ZMDebugger.h>
#include <stdlib.h>
#include <iostream>

#define TEMPLATE_HEADER template<typename Type, typename Comparator>
#define TEMPLATE_ARGS <Type, Comparator>

namespace ZMasher
{
	template<typename Type>
	class BSTComparator
	{
	public:
		virtual bool LessThan(const Type& first, const Type& second)const = 0;
		virtual bool GreaterThan(const Type& first, const Type& second)const = 0;
		virtual bool Equals(const Type& first, const Type& second)const = 0;
	};

	//implemented as a "splay tree"
	TEMPLATE_HEADER
	struct BSTNode
	{
		BSTNode()
			: left(nullptr)
			, right(nullptr)
			, parent(nullptr)
		{
		}
		BSTNode(const Type& a_value, BSTNode* a_parent = nullptr, BSTNode* a_left = nullptr, BSTNode* a_right = nullptr)
			: value(a_value)
			, parent(a_parent)
			, left(a_left)
			, right(a_right)
		{
		}
		bool operator<(const BSTNode& other)
		{
			return m_Comparator.LessThan(value, other.value);
		}
		bool operator>(const BSTNode& other)
		{
			return m_Comparator.GreaterThan(value, other.value);
		}
		bool operator==(const BSTNode& other)
		{
			return m_Comparator.Equals(value, other.value);
		}

		Type value;
		BSTNode* left;
		BSTNode* right;
		BSTNode* parent;
	private:
		Comparator m_Comparator;
	};

	//left is less, right is more, equal is invalid
	TEMPLATE_HEADER
	class BinarySearchTree
	{
	public:
		BinarySearchTree(const bool use_malloc = false);
		BinarySearchTree(BSTNode TEMPLATE_ARGS* root_node);
		~BinarySearchTree();

		BSTNode TEMPLATE_ARGS* Insert(const Type& value);

		void Delete(BSTNode TEMPLATE_ARGS* node);
		void Delete(const Type& value);

		BSTNode TEMPLATE_ARGS* Find(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* Find(const Type& value);

		BSTNode TEMPLATE_ARGS* Max();
		BSTNode TEMPLATE_ARGS* Min();
		BSTNode TEMPLATE_ARGS* Max(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* Min(BSTNode TEMPLATE_ARGS* node);

		int Count();

	private:
		void CountInternal(BSTNode TEMPLATE_ARGS* node, int& count);

		BSTNode TEMPLATE_ARGS* InsertInternal(const Type& value, BSTNode TEMPLATE_ARGS* node = nullptr);

		void Test(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent);

		void Rotate(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent);
		void RotateRight(BSTNode TEMPLATE_ARGS* node);
		void RotateLeft(BSTNode TEMPLATE_ARGS* node);


		BSTNode TEMPLATE_ARGS* Splay(BSTNode TEMPLATE_ARGS* node);


		BSTNode TEMPLATE_ARGS* AllocateNode(const Type& value, 
											   BSTNode TEMPLATE_ARGS* parent = nullptr, 
											   BSTNode TEMPLATE_ARGS* left = nullptr, 
											   BSTNode TEMPLATE_ARGS* right = nullptr);

		void DeallocateNode(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* m_Root;
		int m_NumberOfNodes;
		Comparator m_Comparer;
		const bool m_UseMalloc;
	};

	TEMPLATE_HEADER
	BinarySearchTree TEMPLATE_ARGS::BinarySearchTree(const bool use_malloc)
		:m_UseMalloc(use_malloc)
	{
		m_NumberOfNodes = 0;
		m_Root = nullptr;
	}
	TEMPLATE_HEADER
	BinarySearchTree TEMPLATE_ARGS::BinarySearchTree(BSTNode TEMPLATE_ARGS* root_node)
	{
		m_NumberOfNodes= 0;
		m_Root = root_node;
	}
	TEMPLATE_HEADER
	BinarySearchTree TEMPLATE_ARGS::~BinarySearchTree()
	{
	}
	//TEMPLATE_HEADER
	//void BinarySearchTree TEMPLATE_ARGS::TestIfCorrect()
	//{
	//	Test(m_Root, nullptr);
	//}
	//
	//static int max_counter = 0;
	//static int counter = 0;
	//TEMPLATE_HEADER
	//void BinarySearchTree TEMPLATE_ARGS::Test(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent)
	//{
	//	if (node == nullptr)
	//	{
	//		return;
	//	}
	//	++counter;
	//	if (counter > max_counter)
	//	{
	//		max_counter = counter;
	//	}
	//	ASSERT(node->parent == parent, "nodes parent is wrong!");
	//	Test(node->left, node);
	//	Test(node->right, node);
	//}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Insert(const Type& value)
	{
		if (m_Root == nullptr)
		{
			m_Root = AllocateNode(value);

			++m_NumberOfNodes;
			//const int cnt = Count();
			//ASSERT(cnt == m_NumberOfNodes, "Tree error!");
			return m_Root;
		}
		BSTNode TEMPLATE_ARGS* node = InsertInternal(value);
		if (node != nullptr)
		{
			++m_NumberOfNodes;
			//const int cnt = Count();
			//ASSERT(cnt == m_NumberOfNodes, "Tree error!");
		}
		return node;
	}
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::Delete(BSTNode TEMPLATE_ARGS* node)
	{
		Splay(node);
	

		BSTNode TEMPLATE_ARGS* left_subtree = node->left;
		BSTNode TEMPLATE_ARGS* right_subtree = node->right;
		if (node->left)
		{
			node->left->parent = nullptr;
		}
		if (node->right)
		{
			node->right->parent = nullptr;
		}

		if (node->parent && node->parent->left == node)
		{
			node->parent->left = nullptr;
		}
		else if(node->parent)
		{
			node->parent->right = nullptr;
		}

		DeallocateNode(node);
		if (left_subtree)
		{
			m_Root = Max(left_subtree);
			if (m_Root->left && m_Root->parent)
			{
				m_Root->parent->right = m_Root->left;
				m_Root->parent->right->parent = m_Root->parent;
			}
			else if(m_Root->parent)
			{
				m_Root->parent->right = nullptr;
			}
		}
		else if(right_subtree)
		{
			m_Root = Min(right_subtree);
			if (m_Root->right && m_Root->parent)
			{
				m_Root->parent->left = m_Root->right;
				m_Root->parent->left->parent = m_Root->parent;
			}
			else if(m_Root->parent)
			{
				m_Root->parent->left = nullptr;
			}
		}
		else
		{
			//We splayed it up, and it has no children, remove
			m_Root = nullptr;
			this->m_NumberOfNodes = 0;
			return;
		}

		//if (m_Root->parent && m_Root->left)
		//{
		//	m_Root->parent->left = nullptr;
		//	m_Root->parent->right = nullptr;
		//}

		m_Root->parent = nullptr;

		if (m_Root != left_subtree &&
			left_subtree != nullptr)
		{
			m_Root->left = left_subtree;
			left_subtree->parent = m_Root;
		}
		//ASSERT(m_Root != m_Root->parent, "Tree error!");
		//ASSERT(m_Root != m_Root->parent, "Tree error!");
		if (m_Root != right_subtree &&
			right_subtree != nullptr)
		{
			m_Root->right = right_subtree;
			right_subtree->parent = m_Root;
		}
		//ASSERT(m_Root != m_Root->parent, "Tree error!");
		//ASSERT(m_Root != m_Root->parent, "Tree error!");

		--m_NumberOfNodes;

		//const int cnt = Count();
		//ASSERT(cnt == m_NumberOfNodes, "Tree error!");
	}
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::Delete(const Type& value)
	{
		BSTNode TEMPLATE_ARGS* node = Find(value);
		if (node)
		{
			Delete(node);
		}
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(BSTNode TEMPLATE_ARGS* node)
	{
		return Find(node->value);
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(const Type& value)
	{
		//can't find anything if we haven't added anything yet
		if (!m_Root)
		{
			return nullptr;
		}

		BSTNode TEMPLATE_ARGS* current = m_Root;

		//break means return nullptr
		while (true)
		{
			if (m_Comparer.Equals(value, current->value))
			{
				Splay(current);
				return current;
			}
			if (m_Comparer.LessThan(value, current->value))
			{
				if (current->left == nullptr)
				{
					break;
				}
				current = current->left;
			}
			else
			{
				if (current->right == nullptr)
				{
					break;
				}
				current = current->right;
			}
		}
		Splay(current);
		return nullptr;
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Max()
	{
		return Max(m_Root);
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Min()
	{
		return Min(m_Root);
	}
	TEMPLATE_HEADER
	int BinarySearchTree TEMPLATE_ARGS::Count()
	{
		int count =0;
		CountInternal(m_Root, count);
		return count;
	}

	
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::CountInternal(BSTNode TEMPLATE_ARGS* node, int& count)
	{
		//static GrowArray<BSTNode TEMPLATE_ARGS*> loc_nodes(1000);
		if (count == 0)
		{
			//loc_nodes.RemoveAll();
		}
		if (node)
		{
			++count;
			//ASSERT(loc_nodes.found_none == loc_nodes.Find(node), "Tree error!");
			//loc_nodes.Add(node);
			//ASSERT(node != node->parent, "Tree error!");
			//ASSERT(node != node->left, "Tree error!");
			//ASSERT(node != node->right, "Tree error!");
			if (node->left)
			{
				//ASSERT(m_Comparer.LessThan(node->left->value, node->value), "Tree error!");
			}
			if (node->right)
			{
				//ASSERT(!m_Comparer.LessThan(node->right->value, node->value), "Tree error!");
			}
			//ASSERT(node != node->right, "Tree error!");

			CountInternal(node->left, count);
			CountInternal(node->right, count);
		}
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Max(BSTNode TEMPLATE_ARGS* node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		BSTNode TEMPLATE_ARGS* tmp_node = node;
		while (tmp_node->right != nullptr)
		{
			tmp_node = tmp_node->right;
		}
		return tmp_node;
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Min(BSTNode TEMPLATE_ARGS* node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		BSTNode TEMPLATE_ARGS* tmp_node = node;
		while (tmp_node->left != nullptr)
		{
			tmp_node = tmp_node->left;
		}
		return tmp_node;
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::InsertInternal(const Type& value, BSTNode TEMPLATE_ARGS* node)
	{
		BSTNode TEMPLATE_ARGS* current = m_Root;
		BSTNode TEMPLATE_ARGS* inserted = node;
		while (true)
		{
			if (m_Comparer.LessThan(value, current->value))
			{
				if (current->left == nullptr)
				{
					if (inserted == nullptr)
					{
						inserted = AllocateNode(value);
					}
					current->left = inserted;
					inserted->parent = current;
					break;
				}
				current = current->left;
			}
			else if (m_Comparer.Equals(value, current->value))
			{
				return nullptr;//can't have two of the same value
			}
			else
			{
				if (current->right == nullptr)
				{
					if (inserted == nullptr)
					{
						inserted = AllocateNode(value);
					}
					current->right = inserted;
					inserted->parent = current;
					break;
				}
				current = current->right;
			}
		}
		Splay(inserted);
		return inserted;
	}	
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::Rotate(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent)
	{
		if (parent->left == node)
		{
			RotateRight(parent);
		}
		else
		{
			RotateLeft(parent);
		}
	}
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::RotateRight(BSTNode TEMPLATE_ARGS* node)
	{
		BSTNode TEMPLATE_ARGS* parent_tree = node->parent;

		BSTNode TEMPLATE_ARGS* temp = node->left->right;
		node->left->right = node;
		node->left->parent = parent_tree;
		node->parent = node->left;
		node->left = temp;
		if (node->left != nullptr)
		{
			node->left->parent = node;
		}

		if (parent_tree == nullptr)
		{
			return;
		}
		if (parent_tree->right == node)
		{
			parent_tree->right = node->parent;
		}
		else
		{
			parent_tree->left = node->parent;
		}
	}
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::RotateLeft(BSTNode TEMPLATE_ARGS* node)
	{
		BSTNode TEMPLATE_ARGS* parent_tree = node->parent;

		BSTNode TEMPLATE_ARGS* temp = node->right->left;
		node->right->left = node;
		node->right->parent = parent_tree;
		node->parent = node->right;
		node->right = temp;
		if (node->right != nullptr)
		{
			node->right->parent = node;
		}

		if (parent_tree == nullptr)
		{
			return;
		}
		if (parent_tree->left == node)
		{
			parent_tree->left = node->parent;
		}
		else
		{
			parent_tree->right = node->parent;
		}
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Splay(BSTNode TEMPLATE_ARGS* node)
	{
		ASSERT(node != nullptr, "node is nullptr!");
		if (node == m_Root)
		{
			return node;
		}
		BSTNode TEMPLATE_ARGS* current = node;
		while (true)
		{
			//done
			if (current->parent == nullptr)//some of the rotations set its parent to nullptr
			{
				m_Root = current;
				break;
			}
			//zig
			if (current->parent == m_Root)
			{
				Rotate(current, current->parent);
				m_Root = current;
				break;
			}
			//zig zig
			if (current->parent->parent->left ==	current->parent &&
				current->parent->left ==			current ||
				
				current->parent->parent->right == current->parent &&
				current->parent->right ==			current)
			{
				Rotate(current->parent, current->parent->parent);
				Rotate(current, current->parent);
				continue;
			}

			//zig zag
			if (current->parent->parent->left ==	current->parent &&
				current->parent->right ==			current ||
				
				current->parent->parent->right ==	current->parent &&
				current->parent->left ==			current)
			{
				Rotate(current, current->parent);
				Rotate(current, current->parent);
				continue;
			}
		}

		return current;
	}
	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::AllocateNode(const Type& value, 
											   BSTNode TEMPLATE_ARGS* parent, 
											   BSTNode TEMPLATE_ARGS* left, 
											   BSTNode TEMPLATE_ARGS* right)
	{
		if (m_UseMalloc)
		{
			void* node = malloc(sizeof(BSTNode TEMPLATE_ARGS));
			return new(node) BSTNode TEMPLATE_ARGS(value, parent, left, right);
		}
		return new BSTNode TEMPLATE_ARGS(value, parent, left, right);
	}
	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::DeallocateNode(BSTNode TEMPLATE_ARGS* node)
	{
		if (m_UseMalloc)
		{
			free(node);
		}
		else
		{
			delete node;
		}
	}
}