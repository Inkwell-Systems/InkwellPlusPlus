#pragma once

#include <string>
#include <memory>

namespace inkwell
{
	class Scope
	{
	private:
		enum class ScopeKey
		{
			GLOBAL = 0,
			LOCAL = 1
		};

		ScopeKey key = ScopeKey::GLOBAL;
		std::string strKey = "global";
		int level = 0;

	public:
		friend class Project;
		friend class Table;
		friend class Entry;
		friend class Event;
		friend class Fact;
		friend class Rule;
		friend class Criterion;
		friend class Modification;
	};
}
