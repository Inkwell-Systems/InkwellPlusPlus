#include "inkwellTable.h"

using namespace inkwell;

void Table::setEvents(std::vector<EventRef> events, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->events.clear();

	for (EventRef i : events)
		this->events.insert({ i->key, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		throw std::exception(
			std::string("[Table::setEvents] The array operator used is invalid!\n").c_str()
		);
}

void Table::setFacts(std::vector<FactRef> facts, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->facts.clear();

	for (FactRef i : facts)
		this->facts.insert({ i->key, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		throw std::exception(
			std::string("[Table::setFacts] The array operator used is invalid!\n").c_str()
		);
}

void Table::setRules(std::vector<RuleRef> rules, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->rules.clear();

	for (RuleRef i : rules)
		this->rules.insert({ i->key, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		throw std::exception(
			std::string("[Table::setRules] The array operator used is invalid!\n").c_str()
		);
}

EntryRef Table::getEntry(std::string key)
{
	if (this->events.count(key) == 1)
		return this->events[key];
	else if (this->facts.count(key) == 1)
		return this->facts[key];
	else if (this->rules.count(key) == 1)
		return this->rules[key];
	else
		throw std::exception(
			std::string("[Table::getEntry] No entry with key " + key + " was found!\n").c_str()
		);

	return EntryRef();
}

EntryRef Table::getEntry(int id)
{
	std::string key = this->idToKey[id];

	if (this->events.count(key) == 1)
		return this->events[key];
	else if (this->facts.count(key) == 1)
		return this->facts[key];
	else if (this->rules.count(key) == 1)
		return this->rules[key];
	else
		throw std::exception(
			std::string("[Table::getEntry] No entry with ID " + std::to_string(id) + " was found!\n").c_str()
		);

	return EntryRef();
}
