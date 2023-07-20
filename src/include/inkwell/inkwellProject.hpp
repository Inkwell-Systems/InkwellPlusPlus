#pragma once	

#include <string>
#include "nlohmann/json.hpp"
#include "inkwellTable.hpp"

namespace inkwell
{
	constexpr int PROJECTFLAG_NONE				= 0;
	constexpr int PROJECTFLAG_NOCLOUD			= (1 << 0);
	constexpr int PROJECTFLAG_NOENTRYMAP		= (1 << 1);
	constexpr int PROJECTFLAG_NOINVITECODE		= (1 << 2);
	constexpr int PROJECTFLAG_NOOWNER			= (1 << 3);
	constexpr int PROJECTFLAG_NOBANNER			= (1 << 4);
	constexpr int PROJECTFLAG_NOCREATEDAT		= (1 << 5);
	constexpr int PROJECTFLAG_NODESCRIPTION		= (1 << 6);
	constexpr int PROJECTFLAG_NOID				= (1 << 7);
	constexpr int PROJECTFLAG_NONAME			= (1 << 8);
	constexpr int PROJECTFLAG_NOMEMBERS			= (1 << 9);
	constexpr int PROJECTFLAG_NOEXTRAINFO		= (1 << 10);

	class Project
	{
	private:
		int flags = 0;
		int tabIndex = 0;

		void parseExtraData(Project project, nlohmann::json& parsedJson)
		{
			int flags = project.getFlags();

			if ((flags & PROJECTFLAG_NOEXTRAINFO)) return;

			if ((flags & PROJECTFLAG_NOCLOUD) == 0)			project.extraData["cloud"] = parsedJson.at("cloud");
			if ((flags & PROJECTFLAG_NOENTRYMAP) == 0)		project.extraData["entryMap"] = parsedJson.at("entryMap");
			if ((flags & PROJECTFLAG_NOINVITECODE) == 0)	project.extraData["inviteCode"] = parsedJson.at("inviteCode");
			if ((flags & PROJECTFLAG_NOOWNER) == 0)			project.extraData["owner"] = parsedJson.at("owner");
			if ((flags & PROJECTFLAG_NOBANNER) == 0)		project.extraData["projectBanner"] = parsedJson.at("projectBanner");
			if ((flags & PROJECTFLAG_NOCREATEDAT) == 0)		project.extraData["projectCreatedAt"] = parsedJson.at("projectCreatedAt");
			if ((flags & PROJECTFLAG_NODESCRIPTION) == 0)	project.extraData["projectDescription"] = parsedJson.at("projectDescription");
			if ((flags & PROJECTFLAG_NOID) == 0)			project.extraData["projectId"] = parsedJson.at("projectId");
			if ((flags & PROJECTFLAG_NONAME) == 0)			project.extraData["projectName"] = parsedJson.at("projectName");
			if ((flags & PROJECTFLAG_NOMEMBERS) == 0)		project.extraData["members"] = parsedJson.at("members");
		}

		void parseCriteria(Rule rule, nlohmann::json& parsedRule)
		{
			for (auto& parsedCriterion : parsedRule["ruleCriteria"])
			{
				Criterion tempCriterion(
					parsedCriterion["comparedEntry"],
					parsedCriterion["compareValue"],
					EnumConverter::toKey(parsedCriterion["comparisonOperator"])
				);

				rule.criteria.push_back(tempCriterion);
			}
		}

		void parseModifications(Rule rule, nlohmann::json& parsedRule)
		{
			for (auto& parsedModification : parsedRule["ruleModifications"])
			{
				Modification tempModification(
					parsedModification["modifiedEntry"],
					EnumConverter::toKey(parsedModification["modificationOperator"]),
					parsedModification["modifyWithValue"]
				);

				rule.modifications.push_back(tempModification);
			}
		}

		void parseEvents(Table table, nlohmann::json& parsedTable, std::unordered_map<int, std::vector<int>>& tempEventTriggersMap)
		{
			for (auto& parsedEvent : parsedTable["events"])
			{
				Event tempEvent(
					parsedEvent["eventGuard"],
					parsedEvent["id"],
					parsedEvent["key"],
					parsedEvent["value"]
				);

				for (auto& parsedTriggers : parsedEvent["triggers"])
				{
					tempEventTriggersMap[tempEvent.id].push_back(parsedTriggers);
				}

				table.addEvent(tempEvent);
			}
		}

		void linkEventEntries(Table table, std::unordered_map<int, std::vector<int>>& tempEventTriggersMap)
		{
			for (auto& pairEvent : table.events)
			{
				for (auto& triggeredEntry : tempEventTriggersMap[pairEvent.second.id])
				{
					pairEvent.second.triggers.insert(table.getEntry(triggeredEntry));
				}
			}
		}

		void parseFacts(Table table, nlohmann::json& parsedTable)
		{
			for (auto& parsedFact : parsedTable["facts"])
			{
				Fact tempFact(
					parsedFact["factGuard"],
					parsedFact["id"],
					parsedFact["key"],
					parsedFact["value"]
				);

				table.addFact(tempFact);
			}
		}

		void parseRules(Table table, nlohmann::json& parsedTable, std::unordered_map<int, std::vector<int>>& tempRuleTriggersMap)
		{
			for (auto& parsedRule : parsedTable["rules"])
			{
				Rule tempRule(
					parsedRule["ruleGuard"],
					parsedRule["id"],
					parsedRule["key"],
					parsedRule["value"]
				);

				for (auto& parsedTriggers : parsedRule["triggers"])
				{
					tempRuleTriggersMap[tempRule.id].push_back(parsedTriggers);
				}

				parseCriteria(
					tempRule,
					parsedRule
				);

				parseModifications(
					tempRule,
					parsedRule
				);

				table.addRule(tempRule);
			}
		}

		void linkRuleEntries(Table table, std::unordered_map<int, std::vector<int>>& tempRuleTriggersMap)
		{
			for (auto& pairRule : table.rules)
			{
				for (auto& triggeredEntry : tempRuleTriggersMap[pairRule.second.id])
				{
					pairRule.second.triggers.insert(table.getEntry(triggeredEntry));
				}
			}

			for (auto& pairRule : table.rules)
			{
				for (auto& criterion : pairRule.second.criteria)
				{
					criterion.comparedEntry = table.getEntry(criterion.comparedEntryID);
				}
				for (auto& modification : pairRule.second.modifications)
				{
					modification.modifiedEntry = table.getEntry(modification.modifiedEntryID);
				}
			}
		}

		void parseTables(Project project, nlohmann::json& parsedJson)
		{
			for (auto& parsedTable : parsedJson["tables"])
			{
				Table table;
				std::unordered_map<int, std::vector<int>> tempEventTriggersMap;
				std::unordered_map<int, std::vector<int>> tempRuleTriggersMap;

				table.id = parsedTable["id"];
				table.key = parsedTable["key"];

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

				project.addTable(table);
			}
		}

		void parseScopes(Project project, nlohmann::json& parsedJson)
		{
			for (auto& parsedScope : parsedJson["scopes"])
			{
				Scope tempScope;
				Table tempTable;

				int tableID = parsedScope["id"];
				tempTable = project.tables[project.idToKey[tableID]];

				std::string strScopeKey = parsedScope["key"];
				tempScope.strKey = strScopeKey;
				if (strScopeKey == "global")
					tempScope.key = Scope::ScopeKey::GLOBAL;
				else
					tempScope.key = Scope::ScopeKey::LOCAL;

				tempScope.level = parsedScope["level"];

				tempTable.scope = tempScope;
			}
		}

		void parseProject(Project project, std::istream& fileInput)
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

		void writeExtras(Project project, std::ostream& fileOutput)
		{
			int flags = project.getFlags();

			if ((flags & PROJECTFLAG_NOEXTRAINFO)) return;

			if ((flags & PROJECTFLAG_NOCLOUD) == 0)			tab(fileOutput); fileOutput << "\"cloud\": " << project.extraData.at("cloud") << ",\n";
			if ((flags & PROJECTFLAG_NOENTRYMAP) == 0)		tab(fileOutput); fileOutput << "\"entryMap\": " << project.extraData.at("entryMap") << ",\n";
			if ((flags & PROJECTFLAG_NOINVITECODE) == 0)	tab(fileOutput); fileOutput << "\"inviteCode\": " << project.extraData.at("inviteCode") << ",\n";
			if ((flags & PROJECTFLAG_NOOWNER) == 0)			tab(fileOutput); fileOutput << "\"owner\": " << project.extraData.at("owner") << ",\n";
			if ((flags & PROJECTFLAG_NOBANNER) == 0)		tab(fileOutput); fileOutput << "\"projectBanner\": " << project.extraData.at("projectBanner") << ",\n";
			if ((flags & PROJECTFLAG_NOCREATEDAT) == 0)		tab(fileOutput); fileOutput << "\"createdAt\": " << project.extraData.at("projectCreatedAt") << ",\n";
			if ((flags & PROJECTFLAG_NODESCRIPTION) == 0)	tab(fileOutput); fileOutput << "\"projectDescription\": " << project.extraData.at("projectDescription") << ",\n";
			if ((flags & PROJECTFLAG_NOID) == 0)			tab(fileOutput); fileOutput << "\"projectId\": " << project.extraData.at("projectId") << ",\n";
			if ((flags & PROJECTFLAG_NONAME) == 0)			tab(fileOutput); fileOutput << "\"projectName\": " << project.extraData.at("projectName") << ",\n";
			if ((flags & PROJECTFLAG_NOMEMBERS) == 0)		tab(fileOutput); fileOutput << "\"members\": " << project.extraData.at("members") << ",\n";
		}

		void writeScopes(Project project, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"scopes\": [\n";
			int currentIndex = 0;

			for (auto& pairTable : project.tables)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairTable.second.id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairTable.second.scope.strKey << "\",\n";
				tab(fileOutput); fileOutput << "\"level\": " << pairTable.second.scope.level << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != project.tables.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeCriteria(Rule rule, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"criteria\": [\n";
			int currentIndex = 0;

			for (auto& criterion : rule.criteria)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"comparedEntry\": " << criterion.comparedEntryID << ",\n";
				tab(fileOutput); fileOutput << "\"compareValue\": " << criterion.compareValue << ",\n";
				tab(fileOutput); fileOutput << "\"comparisonOperator\": \"" << EnumConverter::toString(criterion.comparisonOperator) << "\"\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != rule.criteria.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeModifications(Rule rule, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"modifications\": [\n";
			int currentIndex = 0;

			for (auto& modification : rule.modifications)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "{\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"modifiedEntry\": " << modification.modifiedEntryID << ",\n";
				tab(fileOutput); fileOutput << "\"modifyWithValue\": " << modification.modifyWithValue << ",\n";
				tab(fileOutput); fileOutput << "\"modificationOperator\": \"" << EnumConverter::toString(modification.modificationOperator) << "\"\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != rule.modifications.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "],\n";
		}

		void writeEvents(Table table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"events\": {\n";
			int currentIndex = 0;

			for (auto& pairEvent : table.events)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairEvent.second.id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairEvent.second.id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairEvent.second.key << "\",\n";

				tab(fileOutput); fileOutput << "\"triggers\": [\n";
				++tabIndex;
				int triggerIndex = 0;
				for (auto& triggeredEntry : pairEvent.second.triggers)
				{
					tab(fileOutput); fileOutput << triggeredEntry->id;
					++triggerIndex;
					if (triggerIndex != pairEvent.second.triggers.size())
						fileOutput << ",";
					fileOutput << "\n";
				}
				--tabIndex;
				tab(fileOutput); fileOutput << "],\n";

				tab(fileOutput); fileOutput << "\"eventGuard\": " << pairEvent.second.guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table.events.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "},\n";
		}

		void writeFacts(Table table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"facts\": {\n";
			int currentIndex = 0;

			for (auto& pairFact : table.facts)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairFact.second.id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairFact.second.id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairFact.second.key << "\",\n";
				tab(fileOutput); fileOutput << "\"value\": " << pairFact.second.value << ",\n";
				tab(fileOutput); fileOutput << "\"factGuard\": " << pairFact.second.guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table.facts.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "},\n";
		}

		void writeRules(Table table, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"rules\": {\n";
			int currentIndex = 0;

			for (auto& pairRule : table.rules)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairRule.second.id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairRule.second.id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairRule.second.key << "\",\n";

				tab(fileOutput); fileOutput << "\"triggers\": [\n";
				++tabIndex;
				int triggerIndex = 0;
				for (auto& triggeredEntry : pairRule.second.triggers)
				{
					tab(fileOutput); fileOutput << triggeredEntry->id;
					++triggerIndex;
					if (triggerIndex != pairRule.second.triggers.size())
						fileOutput << ",";
					fileOutput << "\n";
				}
				--tabIndex;
				tab(fileOutput); fileOutput << "],\n";

				writeCriteria(pairRule.second, fileOutput);
				writeModifications(pairRule.second, fileOutput);

				tab(fileOutput); fileOutput << "\"ruleGuard\": " << pairRule.second.guard << "\n";

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != table.rules.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "}\n";
		}

		void writeTables(Project project, std::ostream& fileOutput)
		{
			tab(fileOutput); fileOutput << "\"tables\": {\n";
			int currentIndex = 0;

			for (auto& pairTable : project.tables)
			{
				++tabIndex;
				tab(fileOutput); fileOutput << "\"" << pairTable.second.id << "\": {\n";
				++tabIndex;

				tab(fileOutput); fileOutput << "\"id\": " << pairTable.second.id << ",\n";
				tab(fileOutput); fileOutput << "\"key\": \"" << pairTable.second.key << "\",\n";

				writeEvents(pairTable.second, fileOutput);
				writeFacts(pairTable.second, fileOutput);
				writeRules(pairTable.second, fileOutput);

				--tabIndex;
				tab(fileOutput); fileOutput << "}";

				++currentIndex;
				if (currentIndex != project.tables.size())
					fileOutput << ",";
				tab(fileOutput); fileOutput << "\n";

				--tabIndex;
			}

			tab(fileOutput); fileOutput << "}\n";
		}

		void writeProject(Project project, std::ostream& fileOutput)
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

	public:
		Project() {};
		Project(int flagList)
		{
			flags = flagList;
		}

		std::unordered_map<int, std::string> idToKey;
		std::unordered_map<std::string, Table> tables;
		nlohmann::json extraData;

		void addTable(Table table)
		{
			if (idToKey.contains(table.id))
			{
				throw std::exception(
					std::string("A table with the ID " + std::to_string(table.id) + " already exists!").c_str()
				);
			}

			tables[table.key] = table;
			idToKey[table.id] = table.key;
		}

		int getFlags()
		{
			return flags;
		}

		Table& operator[](std::string key)
		{
			return tables[key];
		}

		Table& operator[](int id)
		{
			return tables[idToKey[id]];
		}
		
		friend std::istream& operator>> (std::istream& is, Project& project)
		{
			project.parseProject(project, is);
			return is;
		}

		friend std::ostream& operator<< (std::ostream& os, Project& project)
		{
			project.writeProject(project, os);
			return os;
		}
	};
}
