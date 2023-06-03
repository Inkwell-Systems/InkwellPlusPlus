#include "inkwell.h"

int main()
{
	inkwell::Project project;
	inkwell::Deserializer parser("sample.json");

	project = parser.parseProject();

	inkwell::Serializer serializer("newfile.json");
	serializer.writeProject(project);

	return 0;
}