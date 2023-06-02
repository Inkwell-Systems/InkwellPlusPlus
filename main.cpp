#include "inkwell.h"

int main()
{
	inkwell::Project project;
	inkwell::JSONParser parser("sample.json");

	project = parser.parseProject();

	return 0;
}