#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <variant>

#include "inkwellEnums.h"

#define EntryRef std::shared_ptr<Entry>
#define EventRef std::shared_ptr<Event>
#define FactRef std::shared_ptr<Fact>
#define RuleRef std::shared_ptr<Rule>
#define CriterionRef std::shared_ptr<Criterion>
#define ModificationRef std::shared_ptr<Modification>

namespace inkwell
{
	class Entry
	{
	public:
		int id = 0;
		std::string key = "";
		double value = 0;
		virtual void trigger() {};
	};

	class Criterion
	{
	public:
		EntryRef comparedEntry = nullptr;
		double compareValue = 0;
		Keys comparisonOperator = Keys::NULL_KEY;
		bool check();
	};

	class Modification
	{
	public:
		EntryRef modifiedEntry = nullptr;
		Keys modificationOperator = Keys::NULL_KEY;
		double modifyWithValue = 0;
		void modify();
	};


	class Fact : public Entry
	{
	public:
		void trigger() override;
	};

	class Rule : public Entry
	{
	public:
		std::vector<EntryRef> triggers;
		std::vector<CriterionRef> criteria;
		std::vector<ModificationRef> modifications;
		std::vector<std::function<void()>> callbacks;
		void setTriggers(std::vector<EntryRef> triggers, ArrayOperator operation);
		void setCriteria(std::vector<CriterionRef> criteria, ArrayOperator operation);
		void setModifications(std::vector<ModificationRef> modifications, ArrayOperator operation);
		void trigger() override;
	};

	class Event : public Entry
	{
	public:
		std::vector<RuleRef> triggers;
		void setTriggers(std::vector<RuleRef> triggers, ArrayOperator operation);
		void trigger() override;
	};
}
