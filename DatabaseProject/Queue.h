#ifndef QUEUE_H
#define QUEUE_H

#include <string>

class Queue
{
public:
	Queue();
	Queue(const Queue& other);
	Queue& operator=(const Queue& other);
	~Queue();

public:
	void enqueue(std::string data);
	void dequeue();

public:
	std::string getFront() const;
	size_t getSize() const;

	bool isEmpty() const;
	bool isFull() const;

private:
	size_t size;
	size_t capacity;
	std::string* arr;

	size_t front;
	size_t rear;

private:
	void copy(const Queue& other);
	void clear();
	void resize();
};

#endif // !QUEUE_H


