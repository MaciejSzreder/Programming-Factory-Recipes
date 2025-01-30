#include<variant>
#include<string>
#include<optional>
#include<sstream>
#include<iomanip>
#include<format>
#include<cmath>

#include"value.hpp"

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

bool Value::operator==(const Value &other) const
{
	return value == other.value;
}

std::string Value::stringify() const
{
	return std::visit([](auto value){ return ::stringify(value);},value);
}

bool Value::empty()
{
	return std::holds_alternative<std::monostate>(value);
}

bool Value::Order::bit_orderer(Number f,Number s)
{
	using Integer = unsigned int;
	static_assert(sizeof(Integer) == sizeof(Number));
	return reinterpret_cast<Integer&>(f)<reinterpret_cast<Integer&>(s);
}

bool Value::Order::operator()(const Value &first, const Value &second)
{
	if(first.holds<Number>() && second.holds<Number>()){
		return bit_orderer(first.as<Number>(),second.as<Number>());
	}
	return first.value<second.value;
}

bool Value::Equality::bit_equality(Number f,Number s)
{
	using Integer = unsigned int;
	static_assert(sizeof(Integer) == sizeof(Number));
	return reinterpret_cast<Integer&>(f)==reinterpret_cast<Integer&>(s);
}

bool Value::Equality::operator()(const Value &first, const Value &second)
{
	if(first.holds<Number>() && second.holds<Number>()){
		return bit_equality(first.as<Number>(),second.as<Number>());
	}
	return first.value==second.value;
}