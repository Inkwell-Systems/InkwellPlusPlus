#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace inkwell
{
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
		VALUE,
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
		COMPARISON_OPERATOR_EQUAL,
		COMPARISON_OPERATOR_NOT_EQUAL,
		COMPARISON_OPERATOR_GREATER_THAN,
		COMPARISON_OPERATOR_LESS_THAN,
		COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL,
		COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL,
		MODIFIED_ENTRY,
		MODIFICATION_OPERATOR,
		MODIFICATION_OPERATOR_SET,
		MODIFICATION_OPERATOR_INCREMENT,
		MODIFY_WITH_VALUE
	};

	class EnumConverter
	{
	public:
		static Keys toKey(std::string key);
		static ArrayOperator toArrayOperator(std::string key);
		static std::string toString(Keys k);
		static std::string toString(ArrayOperator a);
	};

	class Entry
	{
	public:
		int id = 0;
		std::string key = "";
		int value = 0;
	};

	class Event : public Entry
	{

	};

	class Fact : public Entry
	{
		
	};

	class Criterion
	{
	public:
		int comparedEntry = 0;
		int compareValue = 0;
		Keys comparisonOperator = Keys::NULL_KEY;
	};

	class Modification
	{
	public:
		int modifiedEntry = 0;
		Keys modificationOperator = Keys::NULL_KEY;
		int modifyWithValue = 0;
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
