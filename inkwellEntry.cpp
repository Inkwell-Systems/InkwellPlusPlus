#include "inkwellEntry.h"

using namespace inkwell;

void Event::setTriggers(std::vector<RuleRef> triggers, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->triggers = triggers;
	else if (operation == ArrayOperator::ADD)
		this->triggers.insert(this->triggers.end(), triggers.begin(), triggers.end());
	else
		std::cout << "error at Event::setTriggers\n";
}

void Rule::setTriggers(std::vector<EntryRef> triggers, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->triggers = triggers;
	else if (operation == ArrayOperator::ADD)
		this->triggers.insert(this->triggers.end(), triggers.begin(), triggers.end());
	else
		std::cout << "error at Rule::setTriggers\n";
}

void Rule::setCriteria(std::vector<CriterionRef> criteria, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->criteria = criteria;
	else if (operation == ArrayOperator::ADD)
		this->criteria.insert(this->criteria.end(), criteria.begin(), criteria.end());
	else
		std::cout << "error at Rule::setCriteria\n";
}

void Rule::setModifications(std::vector<ModificationRef> modifications, ArrayOperator operation)
{
	if (operation == ArrayOperator::SET)
		this->modifications = modifications;
	else if (operation == ArrayOperator::ADD)
		this->modifications.insert(this->modifications.end(), modifications.begin(), modifications.end());
	else
		std::cout << "error at Rule::setModifications\n";
}

void Fact::trigger()
{
	throw std::exception("Facts cannot be triggered\n");
}

void Event::trigger()
{
	for (RuleRef i : this->triggers)
	{
		i->trigger();
	}
}

void Rule::trigger()
{
	for (CriterionRef i : this->criteria) // Checks all criteria
	{
		if (!i->check())
			return;
	}

	for (ModificationRef i : this->modifications) // Does all modifications
	{
		i->modify();
	}

	for (auto& i : this->callbacks) // Dispatches all callbacks
	{
		i();
	}

	for (EntryRef i : this->triggers) // Triggers all entries
	{
		i->trigger();
	}

	++this->value;
}

bool Criterion::check()
{
	switch (this->comparisonOperator)
	{
	case Keys::COMPARISON_OPERATOR_EQUAL: return (this->comparedEntry->value == this->compareValue);
	case Keys::COMPARISON_OPERATOR_NOT_EQUAL: return (this->comparedEntry->value != this->compareValue);
	case Keys::COMPARISON_OPERATOR_GREATER_THAN: return (this->comparedEntry->value > this->compareValue);
	case Keys::COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL: return (this->comparedEntry->value >= this->compareValue);
	case Keys::COMPARISON_OPERATOR_LESS_THAN: return (this->comparedEntry->value < this->compareValue);
	case Keys::COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL: return (this->comparedEntry->value <= this->compareValue);
	default: std::cout << "error at Criterion::check\n";
	}
	return 0;
}

void Modification::modify()
{
	switch (this->modificationOperator)
	{
	case Keys::MODIFICATION_OPERATOR_SET: this->modifiedEntry->value = this->modifyWithValue; break;
	case Keys::MODIFICATION_OPERATOR_ADD: this->modifiedEntry->value += this->modifyWithValue; break;
	default: std::cout << "error at Modification::modify\n";
	}
}
