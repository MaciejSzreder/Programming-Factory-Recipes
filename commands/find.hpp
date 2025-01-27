#pragma once

#include<iostream>

#include"register.hpp"

namespace commands
{

struct Find
{
	inline static Commands::Defined<Find> definition;
	std::string name = "find";
	
	static void execute(Searcher &searcher, const Commands::Definition::ArgumentList &arguments)
	{
		for(const auto &argument: arguments){
			Value value;
			if(auto identifier = std::get_if<std::string>(&argument)){
				value = *identifier;
			}else{
				value = std::get<Value>(argument);
			}
			std::cout << searcher.find(value).getShortRecipe() << '\n';
		}
	}
};

}