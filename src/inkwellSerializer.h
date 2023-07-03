#pragma once

#include "inkwellProject.h"
#include <fstream>
#include <iostream>

namespace inkwell
{
	class Serializer
	{
	private:
		std::ofstream fileOutput;
		int globalNestingLevel = 0;
		void format();
		void startObject();
		void endObject(bool isLast);
		void writeTables(std::unordered_map<std::string, TableRef> tables);
		void writeEvents(std::unordered_map<std::string, EventRef> events);
		void writeFacts(std::unordered_map<std::string, FactRef> facts);
		void writeRules(std::unordered_map<std::string, RuleRef> rules, TableRef currentTable);
		void writeObjArrayID(std::vector<EntryRef> triggers);
		void writeCriteria(std::vector<CriterionRef> criteria);
		void writeModifications(std::vector<ModificationRef> modifications);
		void writeIntArray(std::vector<int> arr);
	public:
		Serializer(const std::string filePath);
		~Serializer();
		void writeProject(ProjectRef project);
	};
}
