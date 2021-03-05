#include "pch.h"
#include <iostream>
#include <string>
#include "RowData.h"

RowData::RowData(size_t capacity)
{
	this->size = 0;
	this->capacity = capacity;
	this->row = new Column*[this->capacity];
}

RowData::RowData(const RowData& other)
{
	this->copy(other);
}

RowData& RowData::operator=(const RowData& other)
{
	if (this != &other)
	{
		this->clear();
		this->copy(other);
	}
	return *this;
}

RowData::~RowData()
{
	this->clear();
}

void RowData::copy(const RowData& other)
{
	this->size = other.size;
	this->capacity = other.capacity;
	this->row = new Column*[this->capacity];

	for (size_t i = 0; i < this->size; i++)
	{
		this->row[i] = other.row[i]->clone();
	}
}

void RowData::clear()
{
	for (size_t i = 0; i < this->size; i++)
	{
		delete this->row[i];
	}
	delete[] this->row;
}

bool RowData::isFull() const
{
	return this->size == this->capacity;
}

void RowData::resize()
{
	this->capacity *= 2;
	Column** temp = new Column*[this->capacity];

	for (size_t i = 0; i < this->size; i++)
	{
		temp[i] = this->row[i];
	}
	delete[] this->row;
	this->row = temp;
}

void RowData::addColumnToRow(const Column& newColumn)
{
	if (this->isFull())
	{
		this->resize();
	}

	this->row[this->size] = newColumn.clone();
	this->size++;
}

size_t RowData::getSize() const
{
	return this->size;
}

const Column& RowData::operator[](size_t index) const
{
	return *this->row[index];
}

Column& RowData::operator[](size_t index)
{
	return *this->row[index];
}

void RowData::print() const
{
	for (size_t i = 1; i < this->size; i++)
	{
		std::cout << "|";
		this->row[i]->print();
	}
	std::cout << "|" << "\n";
}

