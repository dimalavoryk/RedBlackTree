#pragma once



using Type = int;

enum class EColors : unsigned char
{
	cRed,
	cBlack
};


struct Node
{
	Node(Type InVal) : Val(InVal) {}


	





	Node* pLeft = nullptr;
	Node* pRight = nullptr;


	Type Val;

	EColors Color = EColors::cRed;
};


class RedBlackTree
{
public:




private:
	Node* pRoot = nullptr;

};

