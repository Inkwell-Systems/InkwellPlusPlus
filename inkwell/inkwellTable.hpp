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

			return nullptr;
		}

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

			return nullptr;
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

		int ID() const
		{
			return this->id;
		}

		std::string KEY() const
		{
			return this->key;
		}

		std::shared_ptr<Scope> SC() const
		{
			return this->scope;
		}

		bool INIT() const
		{
			return this->initialized;
		}

		std::shared_ptr<Event> E(int id)
		{
			if (!idToKey.contains(id))
			{
				Error::throwException(
					std::format(
						"A Table with the ID {} does not exist!\n",
						id
					)
				);
			}

			if (!events.contains(idToKey[id]))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						idToKey[id]
					)
				);
			};

			return events[idToKey[id]];
		}
		std::shared_ptr<Event> E(std::string key)
		{
			if (!events.contains(key))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						key
					)
				);
			};

			return events[key];
		}

		std::shared_ptr<Fact> F(int id)
		{
			if (!idToKey.contains(id))
			{
				Error::throwException(
					std::format(
						"A Table with the ID {} does not exist!\n",
						id
					)
				);
			}

			if (!facts.contains(idToKey[id]))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						idToKey[id]
					)
				);
			};

			return facts[idToKey[id]];
		}
		std::shared_ptr<Fact> F(std::string key)
		{
			if (!facts.contains(key))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						key
					)
				);
			};

			return facts[key];
		}

		std::shared_ptr<Rule> R(int id)
		{
			if (!idToKey.contains(id))
			{
				Error::throwException(
					std::format(
						"A Table with the ID {} does not exist!\n",
						id
					)
				);
			}

			if (!rules.contains(idToKey[id]))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						idToKey[id]
					)
				);
			};

			return rules[idToKey[id]];
		}
		std::shared_ptr<Rule> R(std::string key)
		{
			if (!rules.contains(key))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						key
					)
				);
			};

			return rules[key];
		}

		friend class Project;
	};
}
