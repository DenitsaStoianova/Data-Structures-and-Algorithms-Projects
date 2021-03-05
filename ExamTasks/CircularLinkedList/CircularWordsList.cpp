#include "pch.h"
#include "CircularWordsList.h"

CircularWordsList::CircularWordsList()
{
	this->head = nullptr;
	this->size = 0;
}

CircularWordsList::CircularWordsList(const CircularWordsList& other)
{
	copy(other);
}

CircularWordsList& CircularWordsList::operator=(const CircularWordsList& other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}
	return *this;
}

CircularWordsList::~CircularWordsList()
{
	clear();
}

void CircularWordsList::copy(const CircularWordsList& other)
{
	size = other.size;
	head = new Node(other.head->data);

	Node* otherCurrent = other.head;
	Node* copyCurrent = head;

	do
	{
		copyCurrent->next = new Node(otherCurrent->next->data);
		otherCurrent = otherCurrent->next;
		copyCurrent = copyCurrent->next;
	} while (otherCurrent->next != other.head);
	copyCurrent->next = head;
}

void CircularWordsList::clear()
{
	Node* current = head;
	Node* holder = nullptr;
	do
	{
		holder = current;
		current = current->next;
		delete holder;
	} while (current != head);
	head = nullptr;
	size = 0;
}

void CircularWordsList::pushFront(const std::string& data)
{
	Node* newNode = new Node(data);
	if (head == nullptr)
	{
		head = newNode;
		newNode->next = head;
	}
	else
	{
		Node* current = head;
		while (current->next != head)
		{
			current = current->next;
		}
		current->next = newNode;
		newNode->next = head;
		head = newNode;
	}
	size++;
}

void CircularWordsList::pushBack(const std::string& data)
{
	Node* newNode = new Node(data);
	if (head == nullptr)
	{
		head = newNode;
		newNode->next = head;
	}
	else
	{
		Node* current = head;
		while (current->next != head)
		{
			current = current->next;
		}
		current->next = newNode;
		newNode->next = head;
	}
	size++;
}

bool CircularWordsList::haveEqualBeginAndEnd(const std::string& currentWord, const std::string& nextWord) const
{
	return toupper(currentWord[currentWord.size() - 1]) == toupper(nextWord[0]);
}

void CircularWordsList::concatenateWords()
{
	Node* current = head;
	Node* holder = nullptr;
	do
	{
		if (haveEqualBeginAndEnd(current->data, current->next->data))
		{
			holder = current->next;
			current->data += ("-" + current->next->data);
			current->next = holder->next;
			delete holder;
			size--;
		}
		else
		{
			current = current->next;
		}
	} while (current->next != head);

	if (haveEqualBeginAndEnd(current->data, head->data))
	{
		current->data += ("-" + head->data);
		holder = head;
		current->next = holder->next;
		head = current;
		delete holder;
		size--;
	}
}

void CircularWordsList::sortList()
{
	bool areSwapped = false;
	Node* current = nullptr;
	std::string temp;
	for (int i = 0; i < size - 1; i++)
	{
		current = head;
		areSwapped = false;
		for (int j = 0; j < size - 1 - i; j++)
		{
			if (current->data > current->next->data)
			{
				temp = current->data;
				current->data = current->next->data;
				current->next->data = temp;
				areSwapped = true;
			}
			current = current->next;
		}

		if (!areSwapped)
		{
			break;
		}
	}
}

void CircularWordsList::findLongestWord() const
{
	Node* current = head;
	Node* longestSizeNode = current;
	do
	{
		if (current->next->data.size() > longestSizeNode->data.size())
		{
			longestSizeNode = current->next;
		}
		current = current->next;
	} while (current->next != head);
	std::cout << "Longest: " << longestSizeNode->data << "\n";
}

void CircularWordsList::print() const
{
	if (head == nullptr)
	{
		std::cout << "List is empty!\n";
		return;
	}

	Node* current = head;
	do
	{
		std::cout << "->" << current->data;
		current = current->next;
	} while (current != head);
	std::cout << "->\n";
}
