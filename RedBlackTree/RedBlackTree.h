#pragma once

#include <iostream>

using Type = int;

enum class EColors : unsigned char
{
	cRed,
	cBlack
};


struct Node
{
	explicit Node(Type InVal) : Val(InVal) {}

	bool IsOnLeft() const {	return pParent->pLeft == this; }
	Node * GetUncle() const;
	Node * GetSibling () const;

	bool HasRedChild() const;

	// changes CurParent  to NewParent and just replace this node as parent's child to NewParent
	void MoveDown(Node* NewParent);

	void SwapColor(Node * Right)
	{
		std::swap(Right->Color, Color);
	}

	void SwapValue(Node * Right)
	{
		std::swap(Right->Val, Val);
	}

	bool IsRed() const { return Color == EColors::cRed; }
	bool IsBlack() const { return Color == EColors::cBlack; }

	Node* pLeft = nullptr;
	Node* pRight = nullptr;
	Node* pParent = nullptr;

	Type Val;

	EColors Color = EColors::cRed;
};


class RedBlackTree
{
public:


	void Insert(Type NewValue);

	void PreOrderTraversal_Recursive() const;
	void PostOrderTraversal_Recursive() const;
	void InOrderTraversal_Recursive() const;

	void DeleteByValue(int Value);


	Node* Search(int Value) const;
	~RedBlackTree();

private:
	void DeleteWithChildren_Internal_Recursive(Node* NodeToDelete);
	void Delete_Internal_Recursive(Node* NodeToDelete);


	void LeftRotate(Node* NodeToMoveDown);
	void RightRotate(Node* NodeToMoveDown);

	void Insert(Node* NewNode) const;


	void FixRedRed(Node * NewNode);
	void FixBlackBlack(Node* X);


	void PreOrderTraversal_Recursive_Internal(Node * Root) const;
	void PostOrderTraversal_Recursive_Internal(Node* Root) const;
	void InOrderTraversal_Recursive_Internal(Node* Root) const;


	Node* Successor(Node * X) const;
	Node* BSTReplace(Node* X) const;


private:
	Node* pRoot = nullptr;

};

