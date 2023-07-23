#pragma once

#include <string>

namespace inkwell
{
	class Error
	{
	private:
		static void throwException(std::string message)
		{
			std::string exceptionMessage = "[Inkwell Error] " + message;
			throw std::exception(exceptionMessage.c_str());
		}

		Error() {};

	public:
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