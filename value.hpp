#pragma once

#include<variant>
#include<string>
#include<optional>
#include<sstream>
#include<iomanip>
#include<format>
#include<cmath>

namespace{
	static std::string stringify(std::monostate)
	{
		return "no value";
	}

	static std::string stringify(std::string string)
	{
		return (std::ostringstream()<<std::quoted(string)).str();
	}

	static std::string stringify(float number)
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

		if(number != ceil(number)){
			return std::format("{:.0f}",number);
		}

		return std::format("{}", number);
	}
}

struct Value
{
	using None = std::monostate;
	using String = std::string;
	using Number = float;
	std::variant<None, Number, String> value;

	template<class V>
	Value(const V &value):value(value){}

	Value(){}

	bool operator==(const Value &other) const
	{
		return value == other.value;
	}

	std::string stringify() const
	{
		return std::visit([](auto value){ return ::stringify(value);},value);
	}

	bool empty()
	{
		return std::holds_alternative<std::monostate>(value);
	}

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
		bool bit_orderer(Number f,Number s)
		{
			using Integer = unsigned int;
			static_assert(sizeof(Integer) == sizeof(Number));
			return reinterpret_cast<Integer&>(f)<reinterpret_cast<Integer&>(s);
		}

		bool operator()(const Value &first, const Value &second)
		{
			if(first.holds<Number>() && second.holds<Number>()){
				return bit_orderer(first.as<Number>(),second.as<Number>());
			}
			return first.value<second.value;
		}
	};

	struct Equality
	{
		
		bool bit_equality(Number f,Number s)
		{
			using Integer = unsigned int;
			static_assert(sizeof(Integer) == sizeof(Number));
			return reinterpret_cast<Integer&>(f)==reinterpret_cast<Integer&>(s);
		}

		bool operator()(const Value &first, const Value &second)
		{
			if(first.holds<Number>() && second.holds<Number>()){
				return bit_equality(first.as<Number>(),second.as<Number>());
			}
			return first.value==second.value;
		}
	};
};
