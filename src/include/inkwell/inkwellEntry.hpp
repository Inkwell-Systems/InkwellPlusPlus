#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <unordered_set>

#include "inkwellEnums.hpp"
#include "inkwellScope.hpp"

namespace inkwell
{
	class Entry
	{
	public:
		int id = NULL;
		std::string key = "";
		double value = 0;

		Scope scope;
		int guard = 0;

		virtual void trigger() {};

		bool operator==(const Entry& rhs) const
		{
			return this->key == rhs.key;
		}

		bool operator!=(const Entry& rhs) const
		{
			return this->key != rhs.key;
		}
	};

	class Criterion
	{
	public:
		Criterion() {}

		Criterion(int comparedEntryID, double compareValue, Keys comparisonOperator)
		{
			this->comparedEntryID = comparedEntryID;
			this->compareValue = compareValue;
			this->comparisonOperator = comparisonOperator;
		}

		Entry* comparedEntry = nullptr;
		int comparedEntryID = NULL;

		double compareValue = 0;

		Keys comparisonOperator = Keys::NULL_KEY;

		bool check()
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

		bool operator==(const Criterion& rhs) const
		{
			return
				this->comparedEntry == rhs.comparedEntry and
				this->compareValue == rhs.compareValue and
				this->comparisonOperator == rhs.comparisonOperator;
		}

		bool operator!=(const Criterion& rhs) const
		{
			return
				this->comparedEntry != rhs.comparedEntry or
				this->compareValue != rhs.compareValue or
				this->comparisonOperator != rhs.comparisonOperator;
		}
	};

	class Modification
	{
	public:
		Modification(){}

		Modification(int modifiedEntryID, Keys modificationOperator, double modifyWithValue)
		{
			this->modifiedEntryID = modifiedEntryID;
			this->modificationOperator = modificationOperator;
			this->modifyWithValue = modifyWithValue;
		}

		bool operator==(const Modification& rhs) const
		{
			return
				this->modifiedEntry == rhs.modifiedEntry and
				this->modificationOperator == rhs.modificationOperator and
				this->modifyWithValue == rhs.modifyWithValue;
		}

		bool operator!=(const Modification& rhs) const
		{
			return
				this->modifiedEntry != rhs.modifiedEntry or
				this->modificationOperator != rhs.modificationOperator or
				this->modifyWithValue != rhs.modifyWithValue;
		}

		Entry* modifiedEntry = nullptr;
		int modifiedEntryID = NULL;

		Keys modificationOperator = Keys::NULL_KEY;

		double modifyWithValue = 0;

		void modify()
		{
			switch (this->modificationOperator)
			{
			case Keys::MODIFICATION_OPERATOR_SET: this->modifiedEntry->value = this->modifyWithValue; break;
			case Keys::MODIFICATION_OPERATOR_INCREMENT: this->modifiedEntry->value += this->modifyWithValue; break;
			default: std::cout << "error at Modification::modify\n";
			}
		}
	};

	class Fact : public Entry
	{
	public:
		Fact(){}

		Fact(int id, std::string key, double value, int guard)
		{
			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
		}

		void trigger() override
		{
			throw std::exception("Facts cannot be triggered\n");
		}
	};

	class Rule : public Entry
	{
	public:
		Rule() {}

		Rule(int id, std::string key, double value, int guard)
		{
			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
		}

		std::unordered_set<Entry*> triggers;
		std::vector<Criterion> criteria;
		std::vector<Modification> modifications;
		std::vector<std::function<void()>> callbacks;

		void trigger() override
		{
			for (Criterion i : this->criteria)
			{
				if (!i.check())
					return;
			}

			for (Modification i : this->modifications)
			{
				i.modify();
			}

			for (auto& i : this->callbacks)
			{
				i();
			}

			for (auto& i : this->triggers)
			{
				i->trigger();
			}

			++this->value;
		}
	};

	class Event : public Entry
	{
	public:
		Event(){}

		Event(int id, std::string key, double value, int guard)
		{
			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
		}

		bool operator==(const Event& rhs) const
		{
			if(this->key != rhs.key)
				return false;

			if(this->triggers.size() != rhs.triggers.size())
				return false;

			for (auto& i : this->triggers)
			{
				if(rhs.triggers.find(i) == rhs.triggers.end())
					return false;
			}

			return true;
		}

		bool operator!=(const Event& rhs) const
		{
			if(this->key != rhs.key)
				return true;

			if (this->triggers.size() != rhs.triggers.size())
				return true;

			for (auto& i : this->triggers)
			{
				if (rhs.triggers.find(i) == rhs.triggers.end())
					return true;
			}

			return false;
		}

		std::unordered_set<Entry*> triggers;

		void trigger() override {
			for (auto& i : this->triggers)
			{
				i->trigger();
			}
		}
	};
}
