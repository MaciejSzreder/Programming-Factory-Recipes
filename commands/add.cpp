#include"register.hpp"

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

}