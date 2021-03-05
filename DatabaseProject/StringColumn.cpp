#include "pch.h"
#include <iostream>
#include <iomanip>
#include "StringColumn.h"

const int STR_SZ = 20;

StringColumn::StringColumn()
{
	this->stringData = "";
}

StringColumn::StringColumn(const std::string& stringData)
{
	this->stringData = stringData;
}

StringColumn* StringColumn::clone() const
{
	return new StringColumn(*this);
}

ColumnType StringColumn::getColumnType() const
{
	return STRING;
}

bool StringColumn::makeComparison(const std::string& sign, const void* value) const
{
	const char* str = reinterpret_cast<const char*>(value);

	if (sign == "==")
	{
		return this->stringData == std::string(str);
	}
	else if (sign == "!=")
	{
		return this->stringData != std::string(str);
	}
	else if (sign == ">")
	{
		return this->stringData > std::string(str);
	}
	else if (sign == ">=")
	{
		return this->stringData >= std::string(str);
	}
	else if (sign == "<")
	{
		return this->stringData < std::string(str);
	}
	
	return this->stringData <= std::string(str);
}

void StringColumn::print() const
{
	if (this->stringData.size() > STR_SZ)
	{
		std::string newStr = this->stringData.substr(0, STR_SZ - 3) + "...";
		std::cout << newStr;
	}
	else
	{
		std::cout << std::setfill(' ') << std::setw(STR_SZ);
		std::cout << this->stringData;
	}
}

void StringColumn::printElement() const
{
	std::cout << "|";
	this->print();
	std::cout << "|" << "\n";
}

const std::string StringColumn::getData() const
{
	return this->stringData;
}