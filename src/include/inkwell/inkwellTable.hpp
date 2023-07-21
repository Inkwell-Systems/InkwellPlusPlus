#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <format>

#include "inkwellEntry.hpp"
#include "inkwellScope.hpp"
#include "inkwellErrors.hpp"

namespace inkwell
{
	class Table
	{
	private:
		bool initialized = false;

		int id = NULL;
		std::string key = "";
		std::shared_ptr<Scope> scope;

		std::unordered_map<int, std::string> idToKey;
		std::unordered_map<std::string, std::shared_ptr<Event>> events;
		std::unordered_map<std::string, std::shared_ptr<Fact>> facts;
		std::unordered_map<std::string, std::shared_ptr<Rule>> rules;

		std::shared_ptr<Entry> getEntry(std::string key)
		{
			if (this->events.contains(key))
				return this->events[key];
			else if (this->facts.contains(key))
				return this->facts[key];
			else if (this->rules.contains(key))
				return this->rules[key];

			Error::throwException(
				std::format(
					"No entry with the key \"{}\" was found!\n",
					key
				)
			);
		}

		std::shared_ptr<Entry> getEntry(int id)
		{
			if (!idToKey.contains(id))
			{
				Error::throwException(
					std::format(
						"No entry with the ID {} was found!\n",
						id
					)
				);
			}

			std::string key = this->idToKey[id];

			if (this->events.contains(key))
				return this->events[key];
			else if (this->facts.contains(key))
				return this->facts[key];
			else if (this->rules.contains(key))
				return this->rules[key];

			Error::throwException(
				std::format(
					"No entry with the ID {} was found!\n",
					id
				)
			);
		}

		void addEvent(std::shared_ptr<Event> _event)
		{
			if (idToKey.contains(_event->id))
			{
				Error::throwException(
					std::format(
						"An event with the ID {} already exists!\n",
						_event->id
					)
				);
			}

			this->events[_event->key] = _event;
			this->idToKey[_event->id] = _event->key;
		}

		void addFact(std::shared_ptr<Fact> fact)
		{
			if (idToKey.contains(fact->id))
			{
				Error::throwException(
					std::format(
						"A fact with the ID {} already exists!\n",
						fact->id
					)
				);
			}

			this->facts[fact->key] = fact;
			this->idToKey[fact->id] = fact->key;
		}

		void addRule(std::shared_ptr<Rule> rule)
		{
			if (idToKey.contains(rule->id))
			{
				Error::throwException(
					std::format(
						"A rule with the ID {} already exists!\n",
						rule->id
					)
				);
			}

			this->rules[rule->key] = rule;
			this->idToKey[rule->id] = rule->key;
		}

	public:
		Table()
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Table (ID {}, Key \"{}\") has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->initialized = true;
		}

		Table(int id, std::string key)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Table (ID {}, Key \"{}\") has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->id = id;
			this->key = key;
			this->initialized = true;
		}

		bool isInitialized() const
		{
			return this->initialized;
		}

		std::shared_ptr<Entry> at(std::string key)
		{
			return this->getEntry(key);
		}

		std::shared_ptr<Entry> at(int id)
		{
			return this->getEntry(id);
		}

		friend class Project;
	};
}
