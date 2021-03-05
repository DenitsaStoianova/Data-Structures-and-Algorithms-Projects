#pragma once
#include "pch.h"
#include <iostream>
#include "Queue.h"

Queue::Queue()
{
	this->front = -1;
	this->rear = -1;

	this->size = 0;
	this->capacity = 4;
	this->arr = new std::string[this->capacity];
}

Queue::Queue(const Queue& other)
{
	this->copy(other);
}

Queue& Queue::operator=(const Queue& other)
{
	if (this != &other)
	{
		this->clear();
		this->copy(other);
	}
	return *this;
}

Queue::~Queue()
{
	this->clear();
}

void Queue::copy(const Queue& other)
{
	this->front = other.front;
	this->rear = other.rear;
	this->size = other.size;
	this->capacity = other.capacity;
	this->arr = new std::string[this->capacity];
	for (size_t i = 0; i <= this->rear; i++)
	{
		this->arr[i] = other.arr[i];
	}
}

void Queue::clear()
{
	delete[] this->arr;
}

void Queue::resize()
{
	this->capacity *= 2;
	std::string* temp = new std::string[this->capacity];
	for (size_t i = 0; i <= this->rear; i++)
	{
		temp[i] = this->arr[i];
	}
	delete[] this->arr;
	this->arr = temp;
}

void Queue::enqueue(std::string x)
{
	if (this->isFull())
	{
		this->resize();
	}

	if (this->isEmpty())
	{
		this->front = 0;
		this->rear = 0;
	}
	else
	{
		this->rear++;
	}

	this->arr[this->rear] = x;
	this->size++;
}

void Queue::dequeue()
{
	if (this->isEmpty())
	{
		std::cout << "Queue is empty.\n";
	}

	if (this->front == this->rear) 
	{
		this->front = -1;
		this->rear = -1;
	}
	else
	{
		this->front++;
	}
	this->size--;
}

std::string Queue::getFront() const
{
	if (this->isEmpty())
	{
		std::cout << "Queue is empty.\n";
	}

	return this->arr[this->front];
}

size_t Queue::getSize() const
{
	return this->size;
}

bool Queue::isEmpty() const
{
	return this->front == -1;
}

bool Queue::isFull() const
{
	return this->rear == this->capacity - 1;
}