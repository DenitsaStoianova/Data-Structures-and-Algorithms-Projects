#ifndef GRACEFULLY_TREE_H
#define GRACEFULLY_TREE_H

#include <vector>
#include <string>

struct TreeNode
{
	int data;
	std::vector<TreeNode*> children;

	TreeNode() {}

	TreeNode(int data, std::vector<TreeNode*> children)
	{
		this->data = data;
		this->children = children;
	}
};

class Tree
{
public:
	Tree();
	Tree(const Tree& other);
	Tree& operator=(const Tree& other);
	~Tree();

	void readEmptyTreeFromTextFile(const char* fileName);

private:
	TreeNode* root;

	TreeNode* copy(TreeNode* otherRoot);
	void clear(TreeNode* treeRoot);

	void isGracefullyImpl(TreeNode* treeRoot, std::vector<int>& differences);
	bool isGracefully(TreeNode* treeRoot);

	std::string convertTreeToString(TreeNode* treeRoot);

	void writeGracefullyTreeToTextFile(const std::string& stringTree);

	void convertStringToTree(const std::string& stringTree);
	TreeNode* convertStringToTreeImpl(const std::string& stringTree, int& indexForTree);

	size_t countTreeNodesImpl(TreeNode* treeNode) const;

	bool generateTree(TreeNode* &treeRoot);

	void constructTreeFromArr(int* arr, int arrlen);
	void constructTreeFromArrImpl(TreeNode* &treeRoot, int* arr, int arrlen, int* index);
};


#endif // !GRACEFULLY_TREE_H
