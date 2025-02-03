#include"value.hpp"

template<typename T>
bool Value::holds() const
{
	return std::holds_alternative<T>(value);
}

template<typename T>
T Value::as() const
{
	return std::get<T>(value);
}

template<typename T>
std::optional<T> Value::get() const
{
	if(auto v = std::get_if<T>(&value)){
		return *v;
	}
	return {};
}