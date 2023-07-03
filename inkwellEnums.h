#pragma once

#include <string>

namespace inkwell
{
	enum class ArrayOperator
	{
		NULL_OPERATOR,
		SET,
		ADD
	};

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
		MODIFICATION_OPERATOR_ADD,
		MODIFY_WITH_VALUE
	};

	class EnumConverter
	{
	public:
		static Keys toKey(std::string key);
		static ArrayOperator toArrayOperator(std::string key);
		static std::string toString(Keys k);
		static std::string toString(ArrayOperator a);
	};
}
