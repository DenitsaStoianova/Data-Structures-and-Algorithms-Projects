#ifndef COLUMNS_INFO_H
#define COLUMNS_INFO_H

#include <vector>
#include <string>
#include "DynamicArray.h"

struct ColumnStruct
{
	std::string name;
	enum Type { STRING, INT } type;
};

class ColumnsInfo
{
public:
	ColumnsInfo();
	ColumnsInfo(const ColumnsInfo& other);
	ColumnsInfo& operator=(const ColumnsInfo& other);
	~ColumnsInfo();

public:
	size_t getSize() const;

	const ColumnStruct& operator[](int index) const;
	ColumnStruct& operator[](int index);

	ColumnStruct::Type getColumnType(size_t index) const;

	bool hasColumn(const std::string& columnName) const;
	size_t getColumnIndex(const std::string& columnName) const;
	const std::string getColumnName(size_t index) const;

	void print() const;
	void printColumnName(size_t index) const;

public:
	void setCapacity(size_t capacity);
	void createColumnsFromString(std::string& columnsInfo);

private:
	ColumnStruct* columnsInfo;
	size_t size;
	size_t capacity;

private:
	void copy(const ColumnsInfo& other);
	void clear();

	void addNewColumn(const ColumnStruct& newColumn);
};



#endif // !COLUMNS_INFO_H
