#include "pch.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "ColumnsInfo.h"

const int INT_SZ = 10;
const int STR_SZ = 20;

ColumnsInfo::ColumnsInfo()
{
	this->size = 0;
	this->capacity = 0;
}

ColumnsInfo::ColumnsInfo(const ColumnsInfo& other)
{
	this->copy(other);
}

ColumnsInfo& ColumnsInfo::operator=(const ColumnsInfo& other)
{
	if (this != &other)
	{
		this->clear();
		this->copy(other);
	}
	return *this;
}

ColumnsInfo::~ColumnsInfo()
{
	this->clear();
}

void ColumnsInfo::createColumnsFromString(std::string& columnsInfoStr)
{
	ColumnStruct colStruct;
	colStruct.name = "Index";
	colStruct.type = ColumnStruct::Type::INT;

	this->addNewColumn(colStruct);

	size_t size = columnsInfoStr.size();
	for (size_t i = 1; i < size - 1; i++)
	{
		std::string pair = "";

		while (columnsInfoStr[i] != ',' && i != size - 1)
		{
			if (columnsInfoStr[i] == ' ')
			{
				i++;
			}

			pair += columnsInfoStr[i];
			i++;
		}

		std::string name = pair.substr(0, pair.find(':'));
		std::string type = pair.substr(pair.find(':') + 1);

		colStruct.name = name;

		if (type == "String")
		{
			colStruct.type = ColumnStruct::Type::STRING;
		}
		else
		{
			colStruct.type = ColumnStruct::Type::INT;
		}

		this->addNewColumn(colStruct);
	}
}

void ColumnsInfo::setCapacity(size_t capacity)
{
	this->capacity = capacity;
	this->columnsInfo = new ColumnStruct[this->capacity];
}

void ColumnsInfo::addNewColumn(const ColumnStruct& newColumn)
{
	this->columnsInfo[this->size] = newColumn;
	this->size++;
}

const std::string ColumnsInfo::getColumnName(size_t index) const
{
	return this->columnsInfo[index].name;
}

size_t ColumnsInfo::getSize() const
{
	return this->size;
}

ColumnStruct::Type ColumnsInfo::getColumnType(size_t index) const
{
	return this->columnsInfo[index].type;
}

void ColumnsInfo::copy(const ColumnsInfo& other)
{
	this->size = other.size;
	this->capacity = other.capacity;
	this->columnsInfo = new ColumnStruct[this->capacity];

	for (size_t i = 0; i < this->size; i++)
	{
		this->columnsInfo[i] = other.columnsInfo[i];
	}
}

void ColumnsInfo::clear()
{
	delete[] this->columnsInfo;
}

const ColumnStruct& ColumnsInfo::operator[](int index) const
{
	return this->columnsInfo[index];
}

ColumnStruct& ColumnsInfo::operator[](int index)
{
	return this->columnsInfo[index];
}

void ColumnsInfo::print() const
{
	int symbolCount = 0;
	for (size_t i = 1; i < this->size; i++)
	{
		if (this->columnsInfo[i].type == ColumnStruct::Type::INT)
		{
			symbolCount += INT_SZ;
		}
		else
		{
			symbolCount += STR_SZ;
		}
	}

	for (size_t i = 1; i < this->size; i++)
	{
		std::cout << "|";
		if (this->columnsInfo[i].type == ColumnStruct::Type::INT)
		{
			std::cout << std::setfill(' ') << std::setw(INT_SZ);
		}
		else
		{
			std::cout << std::setfill(' ') << std::setw(STR_SZ);
		}
		std::cout << this->columnsInfo[i].name;

		if (i == this->size - 1)
		{
			std::cout << "|";
		}
	}
	std::cout << "\n";

	std::cout << std::setfill('-') << std::setw(symbolCount + this->size + 1) << "\n";
}

void ColumnsInfo::printColumnName(size_t index) const
{
	std::cout << "|";
	if (this->columnsInfo[index].type == ColumnStruct::Type::INT)
	{
		std::cout << std::setfill(' ') << std::setw(INT_SZ);
		std::cout << this->columnsInfo[index].name << "|" << "\n";

		std::cout << std::setfill('-') << std::setw(INT_SZ + 3) << "\n";
	}
	else
	{
		std::cout << std::setfill(' ') << std::setw(STR_SZ);
		std::cout << this->columnsInfo[index].name << "|" << "\n";

		std::cout << std::setfill('-') << std::setw(STR_SZ + 3) << "\n";
	}
}

bool ColumnsInfo::hasColumn(const std::string& columnName) const
{
	for (size_t i = 0; i < this->size; i++)
	{
		if (this->columnsInfo[i].name == columnName)
		{
			return true;
		}
	}
	return false;
}

size_t ColumnsInfo::getColumnIndex(const std::string& columnName) const
{
	for (size_t i = 0; i < this->size; i++)
	{
		if (this->columnsInfo[i].name == columnName)
		{
			return i;
		}
	}
	return 0;
}