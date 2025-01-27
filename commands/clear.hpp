#pragma once

#include"register.hpp"

namespace commands
{

struct Clear
{
	inline static Commands::Defined<Clear> definition;
	std::string name = "clear";
	
	static void execute(Searcher &searcher, const Commands::Definition::ArgumentList &arguments)
	{
		searcher.clear();
	}
};

}