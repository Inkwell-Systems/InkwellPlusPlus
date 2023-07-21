#pragma once

#include <string>

namespace inkwell
{
	class Error
	{
	public:
		static void throwException(std::string message)
		{
			std::string exceptionMessage = "[Inkwell Error] " + message;
			throw std::exception(exceptionMessage.c_str());
		}
	};
}