#pragma once

#include<vector>
#include<string>
#include<iostream>

#include"searcher.hpp"
#include"register.hpp"

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

namespace commands
{

struct Add
{
	inline static Commands::Defined<Add> definition;
	std::string name = "add";

	static void execute(Searcher &searcher, const Commands::Definition::ArgumentList &arguments)
	{
		for(const auto &argument: arguments){
			if(auto identifier = std::get_if<std::string>(&argument)){
				if(auto operation = Operations::find(*identifier)){
					searcher.add(*operation);
				}else{
					searcher.add(Value(*identifier));
				}
			}else{
				searcher.add(std::get<Value>(argument));
			}
		}
	}
};

struct Find
{
	inline static Commands::Defined<Find> definition;
	std::string name = "find";
	
	static void execute(Searcher &searcher, const Commands::Definition::ArgumentList &arguments)
	{
		Value argument;
		if(auto identifier = std::get_if<std::string>(&arguments[0])){
			argument = *identifier;
		}else{
			argument = std::get<Value>(arguments[0]);
		}
		std::cout << searcher.find(argument).getShortRecipe() << '\n';
	}
};

}