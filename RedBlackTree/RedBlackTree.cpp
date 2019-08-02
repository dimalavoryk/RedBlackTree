// RedBlackTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RedBlackTree.h"

Node* Node::GetUncle() const
{
	Node* Uncle = nullptr;
	if (pParent && pParent->pParent)
	{
		Uncle = pParent->IsOnLeft() ? pParent->pParent->pRight : pParent->pParent->pLeft;
	}
	return Uncle;
}

Node* Node::GetSibling() const
{
	Node* Sibling = nullptr;
	if (pParent)
	{
		Sibling = IsOnLeft() ? pParent->pRight : pParent->pLeft;
	}
	return Sibling;
}

bool Node::HasRedChild() const
{
	return  (pLeft && pLeft->IsRed()) || (pRight && pRight->IsRed());
}

void Node::MoveDown(Node* NewParent)
{
	if (pParent)
	{
		if (IsOnLeft())
		{
			pParent->pLeft = NewParent;
		}
		else
		{
			pParent->pRight= NewParent;
		}
	}
	NewParent->pParent = pParent;
	pParent = NewParent;
}

void RedBlackTree::Insert(Type NewValue)
{
	Node* NewNode = new Node(NewValue);
	if (!pRoot)
	{
		pRoot = NewNode;
		pRoot->Color = EColors::cBlack;
	}
	else
	{
		Insert(NewNode);

		FixRedRed(NewNode);
	}
}

void RedBlackTree::PreOrderTraversal_Recursive() const
{
	if (pRoot)
	{
		PreOrderTraversal_Recursive_Internal(pRoot);
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Tree is empty\n";
	}
}

void RedBlackTree::PostOrderTraversal_Recursive() const
{
	if (pRoot)
	{
		PostOrderTraversal_Recursive_Internal(pRoot);
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Tree is empty\n";
	}
}

void RedBlackTree::InOrderTraversal_Recursive() const
{
	if (pRoot)
	{
		InOrderTraversal_Recursive_Internal(pRoot);
		std::cout << std::endl;
	}
	else
	{
		std::cerr << "Tree is empty\n";
	}
}

void RedBlackTree::DeleteByValue(int Value)
{
	Node * NodeToDelete = Search(Value);
	if (NodeToDelete && NodeToDelete->Val == Value)
	{
		Delete_Internal_Recursive(NodeToDelete);
	}
	else
	{
		std::cerr << "Not Found\n";
	}
}

Node* RedBlackTree::Search(int Value) const
{
	Node* pIt = pRoot;
	while (pIt != nullptr) 
	{
		if (Value < pIt->Val)
		{
			if (pIt->pLeft == nullptr)
			{
				break;
			}
			else
			{
				pIt = pIt->pLeft;
			}
		}
		else if (Value == pIt->Val)
		{	
			break;
		}
		else 
		{
			if (pIt->pRight == nullptr)
			{
				break;
			}
			else
			{
				pIt = pIt->pRight;
			}
		}
	}
	return pIt;
}

RedBlackTree::~RedBlackTree()
{
	if (pRoot)
	{
		DeleteWithChildren_Internal_Recursive(pRoot);
	}
	pRoot = nullptr;
}

void RedBlackTree::DeleteWithChildren_Internal_Recursive(Node* NodeToDelete)
{
	if (NodeToDelete->pLeft)
	{
		DeleteWithChildren_Internal_Recursive(NodeToDelete->pLeft);
	}
	if (NodeToDelete->pRight)
	{
		DeleteWithChildren_Internal_Recursive(NodeToDelete->pRight);
	}
	delete NodeToDelete;
}

void RedBlackTree::Delete_Internal_Recursive(Node* NodeToDelete)
{
	Node* U = BSTReplace(NodeToDelete);
	const bool bBothAreBlack = (!U || U->IsBlack()) && NodeToDelete->IsBlack();
	Node * Parent = NodeToDelete->pParent;
	if (!U)//NodeToDelete is leaf
	{
		if (NodeToDelete == pRoot)
		{
			pRoot = nullptr;
		}
		else
		{
			if (bBothAreBlack)
			{
				FixBlackBlack(NodeToDelete);
			}
			else if (auto * Sb = NodeToDelete->GetSibling()) // NodeToDelete or U is red, so...//@todo dlavoryk: wtf
			{
				Sb->Color = EColors::cRed;
			}
			(NodeToDelete->IsOnLeft() ? NodeToDelete->pParent->pLeft : NodeToDelete->pParent->pRight) = nullptr;
		}
		delete NodeToDelete;
	}//if 1 child
	else if (!NodeToDelete->pLeft || !NodeToDelete->pRight)
	{
		if (pRoot == NodeToDelete)
		{
			// because it's RED-Black tree, it's normal to expect that if Root has only 1 child than there are just 2 nodes
			NodeToDelete->Val = U->Val;
			NodeToDelete->pLeft = NodeToDelete->pRight = nullptr;
			delete U;
		}
		else
		{
			(NodeToDelete->IsOnLeft() ? Parent->pLeft : Parent->pRight) = U;
			delete NodeToDelete;
			U->pParent = Parent;
			if (bBothAreBlack)
			{
				FixBlackBlack(U);
			}
			else
			{
				U->Color = EColors::cBlack;
			}
		}
	}
	else // NodeToDelete has 2 children, so swap values and try to delete 1 more time
	{
		U->SwapValue(NodeToDelete);
		Delete_Internal_Recursive(U);
	}
}

#define ROTATE_HELPER(NodeToMoveDown, RotationSide, OppositeSide)\
Node* NewParent = (NodeToMoveDown)->OppositeSide;\
if ((NodeToMoveDown) == pRoot)\
{\
	pRoot = NewParent;\
}\
(NodeToMoveDown)->MoveDown(NewParent);\
(NodeToMoveDown)->OppositeSide = NewParent->RotationSide;\
NewParent->RotationSide = (NodeToMoveDown);\
if ((NodeToMoveDown)->OppositeSide)\
{\
	(NodeToMoveDown)->OppositeSide->pParent = NodeToMoveDown;\
}


void RedBlackTree::LeftRotate(Node* NodeToMoveDown)
{
	ROTATE_HELPER(NodeToMoveDown, pLeft, pRight);
}

void RedBlackTree::RightRotate(Node* NodeToMoveDown)
{
	ROTATE_HELPER(NodeToMoveDown, pRight, pLeft);
}

void RedBlackTree::Insert(Node* NewNode) const
{
	Node* pIt = pRoot;
	while (pIt != nullptr)
	{
		if (NewNode->Val < pIt->Val)
		{
			if (pIt->pLeft != nullptr)
			{
				pIt = pIt->pLeft;
			}
			else
			{
				pIt->pLeft = NewNode;
				NewNode->pParent = pIt;
				break;
			}
		}
		else
		{
			if (pIt->pRight != nullptr)
			{
				pIt = pIt->pRight;
			}
			else
			{
				pIt->pRight = NewNode;
				NewNode->pParent = pIt;
				break;
			}
		}
	}
}

//@todo dlavoryk: try to avoid recursion here
void RedBlackTree::FixRedRed(Node* NewNode)
{
	if (NewNode == pRoot)
	{
		NewNode->Color = EColors::cBlack;
	}
	else
	{
		Node* Parent = NewNode->pParent;
		Node* Uncle = NewNode->GetUncle();
		Node* GrandPa = NewNode->pParent->pParent;
		if (Parent->Color != EColors::cBlack)
		{
			if (Uncle && Uncle->IsRed())
			{
				Parent->Color = Uncle->Color = EColors::cBlack;
				GrandPa->Color = EColors::cRed;
				FixRedRed(GrandPa);
			}/// Uncle could possibly be null, but we don't care as all "null nodes" are black
			else
			{
				if (Parent->IsOnLeft())
				{
					if (NewNode->IsOnLeft())
					{
						RightRotate(GrandPa);
						GrandPa->SwapColor(Parent);
					}
					else
					{
						LeftRotate(Parent);
						RightRotate(GrandPa);
						GrandPa->SwapColor(NewNode);
					}
				}
				else
				{
					if (NewNode->IsOnLeft())
					{
						RightRotate(Parent);
						LeftRotate(GrandPa);
						GrandPa->SwapColor(NewNode);
					}
					else
					{
						LeftRotate(GrandPa);
						GrandPa->SwapColor(Parent);
					}
				}
			}
		}
	}
}

void RedBlackTree::FixBlackBlack(Node* X)
{
	Node* Sibling = X->GetSibling();
	Node* Parent = X->pParent;
	if (Sibling)
	{
		if (Sibling->IsRed())
		{
			Parent->Color = EColors::cRed;
			Sibling->Color = EColors::cBlack;
			if (Sibling->IsOnLeft())
			{
				RightRotate(Parent);
			}
			else
			{
				LeftRotate(Parent);
			}
			FixBlackBlack(X);
		}
		else
		{
			if (Sibling->HasRedChild())
			{
				Node* R = Sibling->pLeft;
				if (!R || R->IsBlack())
				{
					R = Sibling->pRight;
				}

				if (Sibling->IsOnLeft())
				{
					if (R->IsOnLeft())//left left
					{
						Sibling->pLeft->Color = Sibling->Color;
						Sibling->Color = Parent->Color;
						RightRotate(Parent);
					}
					else//left right
					{
						Sibling->pRight->Color = Parent->Color;
						LeftRotate(Sibling);
						RightRotate(Parent);
					}
				}
				else
				{
					if (R->IsOnLeft())//right left
					{
						Sibling->pLeft->Color = Parent->Color;
						RightRotate(Sibling);
						LeftRotate(Parent);
					}
					else//right right
					{
						//Sibling->pRight->Color = EColors::cBlack;
						Sibling->pRight->Color = Sibling->Color;
						// just black??
						Sibling->Color = Parent->Color;
						LeftRotate(Parent);
					}
				}
			}
			else
			{
				Sibling->Color = EColors::cRed;
				if (Parent->IsBlack())
				{
					FixBlackBlack(Parent);
				}
				else
				{
					Parent->Color = EColors::cBlack;
				}
			}
		}
	}
	else
	{
		FixBlackBlack(Parent);
	}
}

void RedBlackTree::PreOrderTraversal_Recursive_Internal(Node* Root) const
{
	std::cout << Root->Val << " ";
	if (Root->pLeft)
	{
		PreOrderTraversal_Recursive_Internal(Root->pLeft);
	}
	if (Root->pRight)
	{
		PreOrderTraversal_Recursive_Internal(Root->pRight);
	}
}

void RedBlackTree::PostOrderTraversal_Recursive_Internal(Node* Root) const
{
	if (Root->pLeft)
	{
		PostOrderTraversal_Recursive_Internal(Root->pLeft);
	}
	if (Root->pRight)
	{
		PostOrderTraversal_Recursive_Internal(Root->pRight);
	}
	std::cout << Root->Val << " ";
}


void RedBlackTree::InOrderTraversal_Recursive_Internal(Node* Root) const
{
	if (Root->pLeft)
	{
		InOrderTraversal_Recursive_Internal(Root->pLeft);
	}
	std::cout << Root->Val << " ";
	if (Root->pRight)
	{
		InOrderTraversal_Recursive_Internal(Root->pRight);
	}
}

Node* RedBlackTree::Successor(Node* X) const
{
	Node* pIt = X;
	while (pIt->pLeft != nullptr)
	{
		pIt = pIt->pLeft;
	}
	return pIt;
}

Node* RedBlackTree::BSTReplace(Node* X) const
{
	Node* RetVal = nullptr;
	if (X->pLeft && X->pRight)
	{
		RetVal = Successor(X);
	}
	else if (X->pLeft)
	{
		RetVal = X->pLeft;
	}
	else if (X->pRight)
	{
		RetVal = X->pRight;
	}
	return RetVal;
}
