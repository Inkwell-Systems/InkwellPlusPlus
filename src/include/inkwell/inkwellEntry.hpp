#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <format>

#include "inkwellEnums.hpp"
#include "inkwellScope.hpp"
#include "inkwellErrors.hpp"

namespace inkwell
{
	class Entry
	{
	protected:
		bool initialized = false;

		int id = NULL;
		std::string key = "";
		int guard = 0;

	public:
		double value = 0;

		virtual bool trigger() = 0;

		int ID() const
		{
			return this->id;
		}

		std::string KEY() const
		{
			return this->key;
		}

		int GRD() const
		{
			return this->guard;
		}

		bool INIT() const
		{
			return this->initialized;
		}

		friend class Project;
		friend class Table;
	};

	class Criterion
	{
	private:
		bool initialized = false;

		std::shared_ptr<Entry> comparedEntry = nullptr;
		int comparedEntryID = NULL;
		double compareValue = 0;
		Keys comparisonOperator = Keys::NULL_KEY;

	public:
		Criterion() 
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Criterion (Compared Entry {}, Compare Value {}, Comparison Operator {}) has already been initialized!\n",
						this->comparedEntryID,
						this->compareValue,
						EnumConverter::toString(this->comparisonOperator)
					)
				);
			}

			this->initialized = true;
		}

		Criterion(int comparedEntryID, double compareValue, Keys comparisonOperator)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Criterion (Compared Entry {}, Compare Value {}, Comparison Operator {}) has already been initialized!\n",
						this->comparedEntryID,
						this->compareValue,
						EnumConverter::toString(this->comparisonOperator)
					)
				);
			}

			this->comparedEntryID = comparedEntryID;
			this->compareValue = compareValue;
			this->comparisonOperator = comparisonOperator;
			this->initialized = true;
		}

		bool check() const
		{
			switch (this->comparisonOperator)
			{
			case Keys::COMPARISON_OPERATOR_EQUAL: return (this->comparedEntry->value == this->compareValue);
			case Keys::COMPARISON_OPERATOR_NOT_EQUAL: return (this->comparedEntry->value != this->compareValue);
			case Keys::COMPARISON_OPERATOR_GREATER_THAN: return (this->comparedEntry->value > this->compareValue);
			case Keys::COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL: return (this->comparedEntry->value >= this->compareValue);
			case Keys::COMPARISON_OPERATOR_LESS_THAN: return (this->comparedEntry->value < this->compareValue);
			case Keys::COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL: return (this->comparedEntry->value <= this->compareValue);
			default: Error::throwException(
				std::format(
					"Invalid Comparison Operator \"{}\"!\n",
					EnumConverter::toString(this->comparisonOperator)
				)
			);
			}
			return 0;
		}

		bool INIT() const
		{
			return this->initialized;
		}

		friend class Project;
		friend class Table;
	};

	class Modification
	{
	private:
		bool initialized = false;

		std::shared_ptr<Entry> modifiedEntry = nullptr;
		int modifiedEntryID = NULL;
		Keys modificationOperator = Keys::NULL_KEY;
		double modifyWithValue = NULL;

	public:
		Modification()
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Modification (Modified Entry {}, Modify With Value {}, Modification Operator {}) has already been initialized!\n",
						this->modifiedEntryID,
						this->modifyWithValue,
						EnumConverter::toString(this->modificationOperator)
					)
				);
			}

			this->initialized = true;
		}

		Modification(int modifiedEntryID, Keys modificationOperator, double modifyWithValue)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Modification (Modified Entry {}, Modify With Value {}, Modification Operator {}) has already been initialized!\n",
						this->modifiedEntryID,
						this->modifyWithValue,
						EnumConverter::toString(this->modificationOperator)
					)
				);
			}

			this->modifiedEntryID = modifiedEntryID;
			this->modificationOperator = modificationOperator;
			this->modifyWithValue = modifyWithValue;
			this->initialized = true;
		}

		bool INIT() const
		{
			return this->initialized;
		}

		void modify()
		{
			switch (this->modificationOperator)
			{
			case Keys::MODIFICATION_OPERATOR_SET: this->modifiedEntry->value = this->modifyWithValue; break;
			case Keys::MODIFICATION_OPERATOR_INCREMENT: this->modifiedEntry->value += this->modifyWithValue; break;
			default: Error::throwException(
				std::format(
					"Invalid Modification Operator \"{}\"!\n",
					EnumConverter::toString(this->modificationOperator)
				)
			);
			}
		}

		friend class Project;
		friend class Table;
	};

	class Fact : public Entry
	{
	public:
		Fact()
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Fact (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->initialized = true;
		}

		Fact(int id, std::string key, double value, int guard)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Fact (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
			this->initialized = true;
		}

		bool trigger() override
		{
			Error::throwException("Facts cannot be triggered!\n");
			return false;
		}

		friend class Project;
		friend class Table;
	};

	class Rule : public Entry
	{
	private:
		std::unordered_set<std::shared_ptr<Entry>> triggers;
		std::vector<std::shared_ptr<Criterion>> criteria;
		std::vector<std::shared_ptr<Modification>> modifications;

	public:
		Rule() 
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Rule (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->initialized = true;
		}

		Rule(int id, std::string key, double value, int guard)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Rule (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
			this->initialized = true;
		}

		std::vector<std::function<void()>> callbacks;

		bool trigger() override
		{
			for (auto& i : this->criteria)
			{
				if (!i->check())
					return false;
			}

			for (auto& i : this->modifications)
			{
				i->modify();
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
			return true;
		}

		friend class Project;
		friend class Table;
	};

	class Event : public Entry
	{
	private:
		std::unordered_set<std::shared_ptr<Entry>> triggers;

	public:
		Event()
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Event (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->initialized = true;
		}

		Event(int id, std::string key, double value, int guard)
		{
			if (this->initialized)
			{
				Error::throwException(
					std::format(
						"This Event (ID {}, Key {}) has already been initialized!\n",
						this->id,
						this->key
					)
				);
			}

			this->id = id;
			this->key = key;
			this->value = value;
			this->guard = guard;
			this->initialized = true;
		}

		bool trigger() override {

			for (auto& i : this->triggers)
			{
				i->trigger();
			}

			return true;
		}

		friend class Project;
		friend class Table;
	};
}
