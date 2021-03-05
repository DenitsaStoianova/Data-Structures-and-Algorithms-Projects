#include "pch.h"
#include <iostream>
#include <string>
#include "Table.h"

Table::Table(const std::string& tableName)
{
	this->tableName = tableName;
	this->readStringFromFile(tableName);
}

void Table::readStringFromFile(const std::string& tableName)
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

	size_t columnsCount = this->getNumberOfColumns(stringFromFile) + 1;
	this->columnsInfo.setCapacity(columnsCount);

	this->columnsInfo.createColumnsFromString(stringFromFile);
}

size_t Table::getNumberOfColumns(const std::string& columnsStr) const
{
	size_t count = 1;
	size_t size = columnsStr.size();
	for (size_t i = 0; i < size; i++)
	{
		if (columnsStr[i] == ',')
		{
			count++;
		}
	}
	return count;
}

void Table::addInsertDataToTableFile(std::string& rowToInsert, const std::string& tableName)
{
	int index = this->rowsSizeArr.getSize();

	int dataSizeKB = 0;

	std::string fileName = tableName + ".dat";
	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::out | std::ios::app);

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file!\n";
	}
	else
	{
		file.write((const char*)&index, sizeof(index));
		dataSizeKB += sizeof(index);

		size_t rowSize = rowToInsert.size();
		for (size_t i = 0; i < rowSize; i++)
		{
			std::string dataToInsert = "";

			if (rowToInsert[i] == ' ')
			{
				i++;
			}

			while (rowToInsert[i] != ',' && i != rowSize)
			{
				dataToInsert += rowToInsert[i];
				i++;
			}

			if (dataToInsert[0] != '"')
			{
				int digit = std::stoi(dataToInsert);
				file.write((const char*)&digit, sizeof(digit));

				dataSizeKB += sizeof(digit);
			}
			else
			{
				unsigned strLength = dataToInsert.size();
				file.write((const char*)&strLength, sizeof(strLength));
				file.write(dataToInsert.c_str(), strLength);

				dataSizeKB += (dataToInsert.size() + sizeof(strLength));
			}
		}
	}

	file.close();

	this->rowsSizeArr.add(dataSizeKB);
}

bool Table::checkElement(const std::string& element, size_t colIndex) const
{
	if (element[0] == '"' && this->columnsInfo[colIndex].type == ColumnStruct::Type::INT)
	{
		return false;
	}
	if (element[0] != '"' && this->columnsInfo[colIndex].type == ColumnStruct::Type::STRING)
	{
		return false;
	}
	return true;
}

void Table::getRowsFromRowsInfoFile()
{
	std::string fileName = this->tableName + "_Info.dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	if (!file)
	{
		return;
	}
	else
	{
		int countOfEl = this->getNumOfElInTableRowInfoFile();

		int rowSize;
		for (int i = 0; i < countOfEl; i++)
		{
			file.read((char*)&rowSize, sizeof(rowSize));
			this->rowsSizeArr.add(rowSize);
		}

		file.close();
	}
}

size_t Table::getNumberOfColumns() const
{
	return this->columnsInfo.getSize() - 1;
}

ColumnStruct::Type Table::getColumnType(const std::string& columnName) const
{
	size_t index = this->columnsInfo.getColumnIndex(columnName);
	return this->columnsInfo.getColumnType(index);
}

int Table::getNumOfElInTableRowInfoFile()
{
	std::string fileName = this->tableName + "_Info.dat";
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

void Table::readFromTableFile(const std::string& columnNameToSelect, Queue& commandsQueue)
{
	size_t sizeOfColumnsInfo = this->columnsInfo.getSize();
	int countRows = 0; // count if have to print columns info or count removed rows

	std::string fileName = this->tableName + ".dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file!\n";
	}
	else
	{
		file.seekg(this->rowsSizeArr[0], std::ios::beg);

		size_t rowsArrSize = this->rowsSizeArr.getSize();
		for (size_t j = 1; j < rowsArrSize; j++)
		{
			RowData row(sizeOfColumnsInfo);

			for (size_t i = 0; i < sizeOfColumnsInfo; i++)
			{
				if (this->columnsInfo[i].type == ColumnStruct::Type::INT)
				{
					int digit;
					file.read((char*)&digit, sizeof(digit));

					IntColumn intCol(digit);
					row.addColumnToRow(intCol);
				}
				else
				{
					unsigned strLength;
					file.read((char*)&strLength, sizeof(strLength));

					std::string str;
					char* buffer = new char[strLength + 1];
					file.read(buffer, strLength);
					buffer[strLength] = '\0';
					str = buffer;
					delete[] buffer;

					StringColumn stringCol(str);
					row.addColumnToRow(stringCol);
				}
			}

			if (this->rowsSizeArr[j] > 0)
			{
				if (columnNameToSelect == "Remove")
				{
					this->makeRemoveCommand(row, commandsQueue, j, countRows);
				}
				else
				{
					this->makeSelectCommand(row, commandsQueue, columnNameToSelect, countRows);
				}
			}
			else
			{
				std::cout << "The row with index " << j << " is already removed from table!\n";
			}
		}
	}
	file.close();

	if (columnNameToSelect == "Remove")
	{
		std::cout << "Total " << countRows << " rows removed.\n";
		this->saveRowsSizesToInfoFile();
	}
	else
	{
		std::cout << "Total " << countRows << " rows selected.\n";
	}
}

void Table::saveRowsSizesToInfoFile()
{
	std::string fileName = this->tableName + "_Info.dat";
	std::ofstream file;
	file.open(fileName, std::ios::binary | std::ios::out, std::ios::trunc);

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file!\n";
	}
	else
	{
		size_t size = this->rowsSizeArr.getSize();
		for (size_t i = 0; i < size; i++)
		{
			int sizeToFile = this->rowsSizeArr[i];
			file.write((const char*)&sizeToFile, sizeof(sizeToFile));
		}

		file.close();
	}
}

void Table::makeRemoveCommand(const RowData& row, Queue& commandsQueue, size_t& countForIndex, int& countRemovedRows)
{
	if (this->checkExpression(row, commandsQueue))
	{
		countRemovedRows++;

		this->rowsSizeArr[countForIndex] = -this->rowsSizeArr[countForIndex];
	}
}

void Table::makeSelectCommand(const RowData& row, Queue& commandsQueue, const std::string& columnNameToSelect, int& countForPrint)
{
	if (this->checkExpression(row, commandsQueue))
	{
		countForPrint++;

		size_t columnIndex = columnsInfo.getColumnIndex(columnNameToSelect);

		printForSelect(row, columnNameToSelect, 1, columnIndex, countForPrint);
	}
}

bool Table::checkExpression(const RowData& row, Queue& commandsQueue)
{
	Stack stack;

	if (commandsQueue.getFront() == "&")
	{
		commandsQueue.dequeue();
	}

	while (commandsQueue.getFront() != "&")
	{
		std::string command = commandsQueue.getFront();
		commandsQueue.dequeue();
		commandsQueue.enqueue(command);

		if (command == "(")
		{
			stack.push(command);
		}
		else if (command == ")")
		{
			(this->checkCommandsFromStack(stack)) ? stack.push("true") : stack.push("false");
		}
		else if (command == "AND" || command == "OR")
		{
			stack.push(command);
		}
		else
		{
			size_t columnIndex = this->columnsInfo.getColumnIndex(command);

			std::string sign = commandsQueue.getFront();
			commandsQueue.dequeue();
			commandsQueue.enqueue(sign);

			std::string elementToCompare = commandsQueue.getFront();
			commandsQueue.dequeue();
			commandsQueue.enqueue(elementToCompare);

			if (row[columnIndex].getColumnType() == ColumnType::INT)
			{
				int digit = std::stoi(elementToCompare);

				(row[columnIndex].makeComparison(sign, &digit)) ? stack.push("true") : stack.push("false");
			}
			else
			{
				const char* elemToFunc = elementToCompare.c_str();
				(row[columnIndex].makeComparison(sign, elemToFunc)) ? stack.push("true") : stack.push("false");
			}
		}
	}

	commandsQueue.enqueue("&");

	return this->checkCommandsFromStack(stack);
}

bool Table::checkCommandsFromStack(Stack& stack) const
{
	if (stack.getNumOfElements() == 1)
	{
		return (stack.getTop() == "true") ? true : false;
	}

	std::string rightResult = stack.getTop();
	stack.pop();
	std::string stackCommand = stack.getTop();
	stack.pop();
	std::string leftResult = stack.getTop();
	stack.pop();

	if (!stack.isEmpty() && stack.getTop() == "(")
	{
		stack.pop();
	}

	if (stackCommand == "AND")
	{
		return (leftResult == "true" && rightResult == "true") ? true : false;
	}
	else if (stackCommand == "OR")
	{
		return (leftResult == "false" || rightResult == "false") ? false : true;
	}

	return (stack.getTop() == "true") ? true : false;
}

void Table::readFromFileAndSort(const std::string& columnNameToSelect, Queue& commandsQueue, const std::string& columnForOrder)
{
	DynamicArray<int> arrForIndexes;
	DynamicArray<std::string> arrForColumns;

	size_t sizeOfColumnsInfo = this->columnsInfo.getSize();

	size_t columnIndex = columnsInfo.getColumnIndex(columnForOrder);
	int countForPrint = 0; // count if have to print columns info or count removed rows
	bool isDigit = true;

	std::string fileName = this->tableName + ".dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file!\n";
	}
	else
	{
		file.seekg(this->rowsSizeArr[0], std::ios::beg);

		size_t rowsSize = this->rowsSizeArr.getSize();
		for (size_t j = 1; j < rowsSize; j++)
		{
			RowData row(sizeOfColumnsInfo);

			for (size_t i = 0; i < sizeOfColumnsInfo; i++)
			{
				if (this->columnsInfo[i].type == ColumnStruct::Type::INT)
				{
					int digit;
					file.read((char*)&digit, sizeof(digit));

					IntColumn intCol(digit);
					row.addColumnToRow(intCol);
				}
				else
				{
					unsigned strLength;
					file.read((char*)&strLength, sizeof(strLength));

					std::string str;
					char* buffer = new char[strLength + 1];
					file.read(buffer, strLength);
					buffer[strLength] = '\0';
					str = buffer;
					delete[] buffer;

					StringColumn stringCol(str);
					row.addColumnToRow(stringCol);
				}
			}

			isDigit = true;

			if (this->rowsSizeArr[j] > 0)
			{
				if (this->checkExpression(row, commandsQueue))
				{
					countForPrint++;

					arrForIndexes.add(j);

					if (this->columnsInfo.getColumnType(columnIndex) == ColumnStruct::Type::INT)
					{
						IntColumn* intCol = dynamic_cast<IntColumn*>(&(row[columnIndex]));
						int intDataToOrder = intCol->getData();

						std::string intDataToArr = std::to_string(intDataToOrder);
						arrForColumns.add(intDataToArr);
					}
					else
					{
						isDigit = false;

						StringColumn* strCol = dynamic_cast<StringColumn*>(&(row[columnIndex]));
						std::string strDataToOrder = strCol->getData();

						arrForColumns.add(strDataToOrder);
					}
				}
			}
			else
			{
				std::cout << "The row with index " << j << " is already removed!\n";
			}
		}
	}
	file.close();

	if (arrForIndexes.getSize() != 0)
	{
		this->makeSort(arrForIndexes, arrForColumns, isDigit);
		this->readRowsFromFile(arrForIndexes, columnNameToSelect, columnIndex);
	}

	std::cout << "Total " << countForPrint << " rows selected.\n";
}

void Table::makeSort(DynamicArray<int>& arrForIndexes, DynamicArray<std::string>& arrForColumns, bool isDigit) const
{
	size_t size = arrForColumns.getSize();
	bool areSwapped;
	for (size_t i = 0; i < size - 1; i++)
	{
		areSwapped = false;
		for (size_t j = 0; j < size - i - 1; j++)
		{
			if (isDigit)
			{
				int digit = std::stoi(arrForColumns[j]);
				int digit1 = std::stoi(arrForColumns[j + 1]);

				if (digit > digit1)
				{
					std::swap(arrForColumns[j], arrForColumns[j + 1]);
					std::swap(arrForIndexes[j], arrForIndexes[j + 1]);
					areSwapped = true;
				}
			}
			else
			{
				if (arrForColumns[j] > arrForColumns[j + 1])
				{
					std::swap(arrForColumns[j], arrForColumns[j + 1]);
					std::swap(arrForIndexes[j], arrForIndexes[j + 1]);
					areSwapped = true;
				}
			}
		}

		if (areSwapped == false)
		{
			break;
		}
	}
}


void Table::printForSelect(const RowData& row, const std::string& columnNameToSelect, size_t check, size_t columnIndex, size_t indexToCheck) const
{
	if (columnNameToSelect != "*")
	{
		if (indexToCheck == check)
		{
			columnsInfo.printColumnName(columnIndex);
		}

		row[columnIndex].printElement();
	}
	else
	{
		if (indexToCheck == check)
		{
			columnsInfo.print();
		}
		row.print();
	}
}

bool Table::hasColumn(const std::string& columnName) const
{
	return this->columnsInfo.hasColumn(columnName);
}

int Table::readRowsFromFileSum(int number) const
{
	int pos = 0;
	for (int i = 0; i < number; i++)
	{
		pos += abs(rowsSizeArr[i]);
	}

	return pos;
}

void Table::readRowsFromFile(DynamicArray<int>& indexesArr, const std::string& columnNameToSelect, size_t columnIndex)
{
	size_t sizeOfColumnsInfo = this->columnsInfo.getSize();

	std::string fileName = this->tableName + ".dat";
	std::ifstream file;
	file.open(fileName, std::ios::binary);

	if (!file)
	{
		std::cout << "Cannot open " << fileName << " file!\n";
	}
	else
	{
		size_t sizeArr = indexesArr.getSize();
		for (size_t i = 0; i < sizeArr; i++)
		{
			file.seekg(this->readRowsFromFileSum(indexesArr[i]), std::ios::beg);

			RowData row(sizeOfColumnsInfo);
			for (size_t j = 0; j < sizeOfColumnsInfo; j++)
			{
				if (this->columnsInfo[j].type == ColumnStruct::Type::INT)
				{
					int digit;
					file.read((char*)&digit, sizeof(digit));

					IntColumn intData(digit);
					row.addColumnToRow(intData);
				}
				else
				{
					unsigned strFromFileLength;
					file.read((char*)&strFromFileLength, sizeof(strFromFileLength));

					std::string str;
					char* buffer = new char[strFromFileLength + 1];
					file.read(buffer, strFromFileLength);
					buffer[strFromFileLength] = '\0';
					str = buffer;
					delete[] buffer;

					StringColumn strData(str);
					row.addColumnToRow(strData);
				}
			}

			printForSelect(row, columnNameToSelect, 0, columnIndex, i);
		}
	}

	file.close();
}

void Table::removeNegativeSizes(DynamicArray<int>& arr)
{
	if (!arr.isEmpty())
	{
		int last = arr.getLast();
		arr.remove(arr.getSize() - 1);

		removeNegativeSizes(arr);

		if (last >= 0)
		{
			arr.add(last);
		}
	}
}

int Table::removeMarkedDataSum(int index) const
{
	int pos = 0;
	for (size_t i = 0; i <= index; i++)
	{
		pos += abs(this->rowsSizeArr[i]);
	}
	return pos;
}

void Table::removeMarkedDataFromFile()
{
	this->getRowsFromRowsInfoFile();

	size_t sizeOfColumnsInfo = this->columnsInfo.getSize();

	std::string fileName = this->tableName + ".dat";
	std::ifstream fileToRead;
	fileToRead.open(fileName, std::ios::binary | std::ios::in);

	std::ofstream fileToWrite;
	fileToWrite.open("temp.dat", std::ios::binary | std::ios::app | std::ios::out);

	if (!fileToRead || !fileToWrite)
	{
		std::cout << "Cannot open file!\n";
	}
	else
	{
		// read string with columns info from file
		std::string  stringFromFile;
		unsigned strLength;
		fileToRead.read((char*)&strLength, sizeof(strLength));

		char* buffer = new char[strLength + 1];
		fileToRead.read(buffer, strLength);
		buffer[strLength] = '\0';
		stringFromFile = buffer;
		delete[] buffer;

		// write string with columns to other file
		fileToWrite.write((const char*)&strLength, sizeof(strLength));
		fileToWrite.write(stringFromFile.c_str(), strLength);

		size_t rowsArrSize = this->rowsSizeArr.getSize();

		int newIndexForColumn = 0;

		for (size_t i = 1; i < rowsArrSize; i++)
		{
			if (this->rowsSizeArr[i] < 0)
			{
				fileToRead.seekg(removeMarkedDataSum(i), std::ios::beg);
			}
			else
			{
				newIndexForColumn++;

				for (size_t j = 0; j < sizeOfColumnsInfo; j++)
				{
					if (this->columnsInfo[j].type == ColumnStruct::Type::INT)
					{
						int digit;
						fileToRead.read((char*)&digit, sizeof(digit));

						if (j == 0)
						{
							fileToWrite.write((const char*)&newIndexForColumn, sizeof(newIndexForColumn));
						}
						else
						{
							fileToWrite.write((const char*)&digit, sizeof(digit));
						}
					}
					else
					{
						unsigned strFromFileLength;
						fileToRead.read((char*)&strFromFileLength, sizeof(strFromFileLength));

						std::string str;
						char* buffer = new char[strFromFileLength + 1];
						fileToRead.read(buffer, strFromFileLength);
						buffer[strFromFileLength] = '\0';
						str = buffer;
						delete[] buffer;

						fileToWrite.write((const char*)&strFromFileLength, sizeof(strFromFileLength));
						fileToWrite.write(str.c_str(), strFromFileLength);

					}
				}
			}
		}

		fileToRead.close();
		fileToWrite.close();
	}

	const char* removedName = fileName.c_str();
	std::remove(removedName);
	std::rename("temp.dat", removedName);

	// change array with row sizes
	this->removeNegativeSizes(this->rowsSizeArr);

	this->saveRowsSizesToInfoFile();
}

