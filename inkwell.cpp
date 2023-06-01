#include "inkwell.h"

using namespace inkwell;

void Rule::setTriggeredBy(std::vector<int> triggeredBy, modOp operation)
{
	if (operation == SET)
		this->triggeredBy = triggeredBy;
	else if (operation == ADD)
		this->triggeredBy.insert(this->triggeredBy.end(), triggeredBy.begin(), triggeredBy.end());
	else
		std::cout << "error at Rule::setTriggeredBy\n";
}

void Rule::setTriggers(std::vector<int> triggers, modOp operation)
{
	if (operation == SET)
		this->triggers = triggers;
	else if (operation == ADD)
		this->triggers.insert(this->triggers.end(), triggers.begin(), triggers.end());
	else
		std::cout << "error at Rule::setTriggers\n";
}

void Rule::setCriteria(std::vector<Criterion> criteria, modOp operation)
{
	if (operation == SET)
		this->criteria = criteria;
	else if (operation == ADD)
		this->criteria.insert(this->criteria.end(), criteria.begin(), criteria.end());
	else
		std::cout << "error at Rule::setCriteria\n";
}

void Rule::setModifications(std::vector<Modification> modifications, modOp operation)
{
	if (operation == SET)
		this->modifications = modifications;
	else if (operation == ADD)
		this->modifications.insert(this->modifications.end(), modifications.begin(), modifications.end());
	else
		std::cout << "error at Rule::setModifications\n";
}

void Table::setEvents(std::vector<Event> events, modOp operation)
{
	if (operation == SET)
		this->events.clear();

	for (Event i : events)
		this->events.insert({ i.id, i });

	if (operation != SET and operation != ADD)
		std::cout << "error at Table::setEvents\n";
}

void Table::setFacts(std::vector<Fact> facts, modOp operation)
{
	if (operation == SET)
		this->facts.clear();

	for (Fact i : facts)
		this->facts.insert({ i.id, i });

	if (operation != SET and operation != ADD)
		std::cout << "error at Table::setFacts\n";
}

void Table::setRules(std::vector<Rule> rules, modOp operation)
{
	if (operation == SET)
		this->rules.clear();

	for (Rule i : rules)
		this->rules.insert({ i.id, i });

	if (operation != SET and operation != ADD)
		std::cout << "error at Table::setRules\n";
}

JSONParser::JSONParser(const std::string filename)
{
	this->fileInput.open(filename);
}

std::string JSONParser::getNextString()
{
	char read = 0;
	std::string str = "";

	while (read != '"') // reach 1st quotation mark
		this->fileInput.get(read);
	this->fileInput.get(read); // skip 1st quotation mark lol

	while (read != '"') // shove chars in str until 2nd quotation mark
		str += read;

	return str;
}

int JSONParser::getNextInteger()
{
	char read = 0;
	int integer = 0;
	bool isNegative = 0;
	std::vector<int> digits;

	while (read != '-' and !isdigit(read))
		this->fileInput.get(read);

	if (read == '-')
	{
		isNegative = 1;
		this->fileInput.get(read); // skip the minus
	}

	while (isdigit(read))
	{
		digits.push_back((int)read - 48);
		this->fileInput.get(read);
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

Keys JSONParser::getNextKey()
{
	char read = 0;

	this->fileInput.get(read); // skip 1st quotation mark
	std::string key = "";
	while (read != '"') // read until 2nd quotation mark
	{
		key += read;
		this->fileInput.get(read);
	}

	if (key == "name")
		return NAME;
	else if (key == "description")
		return DESCRIPTION;
	else if (key == "createdAt")
		return CREATEDAT;
	else if (key == "tables")
		return TABLES;

	return NULL_KEYS;
}

std::vector<Table> JSONParser::parseTables()
{
	std::vector<Table> tables;
	return tables;
} // placeholder function

Project JSONParser::parseProject()
{
	char read = 0;
	Project project;

	while (this->fileInput.get(read))
	{
		if (read != '"')
			continue;

		Keys keyToken = getNextKey();

		switch (keyToken)
		{
		case NAME: project.name = getNextString(); break;
		case DESCRIPTION: project.description = getNextString(); break;
		case CREATEDAT: project.createdAtNano = getNextInteger(); break;
		case TABLES: project.tables = parseTables(); break;
		default: throw std::logic_error("Key seems to be invalid!\n");
		}
	}

	return project;
}