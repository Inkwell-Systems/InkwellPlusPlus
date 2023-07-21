#pragma once	

#include <string>
#include <format>
#include "../nlohmann/json.hpp"
#include "inkwellTable.hpp"
#include "inkwellErrors.hpp"

namespace inkwell
{
	constexpr int PROJECTFLAG_NONE						= 0;			// No special flags
	constexpr int PROJECTFLAG_NOCLOUD					= (1 << 0);		// Cloud field will not be read from input
	constexpr int PROJECTFLAG_NOENTRYMAP				= (1 << 1);		// EntryMap field will not be read from input
	constexpr int PROJECTFLAG_NOINVITECODE				= (1 << 2);		// InviteCode field will not be read from input
	constexpr int PROJECTFLAG_NOOWNER					= (1 << 3);		// Owner field will not be read from input
	constexpr int PROJECTFLAG_NOBANNER					= (1 << 4);		// ProjectBanner field will not be read from input
	constexpr int PROJECTFLAG_NOCREATEDAT				= (1 << 5);		// ProjectCreatedAt field will not be read from input
	constexpr int PROJECTFLAG_NODESCRIPTION				= (1 << 6);		// ProjectDescription field will not be read from input
	constexpr int PROJECTFLAG_NOID						= (1 << 7);		// ProjectId field will not be read from input
	constexpr int PROJECTFLAG_NONAME					= (1 << 8);		// ProjectName field will not be read from input
	constexpr int PROJECTFLAG_NOMEMBERS					= (1 << 9);		// Members field will not be read from input
	constexpr int PROJECTFLAG_NOEXTRAINFO				= (1 << 10);	// All of the above flags

	//constexpr int PROJECTFLAG_NOEXCEPT_OUTPUT			= (1 << 11);	// No exceptions will be thrown by the project, but the error will be outputted to the console
	//constexpr int PROJECTFLAG_NOEXCEPT				= (1 << 12);	// No exceptions will be thrown by the project and no error will be outputted to the console
	//constexpr int PROJECTFLAG_FLUSH_UPON_EXCEPTION	= (1 << 13);	// Outputs the project to a 'projectFlush.json' file upon exception

	class Project
	{
	private:
		bool initialized = false;

		std::unordered_map<int, std::string> idToKey;
		std::unordered_map<std::string, std::shared_ptr<Table>> tables;
		nlohmann::json extraData;
		int flags = 0;
		int tabIndex = 0;

		void parseExtraData(std::shared_ptr<Project> project, nlohmann::json& parsedJson)
		{
			int flags = project->getFlags();

			if ((flags & PROJECTFLAG_NOEXTRAINFO)) return;

			if ((flags & PROJECTFLAG_NOCLOUD) == 0)			project->extraData["cloud"] = parsedJson.at("cloud");
			if ((flags & PROJECTFLAG_NOENTRYMAP) == 0)		project->extraData["entryMap"] = parsedJson.at("entryMap");
			if ((flags & PROJECTFLAG_NOINVITECODE) == 0)	project->extraData["inviteCode"] = parsedJson.at("inviteCode");
			if ((flags & PROJECTFLAG_NOOWNER) == 0)			project->extraData["owner"] = parsedJson.at("owner");
			if ((flags & PROJECTFLAG_NOBANNER) == 0)		project->extraData["projectBanner"] = parsedJson.at("projectBanner");
			if ((flags & PROJECTFLAG_NOCREATEDAT) == 0)		project->extraData["projectCreatedAt"] = parsedJson.at("projectCreatedAt");
			if ((flags & PROJECTFLAG_NODESCRIPTION) == 0)	project->extraData["projectDescription"] = parsedJson.at("projectDescription");
			if ((flags & PROJECTFLAG_NOID) == 0)			project->extraData["projectId"] = parsedJson.at("projectId");
			if ((flags & PROJECTFLAG_NONAME) == 0)			project->extraData["projectName"] = parsedJson.at("projectName");
			if ((flags & PROJECTFLAG_NOMEMBERS) == 0)		project->extraData["members"] = parsedJson.at("members");
		}

		void parseCriteria(std::shared_ptr<Rule> rule, nlohmann::json& parsedRule)
		{
			for (auto& parsedCriterion : parsedRule["ruleCriteria"])
			{
				std::shared_ptr<Criterion> tempCriterion = std::make_shared<Criterion>(
					parsedCriterion["comparedEntry"],
					parsedCriterion["compareValue"],
					EnumConverter::toKey(parsedCriterion["comparisonOperator"])
				);

				rule->criteria.push_back(tempCriterion);
			}
		}

		void parseModifications(std::shared_ptr<Rule> rule, nlohmann::json& parsedRule)
		{
			for (auto& parsedModification : parsedRule["ruleModifications"])
			{
				std::shared_ptr<Modification> tempModification = std::make_shared<Modification>(
					parsedModification["modifiedEntry"],
					EnumConverter::toKey(parsedModification["modificationOperator"]),
					parsedModification["modifyWithValue"]
				);

				rule->modifications.push_back(tempModification);
			}
		}

		void parseEvents(std::shared_ptr<Table> table, nlohmann::json& parsedTable, std::unordered_map<int, std::vector<int>>& tempEventTriggersMap)
		{
			for (auto& parsedEvent : parsedTable["events"])
			{
				std::shared_ptr<Event> tempEvent = std::make_shared<Event>(
					parsedEvent["id"],
					parsedEvent["key"],
					parsedEvent["value"],
					parsedEvent["eventGuard"]
				);

				for (auto& parsedTriggers : parsedEvent["triggers"])
				{
					tempEventTriggersMap[tempEvent->id].push_back(parsedTriggers);
				}

				table->addEvent(tempEvent);
			}
		}

		void linkEventEntries(std::shared_ptr<Table> table, std::unordered_map<int, std::vector<int>>& tempEventTriggersMap)
		{
			for (auto& pairEvent : table->events)
			{
				for (auto& triggeredEntry : tempEventTriggersMap[pairEvent.second->id])
				{
					pairEvent.second->triggers.insert(table->getEntry(triggeredEntry));
				}
			}
		}

		void parseFacts(std::shared_ptr<Table> table, nlohmann::json& parsedTable)
		{
			for (auto& parsedFact : parsedTable["facts"])
			{
				std::shared_ptr<Fact> tempFact = std::make_shared<Fact>(
					parsedFact["id"],
					parsedFact["key"],
					parsedFact["value"],
					parsedFact["factGuard"]
				);

				table->addFact(tempFact);
			}
		}

		void parseRules(std::shared_ptr<Table> table, nlohmann::json& parsedTable, std::unordered_map<int, std::vector<int>>& tempRuleTriggersMap)
		{
			for (auto& parsedRule : parsedTable["rules"])
			{
				std::shared_ptr<Rule> tempRule = std::make_shared<Rule>(
					parsedRule["id"],
					parsedRule["key"],
					parsedRule["value"],
					parsedRule["ruleGuard"]
				);

				for (auto& parsedTriggers : parsedRule["triggers"])
				{
					tempRuleTriggersMap[tempRule->id].push_back(parsedTriggers);
				}

				parseCriteria(
					tempRule,
					parsedRule
				);

				parseModifications(
					tempRule,
					parsedRule
				);

				table->addRule(tempRule);
			}
		}

		void linkRuleEntries(std::shared_ptr<Table> table, std::unordered_map<int, std::vector<int>>& tempRuleTriggersMap)
		{
			for (auto& pairRule : table->rules)
			{
				for (auto& triggeredEntry : tempRuleTriggersMap[pairRule.second->id])
				{
					pairRule.second->triggers.insert(table->getEntry(triggeredEntry));
				}
			}

			for (auto& pairRule : table->rules)
			{
				for (auto& criterion : pairRule.second->criteria)
				{
					criterion->comparedEntry = table->getEntry(criterion->comparedEntryID);
				}
				for (auto& modification : pairRule.second->modifications)
				{
					modification->modifiedEntry = table->getEntry(modification->modifiedEntryID);
				}
			}
		}

		void parseTables(std::shared_ptr<Project> project, nlohmann::json& parsedJson)
		{
			for (auto& parsedTable : parsedJson["tables"])
			{
				std::shared_ptr<Table> table = std::make_shared<Table>(
					parsedTable["id"],
					parsedTable["key"]
				);

				std::unordered_map<int, std::vector<int>> tempEventTriggersMap;
				std::unordered_map<int, std::vector<int>> tempRuleTriggersMap;

				parseEvents(
					table,
					parsedTable,
					tempEventTriggersMap
				);

				parseFacts(
					table,
					parsedTable
				);

				parseRules(
					table,
					parsedTable,
					tempRuleTriggersMap
				);

				linkEventEntries(
					table,
					tempEventTriggersMap
				);

				linkRuleEntries(
					table,
					tempRuleTriggersMap
				);

				project->addTable(table);
			}
		}

		void parseScopes(std::shared_ptr<Project> project, nlohmann::json& parsedJson)
		{
			for (auto& parsedScope : parsedJson["scopes"])
			{
				std::shared_ptr<Scope> tempScope = std::make_shared<Scope>();
				std::shared_ptr<Table> tempTable;

				int tableID = parsedScope["id"];
				tempTable = project->tables[project->idToKey[tableID]];

				std::string strScopeKey = parsedScope["key"];
				tempScope->strKey = strScopeKey;
				if (strScopeKey == "global")
					tempScope->key = Scope::ScopeKey::GLOBAL;
				else
					tempScope->key = Scope::ScopeKey::LOCAL;

				tempScope->level = parsedScope["level"];

				tempTable->scope = tempScope;
			}
		}

		void parseProject(std::shared_ptr<Project> project, std::istream& fileInput)
		{
			nlohmann::json parsedJson;

			fileInput >> parsedJson;

			parseExtraData(project, parsedJson);

			parseTables(project, parsedJson);

			parseScopes(project, parsedJson);
		}
		
		void tab(std::ostream& fileOutput)
		{
			for (int i = 0; i < tabIndex; i++)
			{
				fileOutput << "\t";
			}
		}

		void writeExtras(std::shared_ptr<Project> project, std::ostream& fileOutput)
		{
			int flags = project->getFlags();

			if ((flags & PROJECTFLAG_NOEXTRAINFO)) return;

			if ((flags & PROJECTFLAG_NOCLOUD) == 0)			tab(fileOutput), fileOutput << "\"cloud\": " << project->extraData.at("cloud") << ",\n";
			if ((flags & PROJECTFLAG_NOENTRYMAP) == 0)		tab(fileOutput), fileOutput << "\"entryMap\": " << project->extraData.at("entryMap") << ",\n";
			if ((flags & PROJECTFLAG_NOINVITECODE) == 0)	tab(fileOutput), fileOutput << "\"inviteCode\": " << project->extraData.at("inviteCode") << ",\n";
			if ((flags & PROJECTFLAG_NOOWNER) == 0)			tab(fileOutput), fileOutput << "\"owner\": " << project->extraData.at("owner") << ",\n";
			if ((flags & PROJECTFLAG_NOBANNER) == 0)		tab(fileOutput), fileOutput << "\"projectBanner\": " << project->extraData.at("projectBanner") << ",\n";
			if ((flags & PROJECTFLAG_NOCREATEDAT) == 0)		tab(fileOutput), fileOutput << "\"createdAt\": " << project->extraData.at("projectCreatedAt") << ",\n";
			if ((flags & PROJECTFLAG_NODESCRIPTION) == 0)	tab(fileOutput), fileOutput << "\"projectDescription\": " << project->extraData.at("projectDescription") << ",\n";
			if ((flags & PROJECTFLAG_NOID) == 0)			tab(fileOutput), fileOutput << "\"projectId\": " << project->extraData.at("projectId") << ",\n";
			if ((flags & PROJECTFLAG_NONAME) == 0)			tab(fileOutput), fileOutput << "\"projectName\": " << project->extraData.at("projectName") << ",\n";
			if ((flags & PROJECTFLAG_NOMEMBERS) == 0)		tab(fileOutput), fileOutput << "\"members\": " << project->extraData.at("members") << ",\n";
		}

		void writeScopes(std::shared_ptr<Project> project, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"scopes\": [\n";
			int currentIndex = 0;

			for (auto& pairTable : project->tables)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairTable.second->id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairTable.second->scope->strKey << "\",\n";
				tab(fileOutput); fileOutput << "\"level\": " << pairTable.second->scope->level << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != project->tables.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeCriteria(std::shared_ptr<Rule> rule, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"criteria\": [\n";
			int currentIndex = 0;

			for (auto& criterion : rule->criteria)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"comparedEntry\": " << criterion->comparedEntryID << ",\n";
				tab(fileOutput); fileOutput << "\"compareValue\": " << criterion->compareValue << ",\n";
				tab(fileOutput); fileOutput << "\"comparisonOperator\": \"" << EnumConverter::toString(criterion->comparisonOperator) << "\"\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != rule->criteria.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeModifications(std::shared_ptr<Rule> rule, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"modifications\": [\n";
			int currentIndex = 0;

			for (auto& modification : rule->modifications)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"modifiedEntry\": " << modification->modifiedEntryID << ",\n";
				tab(fileOutput); fileOutput << "\"modifyWithValue\": " << modification->modifyWithValue << ",\n";
				tab(fileOutput); fileOutput << "\"modificationOperator\": \"" << EnumConverter::toString(modification->modificationOperator) << "\"\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != rule->modifications.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeEvents(std::shared_ptr<Table> table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"events\": {\n";
			int currentIndex = 0;

			for (auto& pairEvent : table->events)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairEvent.second->id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairEvent.second->id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairEvent.second->key << "\",\n";

				tab(fileOutput); fileOutput << "\"triggers\": [\n";
				++tabIndex;
				int triggerIndex = 0;
				for (auto& triggeredEntry : pairEvent.second->triggers)
				{
					tab(fileOutput); fileOutput << triggeredEntry->id;
					++triggerIndex;
					if (triggerIndex != pairEvent.second->triggers.size())
						fileOutput << ",";
					fileOutput << "\n";
				}
				--tabIndex;
				tab(fileOutput); fileOutput << "],\n";

				tab(fileOutput); fileOutput << "\"value\": " << pairEvent.second->value << ",\n";
				tab(fileOutput); fileOutput << "\"eventGuard\": " << pairEvent.second->guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table->events.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "},\n";
		}

		void writeFacts(std::shared_ptr<Table> table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"facts\": {\n";
			int currentIndex = 0;

			for (auto& pairFact : table->facts)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairFact.second->id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairFact.second->id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairFact.second->key << "\",\n";
				tab(fileOutput); fileOutput << "\"value\": " << pairFact.second->value << ",\n";
				tab(fileOutput); fileOutput << "\"factGuard\": " << pairFact.second->guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table->facts.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "},\n";
		}

		void writeRules(std::shared_ptr<Table> table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"rules\": {\n";
			int currentIndex = 0;

			for (auto& pairRule : table->rules)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairRule.second->id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairRule.second->id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairRule.second->key << "\",\n";

				tab(fileOutput); fileOutput << "\"triggers\": [\n";
				++tabIndex;
				int triggerIndex = 0;
				for (auto& triggeredEntry : pairRule.second->triggers)
				{
					tab(fileOutput); fileOutput << triggeredEntry->id;
					++triggerIndex;
					if (triggerIndex != pairRule.second->triggers.size())
						fileOutput << ",";
					fileOutput << "\n";
				}
				--tabIndex;
				tab(fileOutput); fileOutput << "],\n";

				writeCriteria(pairRule.second, fileOutput);
				writeModifications(pairRule.second, fileOutput);

				tab(fileOutput); fileOutput << "\"value\": " << pairRule.second->value << ",\n";
				tab(fileOutput); fileOutput << "\"ruleGuard\": " << pairRule.second->guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table->rules.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "}\n";
		}

		void writeTables(std::shared_ptr<Project> project, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"tables\": {\n";
			int currentIndex = 0;

			for (auto& pairTable : project->tables)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairTable.second->id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairTable.second->id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairTable.second->key << "\",\n";

				writeEvents(pairTable.second, fileOutput);
				writeFacts(pairTable.second, fileOutput);
				writeRules(pairTable.second, fileOutput);

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != project->tables.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "}\n";
		}

		void writeProject(std::shared_ptr<Project> project, std::ostream& fileOutput)
		{
			tabIndex = 0;

			tab(fileOutput); fileOutput << "{\n";
			++tabIndex;

			writeExtras(project, fileOutput);
			writeScopes(project, fileOutput);
			writeTables(project, fileOutput);

			--tabIndex;
			tab(fileOutput); fileOutput << "}\n";
		}

		void addTable(std::shared_ptr<Table> table)
		{
			if (idToKey.contains(table->id))
			{
				Error::throwException(
					std::format(
						"A Table with the ID {} has already been added!\n",
						table->id
					)
				);
			};

			if (tables.contains(table->key))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" has already been added!\n",
						table->key
					)
				);
			};

			tables[table->key] = table;
			idToKey[table->id] = table->key;
		}

	public:
		Project() 
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Project ({}) has already been initialized!\n",
						(this->flags & PROJECTFLAG_NOID) == 0 ? (std::string)this->extraData.at("ProjectId") : "No ID included"
					)
				);
			}

			this->flags = PROJECTFLAG_NONE;
		};

		Project(int flagList)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Project ({}) has already been initialized!\n",
						(this->flags & PROJECTFLAG_NOID) == 0 ? (std::string)this->extraData.at("ProjectId") : "No ID included"
					)
				);
			}

			this->flags = flagList;
		}

		int getFlags() const
		{
			return this->flags;
		}

		nlohmann::json getExtraData() const
		{
			return this->extraData;
		}

		bool INIT() const
		{
			return this->initialized;
		}

		std::shared_ptr<Table> T(int id)
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

			if (!tables.contains(idToKey[id]))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						idToKey[id]
					)
				);
			};

			return tables[idToKey[id]];
		}
		std::shared_ptr<Table> T(std::string key)
		{
			if (!tables.contains(key))
			{
				Error::throwException(
					std::format(
						"A Table with the key \"{}\" does not exist!\n",
						key
					)
				);
			};

			return tables[key];
		}
		
		friend std::istream& operator>> (std::istream& is, std::shared_ptr<Project> project)
		{
			if (project->initialized)
			{
				Error::throwException(
					std::format(
						"This Project ({}) has already been initialized!\n",
						(project->flags & PROJECTFLAG_NOID) == 0 ? (std::string)project->extraData.at("ProjectId") : "No ID included"
					)
				);
			}

			project->parseProject(project, is);
			project->initialized = true;
			return is;
		}

		friend std::ostream& operator<< (std::ostream& os, std::shared_ptr<Project> project)
		{
			project->writeProject(project, os);
			return os;
		}
	};
}
