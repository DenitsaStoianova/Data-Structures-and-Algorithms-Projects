#include "pch.h"
#include "BinaryTree.h"

BinaryTree::BinaryTree()
{
	this->root = nullptr;
}

TreeNode* BinaryTree::copy(TreeNode* otherRoot)
{
	if (otherRoot == nullptr)
	{
		return nullptr;
	}

	TreeNode* copyRoot = new TreeNode(otherRoot->data);
	copyRoot->left = this->copy(otherRoot->left);
	copyRoot->right = this->copy(otherRoot->right);
	return copyRoot;
}

void BinaryTree::clear(TreeNode* treeRoot)
{
	if (treeRoot == nullptr)
	{
		return;
	}
	this->clear(treeRoot->left);
	this->clear(treeRoot->right);
	delete treeRoot;
}

BinaryTree::BinaryTree(const BinaryTree& other)
{
	this->root = this->copy(other.root);
}

BinaryTree& BinaryTree::operator=(const BinaryTree& other)
{
	if (this != &other)
	{
		this->clear(this->root);
		this->root = this->copy(other.root);
	}
	return *this;
}

BinaryTree::~BinaryTree()
{
	this->clear(this->root);
}

bool BinaryTree::isBST() const
{
	return isBSTHelp(root, INT_MIN, INT_MAX);
}

bool BinaryTree::isBSTHelp(TreeNode* treeRoot, int minData, int maxData) const
{
	bool result = true;
	if (treeRoot->left == nullptr && treeRoot->right == nullptr)
	{
		return treeRoot->data >= minData && treeRoot->data < maxData;
	}

	if (treeRoot->left != nullptr)
	{
		result = result && isBSTHelp(treeRoot->left, minData, treeRoot->data);
	}

	if (treeRoot->right != nullptr)
	{
		result = result && isBSTHelp(treeRoot->right, treeRoot->data, maxData);
	}

	return result;
}

bool BinaryTree::isBalanced() const
{
	return isBalancedTree(root);
}

bool BinaryTree::isBalancedTree(TreeNode* treeRoot) const
{
	if (treeRoot == nullptr)
	{
		return true;
	}

	if (std::abs(findHeight(treeRoot->left) - findHeight(treeRoot->right)) > 1)
	{
		return false;
	}

	return isBalancedTree(treeRoot->left) && isBalancedTree(treeRoot->right);
}

int BinaryTree::findHeight(TreeNode* treeRoot) const
{
	if (treeRoot == nullptr)
	{
		return 0;
	}

	int leftHeight = findHeight(treeRoot->left);
	int rightHeight = findHeight(treeRoot->right);

	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int BinaryTree::findCountOfNodes(TreeNode* treeRoot) const
{
	if (treeRoot == nullptr)
	{
		return 0;
	}
	return 1 + findCountOfNodes(treeRoot->left) + findCountOfNodes(treeRoot->right);
}

TreeNode* BinaryTree::getMinNode(TreeNode* treeRoot) const
{
	TreeNode* currentNode = treeRoot;
	while (currentNode != nullptr && currentNode->left != nullptr)
	{
		currentNode = currentNode->left;
	}
	return currentNode;
}

TreeNode* BinaryTree::removeNodeFromTree(TreeNode* &treeRoot, TreeNode* &nodeToDel)
{
	if (treeRoot == nullptr)
	{
		return nullptr;
	}

	if (treeRoot == nodeToDel)
	{
		if (treeRoot->left == nullptr)
		{
			TreeNode* tempNodeToDelete = treeRoot->right;
			delete treeRoot;
			return tempNodeToDelete;
		}
		else if (treeRoot->right == nullptr)
		{
			TreeNode* tempNodeToDelete = treeRoot->left;
			delete treeRoot;
			return tempNodeToDelete;
		}
		else
		{
			TreeNode* tempNodeToDelete = getMinNode(treeRoot->right);
			treeRoot->data = tempNodeToDelete->data;
			treeRoot->right = removeNodeFromTree(treeRoot->right, tempNodeToDelete);
		}
	}

	treeRoot->left = removeNodeFromTree(treeRoot->left, nodeToDel);
	treeRoot->right = removeNodeFromTree(treeRoot->right, nodeToDel);

	return treeRoot;
}

void BinaryTree::removeWrongNodes()
{
	removeWrongNodesHelp(root);
}

void BinaryTree::removeWrongNodesHelp(TreeNode* &treeRoot)
{
	if (treeRoot == nullptr || (treeRoot->left == nullptr && treeRoot->right == nullptr))
	{
		return;
	}

	if (treeRoot->left != nullptr && treeRoot->left->data > treeRoot->data)
	{
		treeRoot = removeNodeFromTree(treeRoot, treeRoot);
	}

	if (treeRoot->right != nullptr && treeRoot->right->data < treeRoot->data)
	{
		removeNodeFromTree(treeRoot, treeRoot->right);
	}

	removeWrongNodesHelp(treeRoot->left);
	removeWrongNodesHelp(treeRoot->right);
}

bool BinaryTree::checkStringTree(const std::string& stringTree) const
{
	size_t size = stringTree.size();
	int bracketsCount = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (stringTree[i] == '(')
		{
			if (i != 0 && stringTree[i - 1] != ' ')
			{
				return false;
			}
			if (stringTree[i + 1] != ')' && (stringTree[i + 1] < '0' || stringTree[i + 1] > '9'))
			{
				return false;
			}
			bracketsCount++;
			i++;
		}
		if (stringTree[i] == ')')
		{
			if (bracketsCount == 0)
			{
				return false;
			}
			bracketsCount--;
		}
	}

	if (bracketsCount != 0)
	{
		return false;
	}
	return true;
}

void BinaryTree::readTreeFromFile(const char* fileName)
{
	std::string stringTree;
	std::ifstream file(fileName);

	if (!file)
	{
		std::cout << "Unable to open file with string representation of tree!\n";
		return;
	}

	while (file.good())
	{
		std::getline(file, stringTree);
	}
	file.close();

	if (checkStringTree(stringTree))
	{
		constructTreeFromString(stringTree);
	}
	else
	{
		std::cout << "String representation of tree is not valid!\n";
	}
}

void BinaryTree::writeTreeToFile(const char* fileName)
{
	std::ofstream file(fileName);
	std::string stringTree = convertTreeToString();

	if (!file)
	{
		std::cout << "Cannot open file!\n";
		return;
	}

	file << stringTree;
	file.close();
}

void BinaryTree::constructTreeFromStringHelp(TreeNode* &root, const std::string& stringTree, int& index)
{
	if (stringTree[index] == '(' && stringTree[index + 1] != ')')
	{
		index++;
	}
	if (index == stringTree.size() - 1 || (stringTree[index] == '(' && stringTree[++index] == ')'))
	{
		skipSymbols(stringTree, index);
		return;
	}

	int charToNum = convertToNum(stringTree, index);
	root = new TreeNode(charToNum);
	constructTreeFromStringHelp(root->left, stringTree, index);
	constructTreeFromStringHelp(root->right, stringTree, index);
}

void BinaryTree::constructTreeFromString(const std::string& stringTree)
{
	int index = 0;
	constructTreeFromStringHelp(root, stringTree, index);
}

void BinaryTree::convertTreeToStringHelp(TreeNode* &root, std::string& stringTree)
{
	if (root == nullptr)
	{
		stringTree += " ()";
		return;
	}

	stringTree += stringTree.empty() ? "(" : " (";
	convertNumberToString(stringTree, root->data);

	convertTreeToStringHelp(root->left, stringTree);
	convertTreeToStringHelp(root->right, stringTree);
	stringTree += ")";
}

std::string BinaryTree::convertTreeToString()
{
	std::string stringTree;
	convertTreeToStringHelp(root, stringTree);
	return stringTree;
}

int BinaryTree::convertToNum(const std::string& str, int& index)
{
	int charToNum = 0;
	while (str[index] != ' ')
	{
		charToNum = (charToNum * 10) + (str[index] - '0');
		index++;
	}
	index++;
	return charToNum;
}

void BinaryTree::reverseString(std::string& resultString, int count)
{
	char holder;
	for (int i = 0; i < count / 2; i++)
	{
		holder = resultString[i];
		resultString[i] = resultString[count - 1 - i];
		resultString[count - 1 - i] = holder;
	}
}

void BinaryTree::convertNumberToString(std::string& str, int number)
{
	int count = 0;
	std::string res;
	while (number != 0)
	{
		res += number % 10 + '0';
		number /= 10;
		count++;
	}
	reverseString(res, count);
	str += res;
}

void BinaryTree::skipSymbols(const std::string& str, int& index)
{
	int size = str.size();
	while (index != size - 1 && str[index] != '(')
	{
		index++;
	}
}