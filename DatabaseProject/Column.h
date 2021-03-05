#ifndef COLUMN_H
#define COLUMN_H

#include <fstream>

enum ColumnType
{
	INT, STRING
};

class Column
{
public:
	virtual ColumnType getColumnType() const = 0;

	virtual bool makeComparison(const std::string& sign, const void* value) const = 0;

	virtual Column* clone() const = 0;

	virtual void print() const = 0; 
	virtual void printElement() const = 0;

	virtual ~Column() {};
};


#endif // !COLUMN_H
