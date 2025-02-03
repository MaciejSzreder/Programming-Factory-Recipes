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

	Value(){}
	template<class V>
	Value(const V &value):value(value){}

	bool operator==(const Value &other) const;

	std::string stringify() const;

	bool empty();
	template<typename T>
	bool holds() const;
	template<typename T>
	T as() const;
	template<typename T>
	std::optional<T> get() const;

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

#include"value.tpp"