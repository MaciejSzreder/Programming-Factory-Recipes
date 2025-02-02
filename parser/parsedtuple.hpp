#pragma once

namespace parser
{

template<class First, class Second>
struct ParsedTuple
{
	std::tuple<First,Second> parsed;

	ParsedTuple(First first = {}, Second second = {}): parsed(first,second){}
	
	operator bool()
	{
		auto [first, second] = parsed;
		return first && second;
	}

	template<int index>
	auto get()
	{
		return std::get<index>(parsed);
	}
};

}