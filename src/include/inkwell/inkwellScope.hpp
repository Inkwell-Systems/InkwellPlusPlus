#pragma once

#include <string>
#include <memory>

namespace inkwell
{
	class Scope
	{
	public:
		enum class ScopeKey
		{
			GLOBAL = 0,
			LOCAL = 1
		};

		ScopeKey key = ScopeKey::GLOBAL;
		std::string strKey = "global";
		int level = 0;
	};
}
