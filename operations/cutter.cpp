#include<cmath>
#include<string>

#include"register.hpp"

namespace operations
{

struct Cutter
{
	inline static Operations::Defined<Cutter> info;
	std::string symbol = "[]", name = "cutter";
	int arity = 2;
	
	static Value eval(const Operations::Definition::ArgumentList &arguments)
	{
		auto index = arguments[0].get<Value::Number>();
		if(auto i = arguments[1].get<Value::Number>()){
			index = i;
		}

		auto string = arguments[0].get<Value::String>();
		if(auto s = arguments[1].get<Value::String>()){
			string = s;
		}

		if(!index || !string){
			return {};
		}

		if(string->size() < *index+1 || *index < 0 || std::floor(*index)!=*index || std::isnan(*index)){
			return "";
		}

		return Value::String(1,(*string)[*index]);
	}
};

}