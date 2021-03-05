#ifndef STACK_H
#define STACK_H

class Stack
{
public:
	Stack();
	Stack(const Stack& other);
	Stack& operator=(const Stack& other);
	~Stack();

	bool isEmpty() const;
	bool isFull() const;

	int getTop() const;

	void push(int x);
	void pop();

private:
	int* arr;
	int top;
	int capacity;

	void copy(const Stack& other);
	void del();
	void resize();
};


#endif // !STACK_H

