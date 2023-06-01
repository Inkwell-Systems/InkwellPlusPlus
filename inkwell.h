#pragma once

// add ifndef lol
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace inkwell
{
	enum critOp
	{
		EQUALS,
		NOT_EQUALS,
		GREATER_THAN,
		LESS_THAN,
		GREATER_THAN_OR_EQUAL_TO,
		LESS_THAN_OR_EQUAL_TO,
		NULL_CRITOP
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
		TABLES
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
		int value = 0;
	};

	class Criterion
	{
	public:
		int comparedEntry = 0;
		int compareValue = 0;
		critOp critOperator = NULL_CRITOP;
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
		std::vector<Table> tables;
	};

	class JSONParser
	{
	private:

		std::ifstream fileInput;

	public:
		JSONParser(const std::string filename);
		Project parseProject();
		std::vector<Table> parseTables();
		std::vector<Event> parseEvents();
		std::vector<Fact> parseFacts();
		std::vector<Rule> parseRules();
		std::vector<Criterion> parseCriteria();
		std::vector<Modification> parseModifications();

		Keys getNextKey(); // not all keys are introduced yet
		std::string getNextString(); // doesnt support escape sequences
		int getNextInteger(); // do not forget values can be negative aswell
		//float getNextFloat();
	};
}
