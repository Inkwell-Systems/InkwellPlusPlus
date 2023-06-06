#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace inkwell
{
	enum class ComparisonOperator
	{
		NULL_OPERATOR,
		EQUAL,
		NOT_EQUAL,
		GREATER_THAN,
		LESS_THAN,
		GREATER_THAN_OR_EQUAL,
		LESS_THAN_OR_EQUAL
	};

	enum class ModificationOperator
	{
		NULL_OPERATOR,
		SET,
		INCREMENT
	};
	// Fix this

	enum class ArrayOperator
	{
		NULL_OPERATOR,
		SET,
		ADD
	};

	enum class Keys
	{
		NULL_KEY,
		PROJECT_NAME,
		PROJECT_DESCRIPTION,
		PROJECT_CREATEDAT,
		TABLES,
		ID,
		KEY,
		TIMES_TRIGGERED,
		EVENTS,
		FACTS,
		RULES,
		FACT_DATA,
		RULE_TRIGGERED_BY,
		RULE_TRIGGERS,
		RULE_CRITERIA,
		RULE_MODIFICATIONS,
		COMPARED_ENTRY,
		COMPARE_VALUE,
		COMPARISON_OPERATOR,
		MODIFIED_ENTRY,
		MODIFICATION_OPERATOR,
		MODIFY_WITH_VALUE
	};

	class enumConverter
	{
	public:
		static Keys toKey(std::string key);
		static ComparisonOperator toComparisonOperator(std::string key);
		static ModificationOperator toModificationOperator(std::string key);
		static ArrayOperator toArrayOperator(std::string key);
		static std::string toString(Keys k);
		static std::string toString(ComparisonOperator c);
		static std::string toString(ModificationOperator m);
		static std::string toString(ArrayOperator a);
	};

	class Entry
	{
	public:
		int id = 0;
		std::string key = "";
		int timesTriggered = 0;
	};

	class Event : public Entry
	{

	};

	class Fact : public Entry
	{
	public:
		int data = 0;
	};

	class Criterion
	{
	public:
		int comparedEntry = 0;
		int compareValue = 0;
		ComparisonOperator comparisonOperator = ComparisonOperator::NULL_OPERATOR;
	};

	class Modification
	{
	public:
		int modifiedEntry = 0;
		ModificationOperator modificationOperator = ModificationOperator::NULL_OPERATOR;
		int value = 0;
	};

	class Rule : public Entry
	{
	public:
		std::vector<int> triggeredBy;
		std::vector<int> triggers;
		std::vector<Criterion> criteria;
		std::vector<Modification> modifications;
		void setTriggeredBy(std::vector<int> triggeredBy, ArrayOperator operation);
		void setTriggers(std::vector<int> triggers, ArrayOperator operation);
		void setCriteria(std::vector<Criterion> criteria, ArrayOperator operation);
		void setModifications(std::vector<Modification> modifications, ArrayOperator operation);
	};

	class Table
	{
	public:
		int id = 0;
		std::string key = "";
		std::unordered_map<int, Event> events;
		std::unordered_map<int, Fact> facts;
		std::unordered_map<int, Rule> rules;
		void displayOnConsole();
		void setEvents(std::vector<Event> events, ArrayOperator operation);
		void setFacts(std::vector<Fact> facts, ArrayOperator operation);
		void setRules(std::vector<Rule> rules, ArrayOperator operation);
		void mergeWithTables(std::vector<Table> tables);
	};

	class Project
	{
	public:
		std::string name = "";
		std::string description = "";
		int createdAtNano = 0;
		std::unordered_map<int, Table> tables;
	};

	class Deserializer
	{
	private:
		std::ifstream fileInput;
		char read = 0;
		std::unordered_map<int, Table> parseTables();
		std::unordered_map<int, Event> parseEvents();
		std::unordered_map<int, Fact> parseFacts();
		std::unordered_map<int, Rule> parseRules();
		std::vector<Criterion> parseCriteria();
		std::vector<Modification> parseModifications();
		std::vector<int> parseIntArray();
		Keys getNextKey();
		std::string getNextString();
		int getNextInteger();
		ComparisonOperator getNextComparisonOperator();
		ModificationOperator getNextModificationOperator();

	public:
		Deserializer(const std::string filePath);
		~Deserializer();
		Project parseProject();
	};
	//doesnt support multiple projects in one file yet

	class Serializer
	{
	private:
		std::ofstream fileOutput;
		int globalNestingLevel = 0;
		void format();
		void startObject();
		void endObject(bool isLast);
		void writeTables(std::unordered_map<int, Table> tables);
		void writeEvents(std::unordered_map<int, Event> events);
		void writeFacts(std::unordered_map<int, Fact> facts);
		void writeRules(std::unordered_map<int, Rule> rules);
		void writeCriteria(std::vector<Criterion> criteria);
		void writeModifications(std::vector<Modification> modifications);
		void writeIntArray(std::vector<int> arr);
	public:
		Serializer(const std::string filePath);
		~Serializer();
		void writeProject(Project project);
	};
}
