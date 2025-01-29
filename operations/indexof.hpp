#pragma once

#include<string>

#include"register.hpp"

namespace operations
{

struct IndexOf
{
	inline static Operations::Defined<IndexOf> info;
	std::string symbol = "─O", name = "index";
	int arity = 2;
	
	static Value eval(const Operations::Definition::ArgumentList &arguments)
	{
		auto first = arguments[0].get<Value::String>();
		auto second = arguments[1].get<Value::String>();

		if(!first || !second){
			return {};
		}

		if(first->size()<second->size()){
			std::swap(first,second);
		}
		
		using Integer = long long;
		static_assert(sizeof(Integer)==sizeof(Value::String::size_type));
		return (Value::Number)(Integer)first->find(*second);
	}
};

}