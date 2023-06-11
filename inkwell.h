#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory>

/*
IMPORTANT: Watch out for memory leaks! The deserializer allocates memory for the project, tables, events, facts, rules, criteria, and modifications.
*/

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
		std::shared_ptr<Entry> comparedEntry = 0;
		int entryID = 0;
		int compareValue = 0;
		Keys comparisonOperator = Keys::NULL_KEY;
	};

	class Modification
	{
	public:
		std::shared_ptr <Entry> modifiedEntry = 0;
		int entryID = 0;
		Keys modificationOperator = Keys::NULL_KEY;
		int modifyWithValue = 0;
	};

	class Rule : public Entry
	{
	public:
		std::vector<int> triggeredBy;
		std::vector<int> triggers;
		std::vector<std::shared_ptr<Criterion>> criteria;
		std::vector<std::shared_ptr<Modification>> modifications;
		std::vector<void(*)()> callbacks;
		void setTriggeredBy(std::vector<int> triggeredBy, ArrayOperator operation);
		void setTriggers(std::vector<int> triggers, ArrayOperator operation);
		void setCriteria(std::vector<std::shared_ptr<Criterion>> criteria, ArrayOperator operation);
		void setModifications(std::vector<std::shared_ptr<Modification>> modifications, ArrayOperator operation);
		void dispatchCallbacks();
	};

	class Table
	{
	public:
		int id = 0;
		std::string key = "";
		std::unordered_map<int, std::shared_ptr<Event>> events;
		std::unordered_map<int, std::shared_ptr<Fact>> facts;
		std::unordered_map<int, std::shared_ptr<Rule>> rules;
		void setEvents(std::vector<std::shared_ptr<Event>> events, ArrayOperator operation);
		void setFacts(std::vector<std::shared_ptr<Fact>> facts, ArrayOperator operation);
		void setRules(std::vector<std::shared_ptr<Rule>> rules, ArrayOperator operation);
	};

	class Project
	{
	public:
		std::string name = "";
		std::string description = "";
		int createdAtNano = 0;
		std::unordered_map<int, std::shared_ptr<Table>> tables;
	};

	class Deserializer
	{
	private:
		std::ifstream fileInput;
		char read = 0;
		std::unordered_map<int, std::shared_ptr<Table>> parseTables();
		std::unordered_map<int, std::shared_ptr<Event>> parseEvents();
		std::unordered_map<int, std::shared_ptr<Fact>> parseFacts();
		std::unordered_map<int, std::shared_ptr<Rule>> parseRules();
		std::vector<std::shared_ptr<Criterion>> parseCriteria();
		std::vector<std::shared_ptr<Modification>> parseModifications();
		std::vector<int> parseIntArray();
		Keys getNextKey();
		std::string getNextString();
		int getNextInteger();

	public:
		Deserializer(const std::string filePath);
		~Deserializer();
		std::shared_ptr<Project> parseProject();
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
		void writeTables(std::unordered_map<int, std::shared_ptr<Table>> tables);
		void writeEvents(std::unordered_map<int, std::shared_ptr<Event>> events);
		void writeFacts(std::unordered_map<int, std::shared_ptr<Fact>> facts);
		void writeRules(std::unordered_map<int, std::shared_ptr<Rule>> rules);
		void writeCriteria(std::vector<std::shared_ptr<Criterion>> criteria);
		void writeModifications(std::vector<std::shared_ptr<Modification>> modifications);
		void writeIntArray(std::vector<int> arr);
	public:
		Serializer(const std::string filePath);
		~Serializer();
		void writeProject(std::shared_ptr<Project> project);
	};
}
