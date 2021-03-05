#ifndef ROW_DATA_H
#define ROW_DATA_H

#include "Column.h"
#include "Queue.h"
#include "Stack.h"

class RowData
{
public:
	RowData(size_t capacity);
	RowData(const RowData& other);
	RowData& operator=(const RowData& other);
	~RowData();

public:
	void addColumnToRow(const Column& newColumn);

public:
	bool isFull() const;
	size_t getSize() const;

	const Column& operator[](size_t index) const;
	Column& operator[](size_t index);

	void print() const;

private:
	Column** row;
	size_t size;
	size_t capacity;

private:
	void copy(const RowData& other);
	void clear();
	void resize();
};


#endif // !ROW_DATA_H
