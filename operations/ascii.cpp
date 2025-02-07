#include<string>
#include<cmath>

#include"register.hpp"

namespace operations
{

struct Ascii
{
	inline static Operations::Defined<Ascii> info;
	std::string symbol = "ASCII", name = "ASCII";
	int arity = 1;
	
	static Value eval(const Operations::Definition::ArgumentList &arguments)
	{
		if(auto string = arguments[0].get<Value::String>()){
			if(string->size()<1){
				return std::nanf(nullptr);
			}
			return (Value::Number)(*string)[0];
		}
		if(auto number = arguments[0].get<Value::Number>()){
			char character = (char)*number;
			return Value::String(32 <= *number && *number <= 127,character);
		}

		throw "unexpected value type " + std::to_string(arguments[0].value.index());
	};
};

}