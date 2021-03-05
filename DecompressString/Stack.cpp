#include "pch.h"
#include <iostream>
#include "Stack.h"

Stack::Stack()
{
	this->top = 0;
	this->capacity = 8;
	this->arr = new int[this->capacity];
}

Stack::Stack(const Stack& other)
{
	this->copy(other);
}

Stack& Stack::operator=(const Stack& other)
{
	if (this != &other)
	{
		this->del();
		this->copy(other);
	}

	return *this;
}

Stack::~Stack()
{
	this->del();
}

bool Stack::isEmpty() const
{
	return this->top == 0;
}

bool Stack::isFull() const
{
	return this->top >= this->capacity;
}

void Stack::push(int x)
{
	if (this->isFull())
	{
		this->resize();
	}

	this->arr[this->top++] = x;
}

void Stack::copy(const Stack& other)
{
	this->top = other.top;
	this->capacity = other.capacity;
	this->arr = new int[this->capacity];

	for (int i = 0; i < this->top; i++)
	{
		this->arr[i] = other.arr[i];
	}
}

void Stack::del()
{
	delete[] this->arr;
}

void Stack::resize()
{
	this->capacity *= 2;
	int* temp = new int[this->capacity];

	for (int i = 0; i < this->top; i++)
	{
		temp[i] = this->arr[i];
	}

	delete[] this->arr;
	this->arr = temp;
}

void Stack::pop()
{
	if (this->isEmpty())
	{
		std::cout << "Stack is empty.\n";
	}

	this->arr[--this->top];
}

int Stack::getTop() const
{
	return this->arr[this->top - 1];
}