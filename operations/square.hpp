#pragma once

#include<string>

#include"register.hpp"

namespace operations
{

struct Square
{
	inline static Operations::Defined<Square> info;
	std::string symbol = "■", name = "square";
	int arity = 1;
	
	static Value eval(const Operations::Definition::ArgumentList &arguments)
	{
		const Value::Number *f;
		if(f = std::get_if<Value::Number>(&arguments[0].value)){
			return *f * *f;
		}
		return {};
	}
};

}