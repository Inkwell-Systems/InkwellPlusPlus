#include "inkwell.h"

using namespace inkwell;

// GETTERS AND SETTERS

void Rule::setTriggeredBy(std::vector<int> triggeredBy, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->triggeredBy = triggeredBy;
	else if (operation == ArrayOperator::ADD)
		this->triggeredBy.insert(this->triggeredBy.end(), triggeredBy.begin(), triggeredBy.end());
	else
		std::cout << "error at Rule::setTriggeredBy\n";
}

void Rule::setTriggers(std::vector<int> triggers, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->triggers = triggers;
	else if (operation == ArrayOperator::ADD)
		this->triggers.insert(this->triggers.end(), triggers.begin(), triggers.end());
	else
		std::cout << "error at Rule::setTriggers\n";
}

void Rule::setCriteria(std::vector<Criterion> criteria, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->criteria = criteria;
	else if (operation == ArrayOperator::ADD)
		this->criteria.insert(this->criteria.end(), criteria.begin(), criteria.end());
	else
		std::cout << "error at Rule::setCriteria\n";
}

void Rule::setModifications(std::vector<Modification> modifications, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->modifications = modifications;
	else if (operation == ArrayOperator::ADD)
		this->modifications.insert(this->modifications.end(), modifications.begin(), modifications.end());
	else
		std::cout << "error at Rule::setModifications\n";
}

void Table::setEvents(std::vector<Event> events, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->events.clear();

	for (Event i : events)
		this->events.insert({ i.id, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		std::cout << "error at Table::setEvents\n";
}

void Table::setFacts(std::vector<Fact> facts, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->facts.clear();

	for (Fact i : facts)
		this->facts.insert({ i.id, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		std::cout << "error at Table::setFacts\n";
}

void Table::setRules(std::vector<Rule> rules, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->rules.clear();

	for (Rule i : rules)
		this->rules.insert({ i.id, i });

	if (operation != ArrayOperator::SET and operation != ArrayOperator::ADD)
		std::cout << "error at Table::setRules\n";
}

// ENUM CONVERTER CLASS

Keys EnumConverter::toKey(std::string key)
{
	if (key == "projectName")
		return Keys::PROJECT_NAME;
	else if (key == "projectDescription")
		return Keys::PROJECT_DESCRIPTION;
	else if (key == "projectCreatedAt")
		return Keys::PROJECT_CREATEDAT;
	else if (key == "id")
		return Keys::ID;
	else if (key == "key")
		return Keys::KEY;
	else if (key == "value")
		return Keys::VALUE;
	else if (key == "tables")
		return Keys::TABLES;
	else if (key == "events")
		return Keys::EVENTS;
	else if (key == "facts")
		return Keys::FACTS;
	else if (key == "rules")
		return Keys::RULES;
	else if (key == "factData")
		return Keys::FACT_DATA;
	else if (key == "ruleTriggeredBy")
		return Keys::RULE_TRIGGERED_BY;
	else if (key == "ruleTriggers")
		return Keys::RULE_TRIGGERS;
	else if (key == "ruleCriteria")
		return Keys::RULE_CRITERIA;
	else if (key == "ruleModifications")
		return Keys::RULE_MODIFICATIONS;
	else if (key == "comparedEntry")
		return Keys::COMPARED_ENTRY;
	else if (key == "compareValue")
		return Keys::COMPARE_VALUE;
	else if (key == "comparisonOperator")
		return Keys::COMPARISON_OPERATOR;
	else if (key == "equal")
		return Keys::COMPARISON_OPERATOR_EQUAL;
	else if (key == "notEqual")
		return Keys::COMPARISON_OPERATOR_NOT_EQUAL;
	else if (key == "greaterThan")
		return Keys::COMPARISON_OPERATOR_GREATER_THAN;
	else if (key == "greaterThanOrEqual")
		return Keys::COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL;
	else if (key == "lessThan")
		return Keys::COMPARISON_OPERATOR_LESS_THAN;
	else if (key == "lessThanOrEqual")
		return Keys::COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL;
	else if (key == "modifiedEntry")
		return Keys::MODIFIED_ENTRY;
	else if (key == "modificationOperator")
		return Keys::MODIFICATION_OPERATOR;
	else if (key == "set")
		return Keys::MODIFICATION_OPERATOR_SET;
	else if (key == "increment")
		return Keys::MODIFICATION_OPERATOR_INCREMENT;
	else if (key == "modifyWithValue")
		return Keys::MODIFY_WITH_VALUE;

	return Keys::NULL_KEY;
}

ArrayOperator EnumConverter::toArrayOperator(std::string key)
{
	if(key == "set")
		return ArrayOperator::SET;
	else if(key == "add")
		return ArrayOperator::ADD;

	return ArrayOperator::NULL_OPERATOR;
}

std::string EnumConverter::toString(Keys k)
{
	if (k == Keys::PROJECT_NAME)
		return "projectName";
	else if (k == Keys::PROJECT_DESCRIPTION)
		return "projectDescription";
	else if (k == Keys::PROJECT_CREATEDAT)
		return "projectCreatedAt";
	else if (k == Keys::ID)
		return "id";
	else if (k == Keys::KEY)
		return "key";
	else if (k == Keys::VALUE)
		return "value";
	else if (k == Keys::TABLES)
		return "tables";
	else if (k == Keys::EVENTS)
		return "events";
	else if (k == Keys::FACTS)
		return "facts";
	else if (k == Keys::RULES)
		return "rules";
	else if (k == Keys::FACT_DATA)
		return "factData";
	else if (k == Keys::RULE_TRIGGERED_BY)
		return "ruleTriggeredBy";
	else if (k == Keys::RULE_TRIGGERS)
		return "ruleTriggers";
	else if (k == Keys::RULE_CRITERIA)
		return "ruleCriteria";
	else if (k == Keys::RULE_MODIFICATIONS)
		return "ruleModifications";
	else if (k == Keys::COMPARED_ENTRY)
		return "comparedEntry";
	else if (k == Keys::COMPARE_VALUE)
		return "compareValue";
	else if (k == Keys::COMPARISON_OPERATOR)
		return "comparisonOperator";
	else if (k == Keys::COMPARISON_OPERATOR_EQUAL)
		return "equal";
	else if (k == Keys::COMPARISON_OPERATOR_NOT_EQUAL)
		return "notEqual";
	else if (k == Keys::COMPARISON_OPERATOR_GREATER_THAN)
		return "greaterThan";
	else if (k == Keys::COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL)
		return "greaterThanOrEqual";
	else if (k == Keys::COMPARISON_OPERATOR_LESS_THAN)
		return "lessThan";
	else if (k == Keys::COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL)
		return "lessThanOrEqual";
	else if(k == Keys::MODIFIED_ENTRY)
		return "modifiedEntry";
	else if(k == Keys::MODIFICATION_OPERATOR)
		return "modificationOperator";
	else if(k == Keys::MODIFICATION_OPERATOR_SET)
		return "set";
	else if(k == Keys::MODIFICATION_OPERATOR_INCREMENT)
		return "increment";
	else if(k == Keys::MODIFY_WITH_VALUE)
		return "modifyWithValue";

	return "nullKey";
}

std::string EnumConverter::toString(ArrayOperator a)
{
	if(a == ArrayOperator::SET)
		return "set";
	else if(a == ArrayOperator::ADD)
		return "add";

	return "nullOperator";
}

// DESERIALIZER

Deserializer::Deserializer(const std::string filePath)
{
	this->fileInput.open(filePath);
}

Deserializer::~Deserializer()
{
	this->fileInput.close();
}

std::string Deserializer::getNextString()
{
	std::string str = "";

	while (this->read != '"') // reach 1st quotation mark
		this->fileInput.get(this->read);
	this->fileInput.get(this->read);

	while (this->read != '"')
	{
		str += this->read;
		this->fileInput.get(this->read);
	}
	this->fileInput.get(this->read); // skip 2nd quotation mark

	return str;
}

int Deserializer::getNextInteger()
{
	int integer = 0;
	bool isNegative = 0;
	std::vector<int> digits;

	while (this->read != '-' and !isdigit(this->read))
		this->fileInput.get(this->read);

	if (this->read == '-')
	{
		isNegative = 1;
		this->fileInput.get(this->read); // skip the minus
	}

	while (isdigit(this->read))
	{
		digits.push_back((int)this->read - 48);
		this->fileInput.get(this->read);
	}

	int p = 1;
	for (int i = (int)digits.size() - 1; i >= 0; --i)
	{
		integer += digits[i] * p;
		p *= 10;
	}

	if (isNegative)
		integer *= -1;

	return integer;
}

Keys Deserializer::getNextKey()
{
	std::string key = "";

	while (this->read != '"') // reach 1st quotation mark
		this->fileInput.get(this->read);
	this->fileInput.get(this->read);

	while (this->read != '"')
	{
		key += this->read;
		this->fileInput.get(this->read);
	}
	this->fileInput.get(this->read); // skip 2nd quotation mark

	return EnumConverter::toKey(key);
}

std::vector<int> Deserializer::parseIntArray()
{
	this->read = 0;
	std::vector<int> arr;

	while (this->read != '[')
		this->fileInput.get(this->read);

	while (this->read != ']')
	{
		this->fileInput.get(this->read);

		if (this->read == '-' or isdigit(this->read))
			arr.push_back(this->getNextInteger());
	}

	return arr;
}

std::vector<Criterion> Deserializer::parseCriteria()
{
	this->read = 0;
	std::vector<Criterion> criteria;
	int nestingIndex = 1;

	while (this->read != '[')
		this->fileInput.get(this->read);

	while (nestingIndex > 0)
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::COMPARED_ENTRY:
		{
			criteria.push_back(Criterion());
			criteria.back().comparedEntry = this->getNextInteger();
			break;
		}
		case Keys::COMPARE_VALUE:
		{
			criteria.back().compareValue = this->getNextInteger();
			break;
		}
		case Keys::COMPARISON_OPERATOR:
		{
			criteria.back().comparisonOperator = this->getNextKey();
			break;
		}
		default: std::cout << "Invalid key in criteria!\n";
		}
	}

	return criteria;
}

std::vector<Modification> Deserializer::parseModifications()
{
	this->read = 0;
	std::vector<Modification> modifications;
	int nestingIndex = 1;

	while (this->read != '[')
		this->fileInput.get(this->read);

	while (nestingIndex > 0)
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::MODIFIED_ENTRY:
		{
			modifications.push_back(Modification());
			modifications.back().modifiedEntry = this->getNextInteger();
			break;
		}
		case Keys::MODIFICATION_OPERATOR:
		{
			modifications.back().modificationOperator = this->getNextKey();
			break;
		}
		case Keys::MODIFY_WITH_VALUE:
		{
			modifications.back().modifyWithValue = this->getNextInteger();
			break;
		}
		default: std::cout << "Invalid key at modifications!\n";
		}
	}

	return modifications;
}

std::unordered_map<int, Event> Deserializer::parseEvents()
{
	this->read = 0;
	std::unordered_map<int, Event> events;
	int currentEventID = 0;
	int nestingIndex = 1;

	while (this->read != '[')
	{
		this->fileInput.get(this->read);
	}

	while (nestingIndex > 0) // events don't have any further nesting atm but this is more expandable
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if(this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::ID:
		{
			currentEventID = this->getNextInteger();
			events.insert({ currentEventID, Event() });
			events[currentEventID].id = currentEventID;
			break;
		}
		case Keys::KEY:
		{
			events[currentEventID].key = this->getNextString();
			break;
		}
		case Keys::VALUE:
		{
			events[currentEventID].value = this->getNextInteger();
			break;
		}
		default: std::cout << "Invalid key at event!\n";
		}
	}

	return events;
}

std::unordered_map<int, Fact> Deserializer::parseFacts()
{
	this->read = 0;
	std::unordered_map<int, Fact> facts;
	int currentFactID = 0;
	int nestingIndex = 1;

	while (this->read != '[')
	{
		this->fileInput.get(this->read);
	}

	while (nestingIndex > 0)
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::ID: // start of new fact
		{
			currentFactID = this->getNextInteger();
			facts.insert({ currentFactID, Fact() });
			facts[currentFactID].id = currentFactID;
			break;
		}
		case Keys::KEY:
		{
			facts[currentFactID].key = this->getNextString();
			break;
		}
		case Keys::FACT_DATA:
		{
			facts[currentFactID].value = this->getNextInteger();
			break;
		}
		case Keys::VALUE:
		{
			facts[currentFactID].value = this->getNextInteger();
			break;
		}
		default: std::cout << "Invalid key at facts!\n";
		}
	}

	return facts;
}

std::unordered_map<int, Rule> Deserializer::parseRules()
{
	this->read = 0;
	std::unordered_map<int, Rule> rules;
	int currentRuleID = 0;
	int nestingIndex = 1;

	while (this->read != '[')
	{
		this->fileInput.get(this->read);
	}

	while (nestingIndex > 0)
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::ID: // start of new rule
		{
			currentRuleID = this->getNextInteger();
			rules.insert({ currentRuleID, Rule() });
			rules[currentRuleID].id = currentRuleID;
			break;
		}
		case Keys::KEY:
		{
			rules[currentRuleID].key = this->getNextString();
			break;
		}
		case Keys::RULE_TRIGGERED_BY:
		{
			rules[currentRuleID].triggeredBy = this->parseIntArray();
			break;
		}
		case Keys::RULE_TRIGGERS:
		{
			rules[currentRuleID].triggers = this->parseIntArray();
			break;
		}
		case Keys::RULE_CRITERIA: 
		{
			rules[currentRuleID].criteria = this->parseCriteria();
			break;
		}
		case Keys::RULE_MODIFICATIONS:
		{
			rules[currentRuleID].modifications = this->parseModifications();
			break;
		}
		case Keys::VALUE:
		{
			rules[currentRuleID].value = this->getNextInteger();
			break;
		}
		default: std::cout << "Invalid key at rule!\n";
		}
	}

	return rules;
}

std::unordered_map<int, Table> Deserializer::parseTables()
{
	this->read = 0;
	std::unordered_map<int, Table> tables;
	int currentTableID = 0;
	int nestingIndex = 1;

	while (this->read != '[')
	{
		this->fileInput.get(this->read);
	}

	while (nestingIndex > 0)
	{
		this->fileInput.get(this->read);

		if (this->read == '{' or this->read == '[')
		{
			++nestingIndex;
			continue;
		}
		else if (this->read == '}' or this->read == ']')
		{
			--nestingIndex;
			continue;
		}
		else if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::ID:
		{
			currentTableID = this->getNextInteger();
			tables.insert({ currentTableID, Table() });
			tables[currentTableID].id = currentTableID;
			break;
		}
		case Keys::KEY:
		{
			tables[currentTableID].key = this->getNextString();
			break;
		}
		case Keys::EVENTS:
		{
			tables[currentTableID].events = this->parseEvents();
			break;
		}
		case Keys::FACTS:
		{
			tables[currentTableID].facts = this->parseFacts();
			break;
		}
		case Keys::RULES:
		{
			tables[currentTableID].rules = this->parseRules();
			break;
		}
		default: std::cout << "Invalid key at table!\n";
		}
	}

	return tables;
}

Project Deserializer::parseProject()
{
	
	Project project;

	while (this->fileInput.get(this->read))
	{
		if (this->read != '"')
			continue;

		Keys keyToken = this->getNextKey();

		switch (keyToken)
		{
		case Keys::PROJECT_NAME:
		{
			project.name = this->getNextString(); 
			break;
		}
		case Keys::PROJECT_DESCRIPTION:
		{
			project.description = this->getNextString(); 
			break;
		}
		case Keys::PROJECT_CREATEDAT:
		{
			project.createdAtNano = this->getNextInteger(); 
			break;
		}
		case Keys::TABLES: 
		{
			project.tables = this->parseTables();
			break;
		}
		default: std::cout << "Invalid key in project!\n";
		}
	}

	return project;
}

// SERIALIZER

Serializer::Serializer(std::string filePath)
{
	this->fileOutput.open(filePath, std::ios::out);
}

Serializer::~Serializer()
{
	this->fileOutput.close();
}

void Serializer::format()
{
	for (int i = 1; i <= globalNestingLevel; ++i)
		this->fileOutput << "\t";
}

void Serializer::startObject()
{
	++this->globalNestingLevel;
	this->format(); this->fileOutput << "{\n";
	++this->globalNestingLevel;
}

void Serializer::endObject(bool isLast)
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

void Serializer::writeIntArray(std::vector<int> arr)
{
	int intCount = (int)arr.size(), intIndex = 0;

	for (auto i : arr)
	{
		++intIndex;

		if(intIndex == intCount)
			this->fileOutput << " " << i << " ";
		else
			this->fileOutput << " " << i << ",";
	}
}

void Serializer::writeCriteria(std::vector<Criterion> criteria)
{
	int objCount = (int)criteria.size(), objIndex = 0;

	for (auto i : criteria)
	{
		++objIndex;
		this->startObject();
		
		this->format(); this->fileOutput << "\"comparedEntry\": " << i.comparedEntry << ",\n";
		this->format(); this->fileOutput << "\"compareValue\": " << i.compareValue << ",\n";
		this->format(); this->fileOutput << "\"comparisonOperator\": \"" << EnumConverter::toString(i.comparisonOperator) << "\"\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeModifications(std::vector<Modification> modifications)
{
	int objCount = (int)modifications.size(), objIndex = 0;

	for (auto i : modifications)
	{
		++objIndex;
		this->startObject();

		this->format(); this->fileOutput << "\"modifiedEntry\": " << i.modifiedEntry << ",\n";
		this->format(); this->fileOutput << "\"modificationOperator\": \"" << EnumConverter::toString(i.modificationOperator) << "\",\n";
		this->format(); this->fileOutput << "\"modifyWithValue\": " << i.modifyWithValue << "\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeEvents(std::unordered_map<int, Event> events)
{
	int objCount = (int)events.size(), objIndex = 0;

	for (auto event : events)
	{
		++objIndex;
		this->startObject();

		this->format(); this->fileOutput << "\"id\": " << event.second.id << ",\n";
		this->format(); this->fileOutput << "\"key\": \"" << event.second.key << "\",\n";
		this->format(); this->fileOutput << "\"value\": " << event.second.value << "\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeFacts(std::unordered_map<int, Fact> facts)
{
	int objCount = (int)facts.size(), objIndex = 0;

	for (auto fact : facts)
	{
		++objIndex;
		this->startObject();

		this->format(); this->fileOutput << "\"id\": " << fact.second.id << ",\n";
		this->format(); this->fileOutput << "\"key\": \"" << fact.second.key << "\",\n";
		this->format(); this->fileOutput << "\"value\": " << fact.second.value << "\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeRules(std::unordered_map<int, Rule> rules)
{
	int objCount = (int)rules.size(), objIndex = 0;

	for (auto rule : rules)
	{
		++objIndex;
		this->startObject();
		
		this->format(); this->fileOutput << "\"id\": " << rule.second.id << ",\n";
		this->format(); this->fileOutput << "\"key\": \"" << rule.second.key << "\",\n";

		this->format(); this->fileOutput << "\"ruleTriggeredBy\": [";
		writeIntArray(rule.second.triggeredBy);
		this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"ruleTriggers\": [";
		writeIntArray(rule.second.triggers);
		this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"ruleCriteria\": [\n";
		this->writeCriteria(rule.second.criteria);
		this->format(); this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"ruleModifications\": [\n";
		this->writeModifications(rule.second.modifications);
		this->format(); this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"value\": " << rule.second.value << "\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeTables(std::unordered_map<int, Table> tables)
{
	int objCount = (int)tables.size(), objIndex = 0;

	for (auto table : tables)
	{
		++objIndex;
		this->startObject();

		this->format(); this->fileOutput << "\"id\": " << table.second.id << ",\n";
		this->format(); this->fileOutput << "\"key\": \"" << table.second.key << "\",\n";

		this->format(); this->fileOutput << "\"events\": [\n";
		this->writeEvents(table.second.events);
		this->format(); this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"facts\": [\n";
		this->writeFacts(table.second.facts);
		this->format(); this->fileOutput << "],\n";

		this->format(); this->fileOutput << "\"rules\": [\n";
		this->writeRules(table.second.rules);
		this->format(); this->fileOutput << "]\n";

		this->endObject(objIndex == objCount);
	}
}

void Serializer::writeProject(Project project)
{
	this->fileOutput << "{\n";
	++this->globalNestingLevel;

	this->format(); this->fileOutput << "\"projectName\": \"" << project.name << "\",\n";
	this->format(); this->fileOutput << "\"projectDescription\": \"" << project.description << "\",\n";
	this->format(); this->fileOutput << "\"projectCreatedAt\": " << project.createdAtNano << ",\n";

	this->format(); this->fileOutput << "\"tables\": [\n";
	this->writeTables(project.tables);
	this->format(); this->fileOutput << "]\n";

	this->fileOutput << "}\n";
}