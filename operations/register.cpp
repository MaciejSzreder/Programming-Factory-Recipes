#include"register.hpp"

std::optional<operations::Operations::Definition> operations::Operations::find(std::string name)
{
	return Register<Operations>::find(name, &Register<Operations>::Definition::name);
}