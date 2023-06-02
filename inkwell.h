#pragma once

// add ifndef lol
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

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

	class JSONParser
	{
	private:
		std::ifstream fileInput;
		char read = 0;

	public:
		JSONParser(const std::string filename);
		Project parseProject();
		std::unordered_map<int, Table> parseTables();
		std::unordered_map<int, Event> parseEvents();
		std::unordered_map<int, Fact> parseFacts();
		std::unordered_map<int, Rule> parseRules();
		std::vector<Criterion> parseCriteria();
		std::vector<Modification> parseModifications();
		std::vector<int> parseIntArray();

		Keys getNextKey();
		std::string getNextString(); // doesnt support escape sequences
		int getNextInteger();
		comparisonOp getNextComparisonOp();
		modOp getNextModOp();
	};
}
