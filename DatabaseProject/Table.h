#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <fstream>
#include "ColumnsInfo.h"
#include "RowData.h"
#include "Queue.h"
#include "IntColumn.h"
#include "StringColumn.h"
#include "Stack.h"
#include "DynamicArray.h"

class Table
{
public:
	Table(const std::string& tableName); 

public:
	void addInsertDataToTableFile(std::string& rowToInsert, const std::string& tableName);
	void readFromTableFile(const std::string& columnNameToSelect, Queue& commandsQueue);
	void readFromFileAndSort(const std::string& columnNameToSelect, Queue& commandsQueue, const std::string& columnForOrder);

public:
	size_t getNumberOfColumns() const;
	bool checkElement(const std::string& element, size_t colIndex) const;
	bool hasColumn(const std::string& columnName) const;

	ColumnStruct::Type getColumnType(const std::string& columnName) const;

public:
	void removeMarkedDataFromFile();
	void getRowsFromRowsInfoFile();
	void saveRowsSizesToInfoFile();

private:
	std::string tableName; 
	ColumnsInfo columnsInfo;
	DynamicArray<int> rowsSizeArr;

private:
	void readStringFromFile(const std::string& fileName);
	size_t getNumberOfColumns(const std::string& columnsStr) const;

	int getNumOfElInTableRowInfoFile();

private:
	bool checkExpression(const RowData& row, Queue& commandsQueue);

	void makeSelectCommand(const RowData& row, Queue& commandsQueue, const std::string& columnNameToSelect, int& countForPrint);
	void makeRemoveCommand(const RowData& row, Queue& commandsQueue, size_t& countForIndex, int& countRemovedRows);
	void readRowsFromFile(DynamicArray<int>& indexesArr, const std::string& columnNameToSelect, size_t columnIndex);

	void makeSort(DynamicArray<int>& arrForIndexes, DynamicArray<std::string>& arrForColumns, bool isDigit) const;
	bool checkCommandsFromStack(Stack& stack) const;
	void printForSelect(const RowData& row, const std::string& columnNameToSelect, size_t check, size_t columnIndex, size_t indexToCheck) const;

private:
	int readRowsFromFileSum(int number) const;
	int removeMarkedDataSum(int index) const;
	void removeNegativeSizes(DynamicArray<int>& arr);
};

#endif // !TABLE_H
