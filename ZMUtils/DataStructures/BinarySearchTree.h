#pragma once
#include <Debugging\ZMDebugger.h>
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

	//Memory Allocation
	TEMPLATE_HEADER
	static BSTNode TEMPLATE_ARGS* AllocateNode(const Type& value, BSTNode TEMPLATE_ARGS* parent = nullptr, BSTNode TEMPLATE_ARGS* left = nullptr, BSTNode TEMPLATE_ARGS* right = nullptr)
	{
		return new BSTNode TEMPLATE_ARGS(value, parent, left, right);//TODO: add allocator
	}

	TEMPLATE_HEADER
	class BinarySearchTree
	{
	public:
		BinarySearchTree();
		BinarySearchTree(BSTNode TEMPLATE_ARGS* root_node);
		~BinarySearchTree();

		BSTNode TEMPLATE_ARGS* Insert(const Type& value);

		BSTNode TEMPLATE_ARGS* Delete(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* Delete(const Type& value);

		BSTNode TEMPLATE_ARGS* Find(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* Find(const Type& value);

	private:
		BSTNode TEMPLATE_ARGS* InsertInternal(const Type& value);

		void Test(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent);

		void Rotate(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent);
		void RotateRight(BSTNode TEMPLATE_ARGS* node);
		void RotateLeft(BSTNode TEMPLATE_ARGS* node);

		BSTNode TEMPLATE_ARGS* Splay(BSTNode TEMPLATE_ARGS* node);

		BSTNode TEMPLATE_ARGS* m_Root;
		int m_NumberOfNodes;
		Comparator m_Comparer;
	};

	TEMPLATE_HEADER
	BinarySearchTree TEMPLATE_ARGS::BinarySearchTree()
	{
		m_Root = nullptr;
	}

	TEMPLATE_HEADER
	BinarySearchTree TEMPLATE_ARGS::BinarySearchTree(BSTNode TEMPLATE_ARGS* root_node)
	{
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
		++m_NumberOfNodes;
		if (m_Root == nullptr)
		{
			m_Root = AllocateNode TEMPLATE_ARGS(value);
			return m_Root;
		}
		return InsertInternal(value);
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::InsertInternal(const Type& value)
	{
		BSTNode TEMPLATE_ARGS* current = m_Root;
		BSTNode TEMPLATE_ARGS* inserted = nullptr;
		while (true)
		{
			if (m_Comparer.LessThan(value, current->value))
			{
				if (current->left == nullptr)
				{
					inserted = AllocateNode TEMPLATE_ARGS(value);
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
					inserted =  AllocateNode TEMPLATE_ARGS(value);
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
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Delete(BSTNode TEMPLATE_ARGS* node)
	{

		return nullptr;
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(BSTNode TEMPLATE_ARGS* node)
	{
		return Find(node->value);
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(const Type& value)
	{
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
}