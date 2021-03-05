#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "IntColumn.h"

const int INT_SZ = 10;

IntColumn::IntColumn()
{
	this->intData = 0;
}

IntColumn::IntColumn(int intData)
{
	this->intData = intData;
}

IntColumn* IntColumn::clone() const
{
	return new IntColumn(*this);
}

ColumnType IntColumn::getColumnType() const
{
	return INT;
}

bool IntColumn::makeComparison(const std::string& sign, const void* value) const
{
	if (sign == "==")
	{
		return this->intData == *reinterpret_cast<const int*>(value);
	}
	else if (sign == "!=")
	{
		return this->intData != *reinterpret_cast<const int*>(value);
	}
	else if (sign == ">")
	{
		return this->intData > *reinterpret_cast<const int*>(value);
	}
	else if (sign == ">=")
	{
		return this->intData >= *reinterpret_cast<const int*>(value);
	}
	else if (sign == "<")
	{
		return this->intData < *reinterpret_cast<const int*>(value);
	}

	return this->intData <= *reinterpret_cast<const int*>(value);
}

void IntColumn::print() const
{
	std::cout << std::setfill(' ') << std::setw(INT_SZ);
	std::cout << intData;
}

void IntColumn::printElement() const
{
	std::cout << "|";
	this->print();
	std::cout << "|" << "\n";
}

int IntColumn::getData() const
{
	return this->intData;
}