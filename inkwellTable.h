#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "inkwellEntry.h"

namespace inkwell
{
	class Table
	{
	public:
		int id = 0;
		std::string key = "";
		std::unordered_map<int, std::string> idToKey;
		std::unordered_map<std::string, EventRef> events;
		std::unordered_map<std::string, FactRef> facts;
		std::unordered_map<std::string, RuleRef> rules;
		void setEvents(std::vector<EventRef> events, ArrayOperator operation);
		void setFacts(std::vector<FactRef> facts, ArrayOperator operation);
		void setRules(std::vector<RuleRef> rules, ArrayOperator operation);
		EntryRef getEntry(std::string name);
		EntryRef getEntry(int id);
	};
}
