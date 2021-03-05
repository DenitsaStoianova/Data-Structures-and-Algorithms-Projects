#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <fstream>
#include "Queue.h"
#include "Table.h"
#include "DynamicArray.h"

class Database
{
public:
	void createTable(std::string& createInfo);
	void dropTable(const std::string& tableName);

	void listTables();
	void tableInfo(const std::string& tableName);

	void insert(std::string& insertInfo);

	void select(std::string& selectInfo);

	void remove(std::string& removeInfo);
	void removeData(const std::string& tableName);

private:
	std::fstream tablesInfoFile;

private:
	bool checkIfTableExists(const std::string& tableNameToCheck);
	void splitStringForCommand(std::string& commandInfo, Queue& queue);

	void makeSelectOrRemove(std::string& commandInfo, const std::string& tableName, const std::string& columnNameToSelect);
	void makeSelectWithOrderBy(std::string& commandInfo, const std::string& columnNameToSelect, const std::string& tableName, size_t pos);

private:
	void addColumnsInfoToTableFile(const std::string& tableName, const std::string& createInfo);
	void addTableNameToTablesInfoFile(const std::string& tableName);
	void addColumnsInfoStrSizeToTableInfoFile(int sizeToAdd, const std::string& tableName);

private:
	void readColumnsInfoStrFromFile(const std::string& tableName);
	void sumSizesFromTableInfoFile(const std::string& tableName);
	int getNumOfElInTableRowInfoFile(const std::string& tableName);

	void removeTableNameFromTablesInfoFile(const std::string& tableNameToRemove);

private:
	bool isValidCreateTable(std::string& createInfo);
	bool isValidInsert(Table& table, std::string& insertInfo);
	bool isValidCommand(Table& table, std::string& commandInfo, const std::string& columnNameToSelect);

	bool isSign(const std::string& str) const;
	bool isDigit(char c) const;
};

#endif // !DATABASE_H
