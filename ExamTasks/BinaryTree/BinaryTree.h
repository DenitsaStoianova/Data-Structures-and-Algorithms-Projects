#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct TreeNode
{
	int data;
	TreeNode* left;
	TreeNode* right;

	TreeNode(int data, TreeNode* left = nullptr, TreeNode* right = nullptr)
	{
		this->data = data;
		this->left = left;
		this->right = right;
	}
};

class BinaryTree
{
public:
	BinaryTree();
	BinaryTree(const BinaryTree& other);
	BinaryTree& operator=(const BinaryTree& other);
	~BinaryTree();

	void readTreeFromFile(const char* fileName);
	void writeTreeToFile(const char* fileName);

	bool isBST() const;
	bool isBalanced() const;
	void removeWrongNodes();

private:
	TreeNode* root;

	TreeNode* copy(TreeNode* otherRoot);
	void clear(TreeNode* treeRoot);

	bool isBSTHelp(TreeNode* treeRoot, int minData, int maxData) const;
	bool isBalancedTree(TreeNode* treeRoot) const;
	bool isPerfectlyBalanced(TreeNode* root) const;
	int findHeight(TreeNode* treeRoot) const;
	int findCountOfNodes(TreeNode* treeRoot) const;

	TreeNode* getMinNode(TreeNode* treeRoot) const;
	TreeNode* removeNodeFromTree(TreeNode* &treeRoot, TreeNode* &nodeToDel);
	void removeWrongNodesHelp(TreeNode* &treeRoot);

private:
	bool checkStringTree(const std::string& stringTree) const;
	void constructTreeFromString(const std::string& stringTree);
	void constructTreeFromStringHelp(TreeNode* &treeRoot, const std::string& stringTree, int& index);
	void convertTreeToStringHelp(TreeNode* &treeRoot, std::string& stringTree);
	std::string convertTreeToString();

private:
	void skipSymbols(const std::string& str, int& index);
	int convertToNum(const std::string& str, int& index);
	void convertNumberToString(std::string& str, int number);
	void reverseString(std::string& res, int count);
};
