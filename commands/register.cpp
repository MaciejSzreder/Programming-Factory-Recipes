#include"register.hpp"

std::optional<commands::Commands::Definition> commands::Commands::find(std::string name)
{
	return Register<commands::Commands>::find(name, &commands::Commands::Definition::name);
}