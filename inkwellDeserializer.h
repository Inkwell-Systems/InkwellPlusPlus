#pragma once

#include "inkwellProject.h"
#include <fstream>
#include <iostream>

namespace inkwell
{
	class Deserializer
	{
	private:
		std::ifstream fileInput;
		char read = 0;
		std::unordered_map<std::string, TableRef> parseTables();
		std::unordered_map<std::string, EventRef> parseEvents(TableRef currentTable);
		std::unordered_map<std::string, FactRef> parseFacts(TableRef currentTable);
		std::unordered_map<std::string, RuleRef> parseRules(TableRef currentTable);
		std::vector<CriterionRef> parseCriteria(TableRef currentTable);
		std::vector<ModificationRef> parseModifications(TableRef currentTable);
		std::vector<int> parseNumberArray();
		Keys getKey();
		std::string getBuffer();
		int getInteger();
		double getDouble();
		bool readInput();

	public:
		Deserializer(const std::string filePath);
		~Deserializer();
		ProjectRef parseProject();
	};
}
