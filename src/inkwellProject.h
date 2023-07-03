#pragma once

#include <string>

#include "inkwellTable.h"

#define ProjectRef std::shared_ptr<Project>
#define TableRef std::shared_ptr<Table>

namespace inkwell
{
	class Project
	{
	public:
		std::string name = "";
		std::string description = "";
		int createdAtNano = 0;
		std::unordered_map<std::string, TableRef> tables;
	};
}
