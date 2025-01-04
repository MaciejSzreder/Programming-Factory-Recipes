#pragma once

#include<cmath>
#include<functional>
#include<vector>
#include<optional>

#include"value.hpp"


struct Operations
{
	struct Operation
	{
		using ArgumentList = std::vector<Value>;
		using function = std::function<Value(const ArgumentList&)>;
		int id;
		std::string name,symbol;
		int arity;
		function eval;
	};

	inline static std::vector<Operation> operations;

	static int size()
	{
		return operations.size();
	}

	static Operation operation(int id)
	{
		return operations[id];
	}

	static std::optional<Operation> operation(const std::string& name)
	{
		auto operation = std::ranges::find(operations,name,&Operation::name);
		if(operation == operations.end()){
			return {};
		}
		return *operation;
	}

	static Operation define(Operation operation)
	{
		operation.id = (int)operations.size();
		operations.push_back(operation);
		return operations.back();
	}
	static Operation define(auto obj)
	{
		return define({
			.name = obj.name,
			.symbol = obj.symbol,
			.arity = obj.arity,
			.eval = decltype(obj)::eval
		});
	}
	template<class O>
	static Operation define()
	{
		return define(O());
	}

	template<class O>
	struct Defined:Operation
	{
		Defined():Operation(define<O>()){}
	};
};

using OperationList = std::vector<Operations::Operation>;

struct Add
{
	inline static Operations::Defined<Add> info;
	std::string symbol = "+", name = "add";
	int arity = 2;

	static Value eval(const Operations::Operation::ArgumentList& arguments)
	{
		const Value &first = arguments[0], &second = arguments[1];
		{
			const float *f, *s;
			if((f = std::get_if<float>(&first.value)) && (s = std::get_if<float>(&second.value))){
				return Value{.value = *f+*s};
			}
		}
		{
			const std::string *f,*s;
			if((f = std::get_if<std::string>(&first.value)) && (s = std::get_if<std::string>(&second.value))){
				return Value{.value = *f+*s};
			}
		}
		{
			const std::string *text;
			const float *number;
			if((text = std::get_if<std::string>(&first.value)) && (number = std::get_if<float>(&second.value))){
				return Value{.value = *text+std::to_string(*number)};
			}
			if((number = std::get_if<float>(&first.value)) && (text = std::get_if<std::string>(&second.value))){
				return Value{.value = *text+std::to_string(*number)};
			}
		}
		throw "unexpected value type " + std::to_string(first.value.index()) + " and " + std::to_string(second.value.index());
	}
};

struct Remainder
{
	inline static Operations::Defined<Remainder> info;
	std::string symbol = "%", name = "reminder";
	int arity = 2;

	static Value eval(const Operations::Operation::ArgumentList& arguments)
	{
		const float *f, *s;
		if((f = std::get_if<float>(&arguments[0].value)) && (s = std::get_if<float>(&arguments[1].value))){
			if(*f>*s){
				std::swap(f,s);
			}
			return Value{.value = std::fmod(*s,*f)};
		}
		return {};
	}
};

struct Square
{
	inline static Operations::Defined<Square> info;
	std::string symbol = "■", name = "square";
	int arity = 1;
	
	static Value eval(const Operations::Operation::ArgumentList &arguments)
	{
		const float *f;
		if(f = std::get_if<float>(&arguments[0].value)){
			return Value{.value = *f * *f};
		}
		return {};
	}
};

struct Cutter
{
	inline static Operations::Defined<Cutter> info;
	std::string symbol = "[]", name = "cutter";
	int arity = 2;
	
	static Value eval(const Operations::Operation::ArgumentList &arguments)
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

		if(string->size()-1 < *index || *index < 0 || std::floor(*index)!=*index || std::isnan(*index)){
			return {.value = ""};
		}

		return {.value = Value::String(1,(*string)[*index])};
	}
};