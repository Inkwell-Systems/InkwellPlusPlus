#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "inkwellEntry.hpp"
#include "inkwellScope.hpp"

namespace inkwell
{
	class Table
	{
	public:
		int id = NULL;
		std::string key = "";
		Scope scope;

		std::unordered_map<int, std::string> idToKey;
		std::unordered_map<std::string, Event> events;
		std::unordered_map<std::string, Fact> facts;
		std::unordered_map<std::string, Rule> rules;

		Entry* getEntry(std::string name)
		{
			if (this->events.count(key) == 1)
				return &(this->events[key]);
			else if (this->facts.count(key) == 1)
				return &(this->facts[key]);
			else if (this->rules.count(key) == 1)
				return &(this->rules[key]);

			throw std::exception(
				std::string("[Table::getEntry] No entry with key " + key + " was found!\n").c_str()
			);
		}

		Entry* getEntry(int id)
		{
			std::string key = this->idToKey[id];

			if (this->events.count(key) == 1)
				return &(this->events[key]);
			else if (this->facts.count(key) == 1)
				return &(this->facts[key]);
			else if (this->rules.count(key) == 1)
				return &(this->rules[key]);
			
			throw std::exception(
				std::string("[Table::getEntry] No entry with ID " + std::to_string(id) + " was found!\n").c_str()
			);
		}

		void addEvent(Event _event)
		{
			if (idToKey.contains(_event.id))
			{
				throw std::exception(
					std::string("An event with the ID " + std::to_string(_event.id) + " already exists!").c_str()
				);
			}

			this->events[_event.key] = _event;
			this->idToKey[_event.id] = _event.key;
		}

		void addFact(Fact fact)
		{
			if (idToKey.contains(fact.id))
			{
				throw std::exception(
					std::string("A fact with the ID " + std::to_string(fact.id) + " already exists!").c_str()
				);
			}

			this->facts[fact.key] = fact;
			this->idToKey[fact.id] = fact.key;
		}

		void addRule(Rule rule)
		{
			if (idToKey.contains(rule.id))
			{
				throw std::exception(
					std::string("A table with the ID " + std::to_string(rule.id) + " already exists!").c_str()
				);
			}

			this->rules[rule.key] = rule;
			this->idToKey[rule.id] = rule.key;
		}

		Entry& operator[](std::string key)
		{
			return *(this->getEntry(key));
		}

		Entry& operator[](int id)
		{
			return *(this->getEntry(id));
		}
	};
}
