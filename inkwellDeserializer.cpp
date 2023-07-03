#include "inkwellDeserializer.h"

using namespace inkwell;

Deserializer::Deserializer(const std::string filePath)
{
	try
	{
		this->fileInput.open(filePath);
	}
	catch (const std::exception&)
	{
		std::cout << "[Deserializer::Deserializer] Could not open file " << filePath << "!\n";
	}
}

Deserializer::~Deserializer()
{
	try
	{
		this->fileInput.close();
	}
	catch (const std::exception&)
	{
		std::cout << "[Deserializer::~Deserializer] Could not close file!\n";
	}
}

bool Deserializer::readInput()
{
	try
	{
		this->fileInput.get(this->read);
		if (fileInput.bad() or fileInput.fail())
			throw std::runtime_error("Could not read from file - bad bit!\n");
		if(fileInput.fail())
			throw std::runtime_error("Could not read from file - failbit!\n");
		if (fileInput.eof())
			return 0;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cout << "[Deserializer::readInput] " << e.what();
		return 0;
	}
}

std::string Deserializer::getBuffer()
{
	try
	{
		std::streamoff prevPos = this->fileInput.tellg();
		std::string buffer = "";
		const std::string stopAt = ",]}\"";

		while (!isalnum(this->read))
			this->readInput();

		while (stopAt.find(this->read) == std::string::npos)
		{
			buffer.push_back(this->read);
			prevPos = this->fileInput.tellg();
			this->readInput();
		}

		if (this->read != '"')
			this->fileInput.seekg(prevPos);

		return buffer;
	}
	catch (const std::exception& e)
	{
		std::cout << "[Deserializer::getBuffer] " << e.what();
		return "";
	}
}

int Deserializer::getInteger()
{
	std::string buffer = "";
	try
	{
		buffer = getBuffer();
		return std::stoi(buffer);
	}
	catch (const std::invalid_argument&)
	{
		std::cout << "[Deserializer::getInteger] Expected a number, got " << buffer << " instead!\n";
		return 0;
	}
	catch (const std::out_of_range&)
	{
		std::cout << "[Deserializer::getInteger] Number " << buffer << " is out of range!\n";
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "[Deserializer::getInteger] " << e.what();
		return 0;
	}
}

double Deserializer::getDouble()
{
	std::string buffer = "";
	try
	{
		buffer = getBuffer();
		return std::stod(buffer);
	}
	catch (const std::invalid_argument&)
	{
		std::cout << "[Deserializer::getDouble] Expected a number, got " << buffer << " instead!\n";
		return 0;
	}
	catch (const std::out_of_range&)
	{
		std::cout << "[Deserializer::getDouble] Number " << buffer << " is out of range!\n";
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "[Deserializer::getDouble] " << e.what();
		return 0;
	}
}

Keys Deserializer::getKey()
{
	std::string key;

	try
	{
		key = this->getBuffer();
		return EnumConverter::toKey(key);
	}
	catch (const std::exception& e)
	{
		std::cout << "[Deserializer::getKey] " << e.what();
		return Keys::NULL_KEY;
	}
}

std::vector<int> Deserializer::parseNumberArray()
{
	this->read = 0;
	std::vector<int> arr;

	while (this->read != '[')
		this->readInput();

	while (this->read != ']')
	{
		this->readInput();

		if (this->read == '-' or isdigit(this->read))
			arr.push_back(this->getInteger());
	}
	this->readInput();

	return arr;
}

std::vector<CriterionRef> Deserializer::parseCriteria(TableRef currentTable)
{
	try
	{
		this->read = 0;
		std::vector<CriterionRef> criteria;
		int nestingIndex = 1;

		while (this->read != '[')
		{
			this->readInput();
		}

		while (nestingIndex > 0)
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::COMPARED_ENTRY:
			{
				criteria.push_back(std::make_shared<Criterion>());
				criteria.back()->comparedEntry = currentTable->getEntry(this->getInteger());
				break;
			}
			case Keys::COMPARE_VALUE:
			{
				criteria.back()->compareValue = this->getDouble();
				break;
			}
			case Keys::COMPARISON_OPERATOR:
			{
				criteria.back()->comparisonOperator = this->getKey();
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseCriteria] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseCriteria] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return criteria;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return {};
	}
}

std::vector<ModificationRef> Deserializer::parseModifications(TableRef currentTable)
{
	try
	{
		this->read = 0;
		std::vector<ModificationRef> modifications;
		int nestingIndex = 1;

		while (this->read != '[')
			this->readInput();

		while (nestingIndex > 0)
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::MODIFIED_ENTRY:
			{
				modifications.push_back(std::make_shared<Modification>());
				modifications.back()->modifiedEntry = currentTable->getEntry(this->getInteger());
				break;
			}
			case Keys::MODIFICATION_OPERATOR:
			{
				modifications.back()->modificationOperator = this->getKey();
				break;
			}
			case Keys::MODIFY_WITH_VALUE:
			{
				modifications.back()->modifyWithValue = this->getDouble();
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseModifications] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseModifications] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return modifications;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return {};
	}
}

std::unordered_map<std::string, EventRef> Deserializer::parseEvents(TableRef currentTable)
{
	try
	{
		this->read = 0;

		EventRef currentEvent = nullptr;
		std::unordered_map<std::string, EventRef> events;
		int nestingIndex = 1;

		while (this->read != '[')
		{
			this->readInput();
		}

		while (nestingIndex > 0) // events don't have any further nesting atm but this is more expandable
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::ID:
			{
				currentEvent = std::make_shared<Event>();
				currentEvent->id = this->getInteger();
				break;
			}
			case Keys::KEY:
			{
				currentEvent->key = this->getBuffer();
				currentTable->idToKey.insert({ currentEvent->id, currentEvent->key });
				break;
			}
			case Keys::VALUE:
			{
				currentEvent->value = this->getDouble();
				events.insert({ currentEvent->key, currentEvent });
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseEvents] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseEvents] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return events;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return {};
	}
}

std::unordered_map<std::string, FactRef> Deserializer::parseFacts(TableRef currentTable)
{
	try
	{
		this->read = 0;

		FactRef currentFact = nullptr;
		std::unordered_map<std::string, FactRef> facts;
		int nestingIndex = 1;

		while (this->read != '[')
		{
			this->readInput();
		}

		while (nestingIndex > 0)
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::ID:
			{
				currentFact = std::make_shared<Fact>();
				currentFact->id = this->getInteger();
				break;
			}
			case Keys::KEY:
			{
				currentFact->key = this->getBuffer();
				currentTable->idToKey.insert({ currentFact->id, currentFact->key });
				break;
			}
			case Keys::VALUE:
			{
				currentFact->value = this->getDouble();
				facts.insert({ currentFact->key, currentFact });
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseFacts] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseFacts] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return facts;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return {};
	}
}

std::unordered_map<std::string, RuleRef> Deserializer::parseRules(TableRef currentTable)
{
	try
	{
		this->read = 0;

		RuleRef currentRule = nullptr;
		std::unordered_map<std::string, RuleRef> rules;
		int nestingIndex = 1;
		std::vector<int> v;

		while (this->read != '[')
		{
			this->readInput();
		}

		while (nestingIndex > 0)
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			else if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::ID:
			{
				currentRule = std::make_shared<Rule>();
				currentRule->id = this->getInteger();
				break;
			}
			case Keys::KEY:
			{
				currentRule->key = this->getBuffer();
				currentTable->idToKey.insert({ currentRule->id, currentRule->key });
				break;
			}
			case Keys::RULE_TRIGGERED_BY:
			{
				v = this->parseNumberArray();
				for (int i : v)
				{
					currentTable->events[currentTable->idToKey[i]]->triggers.push_back(currentRule);
				}
				break;
			}
			case Keys::RULE_TRIGGERS:
			{
				v = this->parseNumberArray();
				for (int i : v)
				{
					currentRule->triggers.push_back(currentTable->getEntry(i));
				}
				break;
			}
			case Keys::RULE_CRITERIA:
			{
				currentRule->criteria = this->parseCriteria(currentTable);
				break;
			}
			case Keys::RULE_MODIFICATIONS:
			{
				currentRule->modifications = this->parseModifications(currentTable);
				break;
			}
			case Keys::VALUE:
			{
				currentRule->value = this->getDouble();
				rules.insert({ currentRule->key, currentRule });
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseRules] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseRules] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return rules;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return {};
	}
}

std::unordered_map<std::string, TableRef> Deserializer::parseTables()
{
	try
	{
		this->read = 0;

		TableRef currentTable = nullptr;
		std::unordered_map<std::string, TableRef> tables;
		int nestingIndex = 1;

		while (this->read != '[')
			this->readInput();

		while (nestingIndex > 0)
		{
			this->readInput();

			if (this->read == '[' or this->read == '{')
			{
				++nestingIndex;
				continue;
			}
			else if (this->read == ']' or this->read == '}')
			{
				--nestingIndex;
				continue;
			}
			else if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::ID:
			{
				currentTable = std::make_shared<Table>();
				currentTable->id = this->getInteger();
				break;
			}
			case Keys::KEY:
			{
				currentTable->key = this->getBuffer();
				break;
			}
			case Keys::EVENTS:
			{
				currentTable->events = this->parseEvents(currentTable);
				break;
			}
			case Keys::FACTS:
			{
				currentTable->facts = this->parseFacts(currentTable);
				break;
			}
			case Keys::RULES:
			{
				currentTable->rules = this->parseRules(currentTable);
				tables.insert({ currentTable->key, currentTable });
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseTables] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseTables] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return tables;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return {};
	}
}

ProjectRef Deserializer::parseProject()
{
	try
	{
		ProjectRef project = std::make_shared<Project>();

		while (this->readInput())
		{
			if (this->read != '"')
				continue;

			Keys keyToken = this->getKey();

			switch (keyToken)
			{
			case Keys::PROJECT_NAME:
			{
				project->name = this->getBuffer();
				break;
			}
			case Keys::PROJECT_DESCRIPTION:
			{
				project->description = this->getBuffer();
				break;
			}
			case Keys::PROJECT_CREATEDAT:
			{
				project->createdAtNano = this->getInteger();
				break;
			}
			case Keys::TABLES:
			{
				project->tables = this->parseTables();
				break;
			}
			case Keys::NULL_KEY:
			{
				throw std::exception("[Deserializer::parseProject] Key is not recognized!\n");
			}
			default:
			{
				throw std::exception(
					std::string("[Deserializer::parseProject] Key " + EnumConverter::toString(keyToken) + " is not recognized!\n").c_str()
				);
			}
			}
		}

		return project;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return nullptr;
	}
}
