#pragma once

#include<variant>
#include<string>
#include<optional>

struct Value
{
	using None = std::monostate;
	using String = std::string;
	using Number = float;
	std::variant<None, Number, String> value;

	template<class V>
	Value(const V &value):value(value){}

	Value(){}

	bool operator==(const Value &other) const;

	std::string stringify() const;

	bool empty();

	template<typename T>
	bool holds() const
	{
		return std::holds_alternative<T>(value);
	}

	template<typename T>
	T as() const
	{
		return std::get<T>(value);
	}

	template<typename T>
	std::optional<T> get() const
	{
		if(auto v = std::get_if<T>(&value)){
			return *v;
		}
		return {};
	}

	struct Order
	{
		bool bit_orderer(Number f,Number s);
		bool operator()(const Value &first, const Value &second);
	};

	struct Equality
	{
		
		bool bit_equality(Number f,Number s);
		bool operator()(const Value &first, const Value &second);
	};
};