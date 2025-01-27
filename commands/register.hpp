#pragma once

#include<optional>
#include<string>
#include<variant>

#include"../register.hpp"
#include"../searcher.hpp"

namespace commands
{

struct Commands:Register<Commands>
{
	static std::optional<Commands::Definition> find(std::string name);
};

}

template<>
struct commands::Commands::Definition
{
	using Argument = std::variant<Value,std::string>;
	using ArgumentList = std::vector<Argument>;
	using function = std::function<void (Searcher &,const ArgumentList&)>;
	int id;
	std::string name;
	function execute;

	Definition(auto definition):
		name(definition.name),
		execute(definition.execute)
	{}
};

std::optional<commands::Commands::Definition> commands::Commands::find(std::string name)
{
	return Register<commands::Commands>::find(name, &commands::Commands::Definition::name);
}