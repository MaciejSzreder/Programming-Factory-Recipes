#pragma once

#include<cmath>

#include"value.hpp"

struct Operation
{
	enum{
		extracting,
		adding,
		remaindering
	} operation;

	std::string getSymbol()
	{
		switch (operation){
		case extracting:
			return "";
		case adding:
			return "+";
		case remaindering:
			return "%";
		
		default:
			throw "lack symbol for operation " + std::to_string(operation);
		}
	}

		
	static Value add(const Value &first, const Value &second)
	{
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

	static Value mod(const Value &first, const Value &second)
	{
		const float *f, *s;
		if((f = std::get_if<float>(&first.value)) && (s = std::get_if<float>(&second.value))){
			if(*f>*s){
				std::swap(f,s);
			}
			return Value{.value = std::fmod(*s,*f)};
		}
		return {};
	}
};