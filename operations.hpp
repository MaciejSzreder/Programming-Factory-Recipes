#pragma once

#include<cmath>
#include<functional>
#include<vector>
#include<optional>
#include<format>

#include"value.hpp"
#include"register.hpp"

struct Operations:Register<Operations>{
	static std::optional<Operations::Definition> find(std::string name);
};

template<>
struct Register<Operations>::Definition
{
	using ArgumentList = std::vector<Value>;
	using function = std::function<Value(const ArgumentList&)>;
	int id;
	std::string name,symbol;
	int arity;
	function eval;

	Definition(auto definition):
		name(definition.name),
		symbol(definition.symbol),
		arity(definition.arity),
		eval(definition.eval)
	{}
};

std::optional<Operations::Definition> Operations::find(std::string name)
{
	return Register<Operations>::find(name, &Register<Operations>::Definition::name);
}

using OperationList = std::vector<Operations::Definition>;

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

struct Remainder
{
	inline static Operations::Defined<Remainder> info;
	std::string symbol = "%", name = "reminder";
	int arity = 2;

	static Value eval(const Operations::Definition::ArgumentList& arguments)
	{
		const float *f, *s;
		if((f = std::get_if<float>(&arguments[0].value)) && (s = std::get_if<float>(&arguments[1].value))){
			if(*f>*s){
				std::swap(f,s);
			}
			return std::fmod(*s,*f);
		}
		return {};
	}
};

struct Square
{
	inline static Operations::Defined<Square> info;
	std::string symbol = "■", name = "square";
	int arity = 1;
	
	static Value eval(const Operations::Definition::ArgumentList &arguments)
	{
		const float *f;
		if(f = std::get_if<float>(&arguments[0].value)){
			return *f * *f;
		}
		return {};
	}
};

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
		return (float)(Integer)first->find(*second);
	}
};

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
			return (float)(*string)[0];
		}
		if(auto number = arguments[0].get<Value::Number>()){
			char character = (char)*number;
			return Value::String(character < 32 ? 0 : 1,character);
		}

		throw "unexpected value type " + std::to_string(arguments[0].value.index());
	};
};