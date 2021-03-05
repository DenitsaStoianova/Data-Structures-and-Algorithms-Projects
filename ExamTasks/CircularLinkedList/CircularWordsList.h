#pragma once
#include <string>
#include <iostream>

struct Node
{
	std::string data;
	Node* next;

	Node(const std::string& data, Node* next = nullptr)
	{
		this->data = data;
		this->next = next;
	}
};

class CircularWordsList
{
public:
	CircularWordsList();
	CircularWordsList(const CircularWordsList& other);
	CircularWordsList& operator=(const CircularWordsList& other);
	~CircularWordsList();

	void pushFront(const std::string& data);
	void pushBack(const std::string& data);

	void concatenateWords();
	void sortList();

	void findLongestWord() const;
	void print() const;

private:
	Node* head;
	int size;

	void copy(const CircularWordsList& other);
	void clear();

	bool haveEqualBeginAndEnd(const std::string& currentWord, const std::string& nextWord) const;
};
