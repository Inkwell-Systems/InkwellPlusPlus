#include "inkwellSerializer.h"

using namespace inkwell;

Serializer::Serializer(std::string filePath)
{
	try
	{
		this->fileOutput.open(filePath, std::ios::out);
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::Serializer] " << e.what() << std::endl;
	}
}

Serializer::~Serializer()
{
	try
	{
		this->fileOutput.close();
	}
	catch (const std::exception&)
	{
		std::cout << "[Serializer::~Serializer] Could not close file!" << std::endl;
	}
}

void Serializer::format()
{
	try
	{
		for (int i = 1; i <= globalNestingLevel; ++i)
			this->fileOutput << "\t";
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::format] " << e.what() << std::endl;
	}
}

void Serializer::startObject()
{
	try
	{
		++this->globalNestingLevel;
		this->format(); this->fileOutput << "{\n";
		++this->globalNestingLevel;
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::startObject] " << e.what() << std::endl;
	}
}

void Serializer::endObject(bool isLast)
{
	try
	{
		if (isLast)
		{
			--this->globalNestingLevel;
			this->format(); this->fileOutput << "}\n";
			--this->globalNestingLevel;
		}
		else
		{
			--this->globalNestingLevel;
			this->format(); this->fileOutput << "},\n";
			--this->globalNestingLevel;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::endObject] " << e.what() << std::endl;
	}
}

void Serializer::writeIntArray(std::vector<int> arr)
{
	try
	{
		int intCount = (int)arr.size(), intIndex = 0;

		for (auto i : arr)
		{
			++intIndex;

			if (intIndex == intCount)
				this->fileOutput << " " << i << " ";
			else
				this->fileOutput << " " << i << ",";
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeIntArray] " << e.what() << std::endl;
	}
}

void Serializer::writeObjArrayID(std::vector<EntryRef> triggers)
{
	try
	{
		int objCount = (int)triggers.size(), objIndex = 0;

		for (EntryRef i : triggers)
		{
			++objIndex;

			if (objIndex == objCount)
				this->fileOutput << " " << i->id << " ";
			else
				this->fileOutput << " " << i->id << ",";
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeObjArrayID] " << e.what() << std::endl;
	}
}

void Serializer::writeCriteria(std::vector<CriterionRef> criteria)
{
	try
	{
		int objCount = (int)criteria.size(), objIndex = 0;

		for (CriterionRef i : criteria)
		{
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"comparedEntry\": " << i->comparedEntry->id << ",\n";
			this->format(); this->fileOutput << "\"compareValue\": " << i->compareValue << ",\n";
			this->format(); this->fileOutput << "\"comparisonOperator\": \"" << EnumConverter::toString(i->comparisonOperator) << "\"\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeCriteria] " << e.what() << std::endl;
	}
}

void Serializer::writeModifications(std::vector<ModificationRef> modifications)
{
	try
	{
		int objCount = (int)modifications.size(), objIndex = 0;

		for (ModificationRef i : modifications)
		{
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"modifiedEntry\": " << i->modifiedEntry->id << ",\n";
			this->format(); this->fileOutput << "\"modificationOperator\": \"" << EnumConverter::toString(i->modificationOperator) << "\",\n";
			this->format(); this->fileOutput << "\"modifyWithValue\": " << i->modifyWithValue << "\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeModifications] " << e.what() << std::endl;
	}
}

void Serializer::writeEvents(std::unordered_map<std::string, EventRef> events)
{
	try
	{
		int objCount = (int)events.size(), objIndex = 0;

		for (auto& event : events)
		{
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"id\": " << event.second->id << ",\n";
			this->format(); this->fileOutput << "\"key\": \"" << event.second->key << "\",\n";
			this->format(); this->fileOutput << "\"value\": " << event.second->value << "\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeEvents] " << e.what() << std::endl;
	}
}

void Serializer::writeFacts(std::unordered_map<std::string, FactRef> facts)
{
	try
	{
		int objCount = (int)facts.size(), objIndex = 0;

		for (auto& fact : facts)
		{
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"id\": " << fact.second->id << ",\n";
			this->format(); this->fileOutput << "\"key\": \"" << fact.second->key << "\",\n";
			this->format(); this->fileOutput << "\"value\": " << fact.second->value << "\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeFacts] " << e.what() << std::endl;
	}
}

void Serializer::writeRules(std::unordered_map<std::string, RuleRef> rules, TableRef currentTable)
{
	try
	{
		int objCount = (int)rules.size(), objIndex = 0;

		for (auto& rule : rules)
		{
			std::vector<int> v;
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"id\": " << rule.second->id << ",\n";
			this->format(); this->fileOutput << "\"key\": \"" << rule.second->key << "\",\n";

			this->format(); this->fileOutput << "\"ruleTriggeredBy\": [";
			for (auto& i : currentTable->events)
			{
				for (RuleRef j : i.second->triggers)
					if (j->id == rule.second->id)
						v.push_back(i.second->id);
			}
			writeIntArray(v);
			this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"ruleTriggers\": [";
			writeObjArrayID(rule.second->triggers);
			this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"ruleCriteria\": [\n";
			this->writeCriteria(rule.second->criteria);
			this->format(); this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"ruleModifications\": [\n";
			this->writeModifications(rule.second->modifications);
			this->format(); this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"value\": " << rule.second->value << "\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeRules] " << e.what() << std::endl;
	}
}

void Serializer::writeTables(std::unordered_map<std::string, TableRef> tables)
{
	try
	{
		int objCount = (int)tables.size(), objIndex = 0;

		for (auto& table : tables)
		{
			++objIndex;
			this->startObject();

			this->format(); this->fileOutput << "\"id\": " << table.second->id << ",\n";
			this->format(); this->fileOutput << "\"key\": \"" << table.second->key << "\",\n";

			this->format(); this->fileOutput << "\"events\": [\n";
			this->writeEvents(table.second->events);
			this->format(); this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"facts\": [\n";
			this->writeFacts(table.second->facts);
			this->format(); this->fileOutput << "],\n";

			this->format(); this->fileOutput << "\"rules\": [\n";
			this->writeRules(table.second->rules, table.second);
			this->format(); this->fileOutput << "]\n";

			this->endObject(objIndex == objCount);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeTables] " << e.what() << std::endl;
	}
}

void Serializer::writeProject(ProjectRef project)
{
	try
	{
		this->fileOutput << "{\n";
		++this->globalNestingLevel;

		this->format(); this->fileOutput << "\"projectName\": \"" << project->name << "\",\n";
		this->format(); this->fileOutput << "\"projectDescription\": \"" << project->description << "\",\n";
		this->format(); this->fileOutput << "\"projectCreatedAt\": " << project->createdAtNano << ",\n";

		this->format(); this->fileOutput << "\"tables\": [\n";
		this->writeTables(project->tables);
		this->format(); this->fileOutput << "]\n";

		this->fileOutput << "}\n";
	}
	catch (const std::exception& e)
	{
		std::cout << "[Serializer::writeProject] " << e.what() << std::endl;
	}
}