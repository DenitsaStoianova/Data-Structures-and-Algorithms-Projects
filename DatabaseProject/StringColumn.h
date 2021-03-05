#ifndef STRING_COLUMN_H
#define STRING_COLUMN_H

#include <string>
#include "Column.h"

class StringColumn : public Column
{
public:
	StringColumn();
	StringColumn(const std::string& stringData);

public:
	bool makeComparison(const std::string& sign, const void* value) const;

	ColumnType getColumnType() const;
	const std::string getData() const;

	StringColumn* clone() const;

	void print() const;
	void printElement() const;

private:
	std::string stringData;
};


#endif // !STRING_COLUMN_H
