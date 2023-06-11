#include "inkwell.h"

int main()
{
	std::shared_ptr<inkwell::Project> project;
	inkwell::Deserializer parser("sample.json");

	project = parser.parseProject();

	project->tables[1]->rules[5]->criteria[0]->compareValue = 39; // this is horrible, but it works for now
	project->tables[1]->events[1]->value++;

	inkwell::Serializer serializer("sample.json");
	serializer.writeProject(project);

	return 0;
}