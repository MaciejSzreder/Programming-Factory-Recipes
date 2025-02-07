#include<cmath>
#include<string>

#include"register.hpp"

namespace operations
{

struct Remainder
{
	inline static Operations::Defined<Remainder> info;
	std::string symbol = "%", name = "remainder";
	int arity = 2;

	static Value eval(const Operations::Definition::ArgumentList& arguments)
	{
		const Value::Number *f, *s;
		if((f = std::get_if<Value::Number>(&arguments[0].value)) && (s = std::get_if<Value::Number>(&arguments[1].value))){
			if(*f>*s){
				std::swap(f,s);
			}
			return std::fmod(*s,*f);
		}
		return {};
	}
};

}