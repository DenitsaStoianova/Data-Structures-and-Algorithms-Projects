#ifndef STACK_H
#define STACK_H

#include <string>

class Stack
{
public:
	Stack();
	Stack(const Stack& other);
	Stack& operator=(const Stack& other);
	~Stack();

public:
	void push(const std::string& str);
	void pop();

public:
	const std::string getTop() const;
	size_t getNumOfElements() const;

	bool isEmpty() const;
	bool isFull() const;

private:
	std::string* arr;
	size_t top;
	size_t capacity;

private:
	void copy(const Stack& other);
	void del();
	void resize();
};


#endif // !STACK_H

