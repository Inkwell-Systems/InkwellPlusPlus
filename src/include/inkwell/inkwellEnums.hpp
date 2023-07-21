#pragma once

#include <string>
#include <format>
#include "inkwellErrors.hpp"

namespace inkwell
{
	enum class Keys
	{
		NULL_KEY,
		PROJECT_NAME,
		PROJECT_DESCRIPTION,
		PROJECT_CREATEDAT,
		TABLES,
		ID,
		KEY,
		VALUE,
		EVENTS,
		FACTS,
		RULES,
		FACT_DATA,
		RULE_TRIGGERED_BY,
		RULE_TRIGGERS,
		RULE_CRITERIA,
		RULE_MODIFICATIONS,
		COMPARED_ENTRY,
		COMPARE_VALUE,
		COMPARISON_OPERATOR,
		COMPARISON_OPERATOR_EQUAL,
		COMPARISON_OPERATOR_NOT_EQUAL,
		COMPARISON_OPERATOR_GREATER_THAN,
		COMPARISON_OPERATOR_LESS_THAN,
		COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL,
		COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL,
		MODIFIED_ENTRY,
		MODIFICATION_OPERATOR,
		MODIFICATION_OPERATOR_SET,
		MODIFICATION_OPERATOR_INCREMENT,
		MODIFY_WITH_VALUE
	};

	class EnumConverter
	{
	public:
		static Keys toKey(std::string key)
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
			else if (k == Keys::MODIFIED_ENTRY)
				return "modifiedEntry";
			else if (k == Keys::MODIFICATION_OPERATOR)
				return "modificationOperator";
			else if (k == Keys::MODIFICATION_OPERATOR_SET)
				return "set";
			else if (k == Keys::MODIFICATION_OPERATOR_INCREMENT)
				return "increment";
			else if (k == Keys::MODIFY_WITH_VALUE)
				return "modifyWithValue";

			Error::throwException(
				"A Key is not recognized by the Enum Converter!\n"
			);

			return "nullKey";
		}
	};
}
