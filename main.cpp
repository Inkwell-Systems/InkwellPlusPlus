#include "inkwell.h"

int main()
{
	inkwell::Project project;
	inkwell::Deserializer parser("sample.json");

	project = parser.parseProject();

	project.tables[1].rules[5].criteria[0].compareValue = 39;

	inkwell::Serializer serializer("newfile.json");
	serializer.writeProject(project);

	return 0;
}