#pragma once

#include<variant>
#include<string>

namespace{
	static std::string stringify(std::nullptr_t)
	{
		return "no value";
	}

	static std::string stringify(std::string value)
	{
		return value;
	}

	template<typename T>
	static std::string stringify(T value)
	{
		return std::to_string(value);
	}
}

struct Value
{
	std::variant<std::nullptr_t, std::string, float> value;

	std::string stringify() const
	{
		return std::visit([](auto value){ return ::stringify(value);},value);
	}

	bool empty()
	{
		return std::holds_alternative<std::nullptr_t>(value);
	}
};
