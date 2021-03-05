#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "GracefullyTree.h"

Tree::Tree()
{
	this->root = nullptr;
}

TreeNode* Tree::copy(TreeNode* otherRoot)
{
	if (otherRoot == nullptr)
	{
		return nullptr;
	}

	TreeNode* copyRoot = new TreeNode();
	copyRoot->data = otherRoot->data;
	size_t childredSize = otherRoot->children.size();
	for (size_t i = 0; i < childredSize; i++)
	{
		copyRoot->children.push_back(copyRoot->children[i]);
	}

	return copyRoot;
}

void Tree::clear(TreeNode* treeRoot)
{
	if (treeRoot == nullptr)
	{
		return;
	}

	size_t childrenSize = treeRoot->children.size();
	for (size_t i = 0; i < childrenSize; i++)
	{
		this->clear(treeRoot->children[i]);
	}

	delete treeRoot;
}

Tree::Tree(const Tree& other)
{
	this->root = this->copy(other.root);
}

Tree& Tree::operator=(const Tree& other)
{
	if (this != &other)
	{
		this->clear(this->root);
		this->root = this->copy(other.root);
	}
	return *this;
}

Tree::~Tree()
{
	this->clear(this->root);
}

size_t Tree::countTreeNodesImpl(TreeNode* treeNode) const
{
	if (treeNode == nullptr)
	{
		return 0;
	}

	size_t count = 1;
	size_t childrenSize = treeNode->children.size();
	for (size_t i = 0; i < childrenSize; i++)
	{
		count += this->countTreeNodesImpl(treeNode->children[i]);
	}

	return count;
}


void Tree::isGracefullyImpl(TreeNode* treeRoot, std::vector<int>& differences)
{
	if (treeRoot == nullptr)
	{
		return;
	}

	int currDiff = treeRoot->data;

	size_t size = treeRoot->children.size();
	for (size_t i = 0; i < size; i++)
	{
		int diff = 0;
		diff = abs(currDiff - treeRoot->children[i]->data);
		differences.push_back(diff);
		isGracefullyImpl(treeRoot->children[i], differences);
	}
}

bool hasEquals(std::vector<int> differences)
{
	size_t size = differences.size();
	for (size_t i = 0; i < size - 1; i++)
	{
		for (size_t j = i + 1; j < size; j++)
		{
			if (differences[i] == differences[j])
			{
				return true;
			}
		}
	}

	return false;
}

bool Tree::isGracefully(TreeNode* treeRoot)
{
	std::vector<int> differences;

	isGracefullyImpl(treeRoot, differences);

	if (!hasEquals(differences))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string Tree::convertTreeToString(TreeNode* treeRoot)
{
	if (treeRoot == nullptr)
	{
		return "";
	}

	std::string result = "[" + std::to_string(treeRoot->data);

	size_t childrenSize = treeRoot->children.size();
	for (size_t i = 0; i < childrenSize; i++)
	{
		result += convertTreeToString(treeRoot->children[i]);
	}

	return result + "]";
}

TreeNode* Tree::convertStringToTreeImpl(const std::string& stringTree, int& indexForTree)
{
	indexForTree++;
	if (stringTree[indexForTree] == '0')
	{
		indexForTree++;
	}
	std::vector<TreeNode*> treeChildren;
	while (stringTree[indexForTree] == '[')
	{
		treeChildren.push_back(convertStringToTreeImpl(stringTree, indexForTree));
	}
	TreeNode* treeRoot = new TreeNode(0, treeChildren);
	indexForTree++;

	return treeRoot;
}

bool Tree::generateTree(TreeNode* &treeRoot)
{
	size_t count = this->countTreeNodesImpl(treeRoot);

	int* oddArr = new int[count];

	int index = 0;
	for (size_t i = 1; i < count * 2; i += 2)
	{
		oddArr[index] = i;
		index++;
	}

	bool isGracefullyTree = false;
	do
	{
		this->constructTreeFromArr(oddArr, count);

		if (this->isGracefully(this->root))
		{
			isGracefullyTree = true;
			break;
		}

	} while (std::next_permutation(oddArr, oddArr + count));

	delete[] oddArr;

	return isGracefullyTree;

}

void Tree::constructTreeFromArr(int* arr, int arrlen)
{
	int ind = 0;

	constructTreeFromArrImpl(this->root, arr, arrlen, &ind);
}

void Tree::constructTreeFromArrImpl(TreeNode* &treeRoot, int* arr, int arrlen, int* index)
{
	int pindex = (*index);
	if (treeRoot == nullptr && arrlen == (*index))
	{
		return;
	}

	treeRoot->data = arr[pindex];
	(*index)++;

	size_t childrenSize = treeRoot->children.size();
	for (size_t i = 0; i < childrenSize; i++)
	{
		constructTreeFromArrImpl(treeRoot->children[i], arr, arrlen, index);
	}
}

void Tree::convertStringToTree(const std::string& stringTree)
{
	int indexForTree = 0;

	TreeNode* converted = convertStringToTreeImpl(stringTree, indexForTree);

	this->root = converted;

	if (this->generateTree(this->root))
	{
		std::string stringTree = this->convertTreeToString(this->root);
		this->writeGracefullyTreeToTextFile(stringTree);
	}
	else
	{
		std::cout << "Tree cannot be gracefully!\n";
	}
}

void Tree::writeGracefullyTreeToTextFile(const std::string& stringTree)
{
	std::ofstream file("GracefullyTree.txt");

	if (!file)
	{
		std::cout << "Cannot open file with gracefully tree!\n";
	}
	else
	{
		file << stringTree;

		file.close();
	}
}

void Tree::readEmptyTreeFromTextFile(const char* fileName)
{
	std::string fileLineTree;
	std::ifstream file(fileName);

	if (!file)
	{
		std::cout << "Cannot open file with empty tree!\n";
	}
	else
	{
		while (file.good())
		{
			std::getline(file, fileLineTree);
		}

		file.close();
		this->convertStringToTree(fileLineTree);
	}
}

