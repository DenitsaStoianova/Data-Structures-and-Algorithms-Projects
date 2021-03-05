#include "pch.h"
#include <iostream>
#include "Stack.h"

Stack::Stack()
{
	this->top = 0;
	this->capacity = 8;
	this->arr = new std::string[this->capacity];
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

void Stack::push(const std::string& str)
{
	if (this->isFull())
	{
		this->resize();
	}

	this->arr[this->top++] = str;
}

void Stack::copy(const Stack& other)
{
	this->top = other.top;
	this->capacity = other.capacity;
	this->arr = new std::string[this->capacity];

	for (size_t i = 0; i < this->top; i++)
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
	std::string* temp = new std::string[this->capacity];

	for (size_t i = 0; i < this->top; i++)
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
		return;
	}

	this->arr[--this->top];
}

const std::string Stack::getTop() const
{
	return this->arr[this->top - 1];
}

size_t Stack::getNumOfElements() const
{
	return this->top;
}