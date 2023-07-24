#pragma once

#include <string>
#include <format>
#include "inkwellErrors.hpp"

namespace inkwell
{
	class EnumConverter
	{
	private:
		enum class Keys
		{
			NULL_KEY,
			COMPARISON_OPERATOR_EQUAL,
			COMPARISON_OPERATOR_NOT_EQUAL,
			COMPARISON_OPERATOR_GREATER_THAN,
			COMPARISON_OPERATOR_LESS_THAN,
			COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL,
			COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL,
			MODIFICATION_OPERATOR_SET,
			MODIFICATION_OPERATOR_INCREMENT
		};

		static Keys toKey(std::string key)
		{
			if (key == "equal")
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
			else if (key == "set")
				return Keys::MODIFICATION_OPERATOR_SET;
			else if (key == "increment")
				return Keys::MODIFICATION_OPERATOR_INCREMENT;

			Error::throwException(
				std::format(
					"String \"{}\" is not recognized by the Enum Converter!\n", 
					key
				)
			);

			return Keys::NULL_KEY;
		}

		static std::string toString(Keys k)
		{
			if (k == Keys::COMPARISON_OPERATOR_EQUAL)
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
				return "modificationOperator";
			else if (k == Keys::MODIFICATION_OPERATOR_SET)
				return "set";
			else if (k == Keys::MODIFICATION_OPERATOR_INCREMENT)
				return "increment";

			return "nullKey";
		}

	public:
		EnumConverter() {};

		friend class Project;
		friend class Table;
		friend class Entry;
		friend class Event;
		friend class Fact;
		friend class Rule;
		friend class Criterion;
		friend class Modification;
		friend class EnumConverter;
	};
}
