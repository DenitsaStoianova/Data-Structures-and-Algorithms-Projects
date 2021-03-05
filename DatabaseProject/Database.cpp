#include "pch.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "Database.h"

const std::string FILE_NAME = "TablesInfo.txt";

void Database::addColumnsInfoToTableFile(const std::string& tableName, const std::string& createInfo)
{
	std::string fileName = tableName + ".dat";

	std::ofstream file;
	file.open(fileName, std::ios::binary | std::ios::out);

	unsigned strLength = createInfo.size();
	if (!file)
	{
		std::cout << "Cannot open " << fileName << " text file!\n";
	}
	else
	{
		file.write((const char*)&strLength, sizeof(strLength));
		file.write(createInfo.c_str(), strLength);
	}
	file.close();

	strLength += sizeof(strLength);

	this->addColumnsInfoStrSizeToTableInfoFile(strLength, tableName);
}

void Database::addTableNameToTablesInfoFile(const std::string& tableName)
{
	tablesInfoFile.open(FILE_NAME, std::ios::out | std::ios::app);

	if (!tablesInfoFile)
	{
		std::cout << "Cannot open TablesInfo.txt file!\n";
	}
	else
	{
		std::string tableNameToFile = tableName + ".dat";
		tablesInfoFile << tableNameToFile << "\n";

		tablesInfoFile.close();
	}
}

void Database::addColumnsInfoStrSizeToTableInfoFile(int sizeToAdd, const std::string& tableName)
{
	std::string fileRowsName = tableName + "_Info.dat";
	std::fstream file;
	file.open(fileRowsName, std::ios::out | std::ios::binary);

	if (!file)
	{
		std::cout << "Cannot open " << fileRowsName << " file!\n";
	}
	else
	{
		file.write((const char*)&sizeToAdd, sizeof(sizeToAdd));

		file.close();
	}
}

bool Database::isValidCreateTable(std::string& createInfo)
{
	size_t size = createInfo.size();
	if (createInfo[0] != '(' || createInfo[size - 1] != ')')
	{
		std::cout << "Invalid syntax of CreateTable command, you must have opening and closed brackets!\n";
		return false;
	}

	for (size_t i = 1; i < size - 2; i++)
	{
		std::string pair = "";
		bool found = false;
		while (createInfo[i] != ',' && createInfo[i + 1] != ' ' && i != size - 1)
		{
			if (createInfo[i] == ' ')
			{
				i++;
			}

			if (createInfo[i] == ':')
			{
				found = true;
			}

			pair += createInfo[i];
			i++;
		}

		if (createInfo[i] != ')')
		{
			if (createInfo[i] != ',' || createInfo[i + 1] != ' ')
			{
				std::cout << "Invalid syntax of CreateTable command, you must have symbols ',' and ' ' between information of every column!\n";
				return false;
			}
		}

		if (!found)
		{
			std::cout << "Invalid syntax of CreateTable command, you must have symbol ':' between name and type of every column!\n";
			return false;
		}
		else
		{
			std::string type = pair.substr(pair.find(':') + 1);
			if (type != "Int" && type != "String")
			{
				std::cout << "Invalid syntax of CreateTable command, column type cannot be different from String and Int!\n";
				return false;
			}
		}
	}
	return true;
}

void Database::createTable(std::string& createInfo)
{
	std::string tableName = createInfo.substr(0, createInfo.find(' '));
	createInfo.erase(0, tableName.size() + 1);

	if (this->isValidCreateTable(createInfo))
	{
		this->addColumnsInfoToTableFile(tableName, createInfo);

		this->addTableNameToTablesInfoFile(tableName);

		std::cout << "Table " << tableName << " created!\n";
	}
}

void Database::dropTable(const std::string& tableName)
{
	if (this->checkIfTableExists(tableName))
	{
		std::string fileName = tableName + ".dat";
		std::experimental::filesystem::remove(fileName);

		this->removeTableNameFromTablesInfoFile(tableName);

		std::string rowsFileName = tableName + "_Info.dat";
		std::experimental::filesystem::remove(rowsFileName);

		std::cout << "Table " << tableName << " was removed from database.\n";
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}

void Database::removeTableNameFromTablesInfoFile(const std::string& tableName)
{
	std::string tableNameToRemove = tableName + ".dat";
	std::string lineFromFile;

	tablesInfoFile.open(FILE_NAME, std::ios::in);

	std::ofstream tempFile;
	tempFile.open("temp.txt");

	if (!tablesInfoFile || !tempFile)
	{
		std::cout << "Cannot open file!\n";
	}
	else
	{
		while (std::getline(tablesInfoFile, lineFromFile))
		{
			if (lineFromFile != tableNameToRemove)
			{
				tempFile << lineFromFile << "\n";
			}
		}

		tempFile.close();
		tablesInfoFile.close();
	}

	const char* removedName = FILE_NAME.c_str();
	std::remove(removedName);
	std::rename("temp.txt", removedName);
}

void Database::listTables() 
{
	tablesInfoFile.open(FILE_NAME, std::ios::in);

	if (!tablesInfoFile)
	{
		std::cout << "Cannot open TablesInfo.txt file!\n";
	}
	else
	{
		std::string tableName;
		int count = 0;
		while (std::getline(tablesInfoFile, tableName))
		{
			std::cout << tableName << "\n";
			count++;
		}

		tablesInfoFile.close();

		(count == 1) ?
			std::cout << "Total " << count << " table in database!\n" :
			std::cout << "Total " << count << " tables in database!\n";
	}
}

void Database::tableInfo(const std::string& tableName)
{
	if (this->checkIfTableExists(tableName))
	{
		this->readColumnsInfoStrFromFile(tableName);
		this->sumSizesFromTableInfoFile(tableName);
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}

void Database::readColumnsInfoStrFromFile(const std::string& tableName)
{
	std::string fileName = tableName + ".dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);
	std::string stringFromFile;

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file.\n";
	}
	else
	{
		unsigned strLength;
		file.read((char*)&strLength, sizeof(strLength));

		char* buffer = new char[strLength + 1];
		file.read(buffer, strLength);
		buffer[strLength] = '\0';
		stringFromFile = buffer;
		delete[] buffer;

	}
	file.close();

	std::cout << "Table " << tableName << " : " << stringFromFile << "\n";
}

void Database::sumSizesFromTableInfoFile(const std::string& tableName)
{
	std::string fileName = tableName + "_Info.dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	int countOfEl = this->getNumOfElInTableRowInfoFile(tableName) - 1;
	int sizesSum = 0;
	int countForRowsSize = 0;

	if (!file)
	{
		return;
	}
	else
	{
		int rowSize;
		file.seekg(sizeof(rowSize), std::ios::beg);
		for (int i = 0; i < countOfEl; i++)
		{
			file.read((char*)&rowSize, sizeof(rowSize));

			if (rowSize > 0)
			{
				sizesSum += rowSize;
				countForRowsSize++;
			}
		}
	}
	file.close();

	std::cout << "Total " << countForRowsSize << " rows (" << sizesSum << " KB data) in the table.\n";
}

int Database::getNumOfElInTableRowInfoFile(const std::string& tableName)
{
	std::string fileName = tableName + "_Info.dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::ate);

	if (!file)
	{
		return 0;
	}
	else
	{
		std::streampos fileSize = file.tellg();
		std::streampos numberOfElements = fileSize / sizeof(int);

		return (int)numberOfElements;

		file.close();
	}
}

bool Database::isSign(const std::string& str) const 
{
	return str == "==" || str == "!=" || str == ">=" || str == "<=" || str == ">" || str == "<";
}

bool Database::isDigit(char c) const
{
	return c >= '0' && c <= '9';
}

bool Database::isValidCommand(Table& table, std::string& commandInfo, const std::string& columnNameToSelect)
{
	std::string whereCommand = commandInfo.substr(0, commandInfo.find(' '));
	commandInfo.erase(0, whereCommand.size() + 1);

	if (whereCommand != "WHERE")
	{
		std::cout << "Invalid syntax of command, you must have the word WHERE after name of table!\n";
		return false;
	}

	if (commandInfo == "")
	{
		std::cout << "Invalid syntax of command, elements to compare are missing!\n";
		return false;
	}

	if (!table.hasColumn(columnNameToSelect) && columnNameToSelect != "*" && columnNameToSelect != "Remove")
	{
		std::cout << "Invalid syntax of command, name of column to select isn't correct!\n";
		return false;
	}

	Stack stack;

	int bracketsCount = 0;
	size_t size = commandInfo.size();
	for (size_t i = 0; i < size; i++)
	{
		std::string element = "";

		if (commandInfo[i] == '(')
		{
			bracketsCount++;
			i++;
		}

		while (commandInfo[i] != ' ' && i != size && commandInfo[i] != ')')
		{
			element += commandInfo[i];
			i++;
		}

		if (commandInfo[i] == ')')
		{
			if (bracketsCount == 0)
			{
				std::cout << "Invalid syntax of command, you cannot have ')' bracket before '(' bracket!\n";
				return false;
			}

			bracketsCount--;
			i++;
		}

		if (i == size)
		{
			stack.push(element);
		}

		if (element == "AND" || element == "OR" || i == size)
		{
			if (stack.getNumOfElements() != 3)
			{
				std::cout << "Invalid syntax of command!\n";
				return false;
			}

			std::string elToCompare = stack.getTop();
			stack.pop();

			std::string sign = stack.getTop();
			stack.pop();

			if (!isSign(sign))
			{
				std::cout << "Invalid syntax of command, sign is not correct!\n";
				return false;
			}

			std::string column = stack.getTop();
			stack.pop();

			if (!table.hasColumn(column))
			{
				std::cout << "Invalid syntax of command, column name is not correct!\n";
				return false;
			}

			if (table.getColumnType(column) == ColumnStruct::Type::STRING)
			{
				if (elToCompare[0] != '"')
				{
					std::cout << "Invalid syntax of command, type of element to compare is not correct!\n";
					return false;
				}
			}
			else
			{
				if (!isDigit(elToCompare[0]))
				{
					if (elToCompare[0] != '"')
					{
						std::cout << "Invalid syntax of command, type of element to compare is not correct!\n";
						return false;
					}
				}
			}
		}
		else
		{
			stack.push(element);
		}
	}

	if (bracketsCount != 0)
	{
		std::cout << "Invalid syntax of command, brackets are not balanced.\n";
		return false;
	}

	return true;
}

void Database::select(std::string& commandInfo)
{
	std::string columnNameToSelect = commandInfo.substr(0, commandInfo.find(' '));
	commandInfo.erase(0, columnNameToSelect.size() + 1);

	std::string fromCommand = commandInfo.substr(0, commandInfo.find(' '));
	commandInfo.erase(0, fromCommand.size() + 1);

	if (fromCommand != "FROM")
	{
		std::cout << "Invalid syntax of command, you must have word FROM before name of table!\n";
		return;
	}

	std::string tableName = commandInfo.substr(0, commandInfo.find(' '));
	commandInfo.erase(0, tableName.size() + 1);

	if (this->checkIfTableExists(tableName))
	{
		size_t pos = commandInfo.find("ORDER BY");
		if (pos != std::string::npos)
		{
			this->makeSelectWithOrderBy(commandInfo, columnNameToSelect, tableName, pos);
		}
		else
		{
			this->makeSelectOrRemove(commandInfo, tableName, columnNameToSelect);
		}
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}

void Database::makeSelectWithOrderBy(std::string& commandInfo, const std::string& columnNameToSelect, const std::string& tableName, size_t pos)
{
	std::string columnNameForOrder = commandInfo.substr(pos + 9);
	commandInfo.erase(pos - 1);

	Table table(tableName);

	if (!table.hasColumn(columnNameForOrder))
	{
		std::cout << "Invalid syntax of command, name of column to order is not correct!\n";
		return;
	}

	if (this->isValidCommand(table, commandInfo, columnNameToSelect))
	{
		Queue queue;
		this->splitStringForCommand(commandInfo, queue);

		table.getRowsFromRowsInfoFile();
		table.readFromFileAndSort(columnNameToSelect, queue, columnNameForOrder);
	}
}

void Database::makeSelectOrRemove(std::string& commandInfo, const std::string& tableName, const std::string& columnNameToSelect)
{
	Table table(tableName);

	if (isValidCommand(table, commandInfo, columnNameToSelect))
	{
		Queue queue;
		splitStringForCommand(commandInfo, queue);

		table.getRowsFromRowsInfoFile();
		table.readFromTableFile(columnNameToSelect, queue);
	}
}

void Database::splitStringForCommand(std::string& commandInfo, Queue& queue)
{
	size_t size = commandInfo.size();
	for (size_t i = 0; i < size; i++)
	{
		std::string element = "";

		if (commandInfo[i] == '(')
		{
			queue.enqueue("(");
			i++;
		}

		while (commandInfo[i] != ' ' && i != size && commandInfo[i] != ')')
		{
			element += commandInfo[i];
			i++;
		}

		queue.enqueue(element);

		if (commandInfo[i] == ')')
		{
			queue.enqueue(")");
			i++;
		}
	}
	queue.enqueue("&");
}

void Database::remove(std::string& commandInfo)
{
	std::string fromCommand = commandInfo.substr(0, commandInfo.find(' '));
	if (fromCommand != "FROM")
	{
		std::cout << "Invalid syntax of command, you must have word FROM before name of table!\n";
		return;
	}
	commandInfo.erase(0, fromCommand.size() + 1);

	std::string tableName = commandInfo.substr(0, commandInfo.find(' '));
	commandInfo.erase(0, tableName.size() + 1);

	if (this->checkIfTableExists(tableName))
	{
		this->makeSelectOrRemove(commandInfo, tableName, "Remove");
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}

bool Database::isValidInsert(Table& table, std::string& insertInfo)
{
	size_t size = insertInfo.size();
	if (insertInfo[0] != '{' || insertInfo[size - 1] != '}' || insertInfo[size - 2] != ')')
	{
		std::cout << "Invalid sxyntax of Insert command, you must have brackets!\n";
		return false;
	}

	size_t colsCount = table.getNumberOfColumns();
	for (size_t i = 1; i < size - 2; i++)
	{
		int quotesCount = 0;
		int commaCount = 0;
		std::string currentElement = "";

		if (insertInfo[i] != '(')
		{
			std::cout << "Invalid syntax of Insert command!\n";
			return false;
		}
		i++;

		while (insertInfo[i] != ')')
		{
			if (insertInfo[i] == '"')
			{
				quotesCount++;
			}

			if (insertInfo[i] == ',' && insertInfo[i + 1] == ' ' && quotesCount % 2 != 0)
			{
				std::cout << "Invalid syntax of Insert command, you cannot have \", \" in string data!\n";
				return false;
			}

			if (insertInfo[i] == ',' && insertInfo[i + 1] == ' ')
			{
				commaCount++;
				if (currentElement[0] == ' ')
				{
					currentElement.erase(0, 1);
				}

				if (!table.checkElement(currentElement, commaCount))
				{
					std::cout << "Invalid syntax of Insert command, data type is not correct!\n";
					return false;
				}

				i++;
				currentElement = "";
			}

			currentElement += insertInfo[i];
			i++;
		}


		currentElement.erase(0, 1);
		if (!table.checkElement(currentElement, commaCount + 1))
		{
			std::cout << "Invalid syntax of Insert command, you must have symbols ',' and ' ' between data that you want to insert!\n";
			return false;
		}

		if ((commaCount == colsCount - 1 && insertInfo[i] != ')') || commaCount != colsCount - 1)
		{
			std::cout << "Invalid syntax of Insert command!\n";
			return false;
		}

		if (i != size - 1)
		{
			i += 2;
		}
	}
	return true;
}

void Database::insert(std::string& insertInfo)
{
	if (insertInfo.substr(0, insertInfo.find(' ')) != "INTO")
	{
		std::cout << "Invalid syntax of Insert command, you must have the word INTO before name of table!\n";
		return;
	}
	insertInfo.erase(0, insertInfo.find(' ') + 1);

	std::string tableName = insertInfo.substr(0, insertInfo.find(' '));
	insertInfo.erase(0, tableName.size() + 1);

	if (this->checkIfTableExists(tableName))
	{
		int countInsertedRows = 0;

		Table table(tableName);

		if (this->isValidInsert(table, insertInfo))
		{
			table.getRowsFromRowsInfoFile();

			size_t size = insertInfo.size();
			std::string rowToInsert;
			for (size_t i = 2; i < size - 1; i++)
			{
				while (insertInfo[i] != ')')
				{
					rowToInsert += insertInfo[i];
					i++;
				}

				table.addInsertDataToTableFile(rowToInsert, tableName);

				countInsertedRows++;

				if (i != size - 1)
				{
					i += 3;
					rowToInsert = "";
				}
			}

			table.saveRowsSizesToInfoFile();

			(countInsertedRows == 1) ?
				std::cout << countInsertedRows << " row inserted.\n" :
				std::cout << countInsertedRows << " rows inserted.\n";
		}
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}

bool Database::checkIfTableExists(const std::string& tableName)
{
	std::string tableNameToCheck = tableName + ".dat";

	tablesInfoFile.open(FILE_NAME, std::ios::in);

	bool found = false;
	if (!tablesInfoFile)
	{
		std::cout << "Cannot open " << FILE_NAME << " file!\n";
	}
	else
	{
		std::string fileLine;
		while (std::getline(tablesInfoFile, fileLine))
		{
			if (fileLine == tableNameToCheck)
			{
				found = true;
				break;
			}
		}

		tablesInfoFile.close();
	}

	return found;
}

void Database::removeData(const std::string& tableName)
{
	if (this->checkIfTableExists(tableName))
	{
		Table table(tableName);
		table.removeMarkedDataFromFile();

		std::cout << "All marked data was removed from table!\n";
	}
	else
	{
		std::cout << "There is no such a table in database!\n";
	}
}
