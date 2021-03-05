#include "pch.h"
#include <iostream>
#include "Database.h"

void selectFunction(Database& database)
{
	std::string command;

	do
	{
		std::cout << "FMISql> ";
		std::cin >> command;

		if (command == "Quit")
		{
			std::cout << "Goodbye!\n";
			break;
		}
		else if (command == "CreateTable")
		{
			std::string createInfo;
			std::cin.ignore();
			std::getline(std::cin, createInfo);

			database.createTable(createInfo);
		}
		else if (command == "DropTable")
		{
			std::string tableName;
			std::cin >> tableName;

			database.dropTable(tableName);
		}
		else if (command == "ListTables")
		{
			database.listTables();
		}
		else if (command == "TableInfo")
		{
			std::string tableName;
			std::cin >> tableName;

			database.tableInfo(tableName);
		}
		else if (command == "Select")
		{
			std::string selectInfo;
			std::cin.ignore();
			std::getline(std::cin, selectInfo);

			database.select(selectInfo);
		}
		else if (command == "Remove")
		{
			std::string removeInfo;
			std::cin.ignore();
			std::getline(std::cin, removeInfo);

			database.remove(removeInfo);
		}
		else if (command == "Insert")
		{
			std::string insertInfo;
			std::cin.ignore();
			std::getline(std::cin, insertInfo);

			database.insert(insertInfo);
		}
		else if (command == "RemoveData")
		{
			std::string tableName;
			std::cin >> tableName;

			database.removeData(tableName);
		}
		else
		{
			std::cout << "Invalid command!\n";
		}

	} while (true);
}