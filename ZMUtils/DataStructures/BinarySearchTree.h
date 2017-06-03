#pragma once

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
		{
			m_LeftNode = nullptr;
			m_RightNode = nullptr;
		}
		BSTNode(const Type& value, BSTNode* left = nullptr, BSTNode* right = nullptr)
		{
			m_Value = value;
			m_LeftNode = left;
			m_RightNode = right;
		}
		bool operator<(const BSTNode& other)
		{
			return m_Comparator.LessThan(m_Value, other.m_Value);
		}
		bool operator>(const BSTNode& other)
		{
			return m_Comparator.GreaterThan(m_Value, other.m_Value);
		}
		bool operator==(const BSTNode& other)
		{
			return m_Comparator.Equals(m_Value, other.m_Value);
		}

		Type m_Value;
		BSTNode* m_LeftNode;
		BSTNode* m_RightNode;
	private:
		Comparator m_Comparator;
	};

	//Memory Allocation
	TEMPLATE_HEADER
	static BSTNode TEMPLATE_ARGS* AllocateNode(const Type& value, BSTNode TEMPLATE_ARGS* left = nullptr, BSTNode TEMPLATE_ARGS* right = nullptr)
	{
		return new BSTNode TEMPLATE_ARGS(value, left, right);
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

		BSTNode TEMPLATE_ARGS* Find(BSTNode TEMPLATE_ARGS* node);
		BSTNode TEMPLATE_ARGS* Find(const Type& value);

	private:

		BSTNode TEMPLATE_ARGS* FindRecursive(BSTNode TEMPLATE_ARGS* node, const Type& value, BSTNode TEMPLATE_ARGS* parent, BSTNode TEMPLATE_ARGS* grand_parent);
		BSTNode TEMPLATE_ARGS* InsertValueRecursive(BSTNode TEMPLATE_ARGS* node, const Type& value);

		void Rotate(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent);

		BSTNode TEMPLATE_ARGS* Splay(BSTNode TEMPLATE_ARGS* current,
									 BSTNode TEMPLATE_ARGS* parent,
									 BSTNode TEMPLATE_ARGS* grand_parent);

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

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Insert(const Type& value)
	{
		++m_NumberOfNodes;
		if (m_Root == nullptr)
		{
			m_Root = AllocateNode TEMPLATE_ARGS(value);
			return m_Root;
		}
		return InsertValueRecursive(m_Root, value);
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::InsertValueRecursive(BSTNode TEMPLATE_ARGS* node, const Type& value)
	{
		ASSERT(node != nullptr, "Node is nullptr!");

		if (m_Comparer.LessThan(value, node->m_Value))
		{
			if (node->m_LeftNode == nullptr)
			{
				return node->m_LeftNode = AllocateNode TEMPLATE_ARGS(value);
			}
			return InsertValueRecursive(node->m_LeftNode, value);
		}
		if (node->m_RightNode == nullptr)
		{
			return node->m_RightNode = AllocateNode TEMPLATE_ARGS(value);
		}
		return InsertValueRecursive(node->m_RightNode, value);
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Delete(BSTNode TEMPLATE_ARGS* node)
	{

		return nullptr;
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(BSTNode TEMPLATE_ARGS* node)
	{

		return nullptr;
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Find(const Type& value)
	{
		if (m_Root == nullptr)
		{
			return m_Root;
		}

		return FindRecursive(m_Root, value);
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::FindRecursive(BSTNode TEMPLATE_ARGS* node,
																		 const Type& value,
																		 BSTNode TEMPLATE_ARGS* parent,
																		 BSTNode TEMPLATE_ARGS* grand_parent)
	{
		ASSERT(node != nullptr, "Node is nullptr!");
		if (m_Comparer.Equals(value, node->m_Value))
		{
			return node;
		}
		if (m_Comparer.LessThan(value, node->m_Value))
		{
			if (node->m_LeftNode == nullptr)
			{
				return nullptr;
			}
			return FindRecursive(node->m_LeftNode, value);
		}
		if (node->m_RightNode == nullptr)
		{
			return nullptr;
		}
		return FindRecursive(node->m_RightNode, value);
	}

	TEMPLATE_HEADER
	void BinarySearchTree TEMPLATE_ARGS::Rotate(BSTNode TEMPLATE_ARGS* node, BSTNode TEMPLATE_ARGS* parent)
	{
		//less than
		if (m_Comparer.LessThan(node->m_Value, parent->m_Value))
		{
			BSTNode TEMPLATE_ARGS* temp = node->m_RightNode;
			node->m_RightNode = parent;
			parent->m_LeftNode = temp;
			return;
		}
		//greater than, equals to
		BSTNode TEMPLATE_ARGS* temp = node->m_LeftNode;
		node->m_LeftNode = parent;
		parent->m_RightNode = temp;
	}

	TEMPLATE_HEADER
	BSTNode TEMPLATE_ARGS* BinarySearchTree TEMPLATE_ARGS::Splay(	BSTNode TEMPLATE_ARGS* current,
																	BSTNode TEMPLATE_ARGS* parent,
																	BSTNode TEMPLATE_ARGS* grand_parent)
	{
		//zig zag
		if (current == parent->m_LeftNode && //leftchild of a rightchild
			parent == grand_parent->m_RightNode || 

			current == parent->m_RightNode &&
			parent == grand_parent->m_LeftNode)
		{
			Rotate(current, parent);
			Rotate(current, grand_parent);
			return current;
		}

		//zig zig
		if (current == parent->m_LeftNode && //leftchild of a rightchild
			parent == grand_parent->m_LeftNode || 

			current == parent->m_RightNode &&
			parent == grand_parent->m_RightNode)
		{
			Rotate(parent, grand_parent);
			Rotate(current, parent);
			return current;
		}

		//zig
		if (grand_parent == nullptr)//we have hit root node
		{
			Rotate(current, parent);
			return current;
		}

		ASSERT(false, "Splay Failed!");
		return nullptr;
	}
}