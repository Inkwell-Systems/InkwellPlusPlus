#pragma once

// TODO fix inconsistencies in naming

// add ifndef lol
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <typeinfo>

namespace inkwell
{
	enum comparisonOp
	{
		EQUAL,
		NOT_EQUAL,
		GREATER_THAN,
		LESS_THAN,
		GREATER_THAN_OR_EQUAL,
		LESS_THAN_OR_EQUAL,
		NULL_COMPARISONOP
	};

	enum modOp
	{
		SET,
		ADD,
		INCREMENT,
		NULL_MODOP
	};

	enum Keys
	{
		NULL_KEYS,
		NAME,
		DESCRIPTION,
		CREATEDAT,
		TABLES,
		ID,
		KEY,
		TIMES_TRIGGERED,
		EVENTS,
		FACTS,
		RULES,
		DATA,
		TRIGGERED_BY,
		TRIGGERS,
		CRITERIA,
		MODIFICATIONS,
		COMPARED_ENTRY,
		COMPARE_VALUE,
		COMPARISON_OPERATOR,
		MODIFIED_ENTRY,
		MOD_OPERATOR,
		VALUE
	};

	class Entry
	{
	public:
		int id = 0;
		std::string key = "";
		int timesTriggered = 0;
		void displayOnConsole();
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
		comparisonOp comparisonOperator = NULL_COMPARISONOP;
	};

	class Modification
	{
	public:
		int modifiedEntry = 0;
		modOp modOperator = NULL_MODOP;
		int value = 0;
	};

	class Rule : public Entry
	{
	public:
		std::vector<int> triggeredBy;
		std::vector<int> triggers;
		std::vector<Criterion> criteria;
		std::vector<Modification> modifications;
		void setTriggeredBy(std::vector<int> triggeredBy, modOp operation);
		void setTriggers(std::vector<int> triggers, modOp operation);
		void setCriteria(std::vector<Criterion> criteria, modOp operation);
		void setModifications(std::vector<Modification> modifications, modOp operation);
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
		void setEvents(std::vector<Event> events, modOp operation);
		void setFacts(std::vector<Fact> facts, modOp operation);
		void setRules(std::vector<Rule> rules, modOp operation);
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
		Keys stringToKey(std::string s);
		std::string getNextString(); // doesnt support escape sequences
		int getNextInteger();
		comparisonOp getNextComparisonOp();
		modOp getNextModOp();

	public:
		Deserializer(const std::string filePath);
		~Deserializer();
		Project parseProject();
	};
	//doesnt support multiple projects in one file yet
	//add backtracking through file to avoid some issues with formatting

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
	// also output times triggered
}
