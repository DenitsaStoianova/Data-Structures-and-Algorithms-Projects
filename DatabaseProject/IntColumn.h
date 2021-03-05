#ifndef INT_COLUMN_H
#define INT_COLUMN_H

#include "Column.h"

class IntColumn : public Column
{
public:
	IntColumn();
	IntColumn(int intData);

public:
	bool makeComparison(const std::string& sign, const void* value) const;

	ColumnType getColumnType() const;
	int getData() const;

	IntColumn* clone() const;

	void print() const;
	void printElement() const;

private:
	int intData;
};

#endif // !INT_COLUMN_H
