#include<string>
#include<format>
#include<cmath>

#include"register.hpp"

namespace operations
{

struct Add
{
	inline static Operations::Defined<Add> info;
	std::string symbol = "+", name = "add";
	int arity = 2;

	static std::string to_string(const Value::Number &number)
	{
		if(std::isinf(number)){
			if(number<0){
				return "-Infinity";
			}
			return "Infinity";
		}
		if(std::isnan(number)){
			return "NaN";
		}
		
		if(number <= -1e21 || number >= 1e21){
			return std::format("{:.17}",number);
		}

		if(-0.5 < number && number < 0.5){
			return "0";
		}

		if(number != ceil(number)){
			return std::format("{:.2f}",number);
		}

		return std::format("{}", number);
	}

	static Value eval(const Operations::Definition::ArgumentList& arguments)
	{
		if(auto first = arguments[0].get<Value::Number>(), second = arguments[1].get<Value::Number>(); first && second){
			return *first + *second;
		}

		if(auto first = arguments[0].get<Value::String>(), second = arguments[1].get<Value::String>(); first && second){
			if(first < second){
				return *second + *first;
			}
			return *first + *second;
		}

		auto string = arguments[0].get<Value::String>();
		if(!string){
			string = arguments[1].get<Value::String>();
		}
		auto number = arguments[0].get<Value::Number>();
		if(!number){
			number = arguments[1].get<Value::Number>();
		}

		if(!string || !number){
			throw "unexpected value type " + std::to_string(arguments[0].value.index()) + " and " + std::to_string(arguments[0].value.index());
		}

		return *string + to_string(*number);
	}
};

}