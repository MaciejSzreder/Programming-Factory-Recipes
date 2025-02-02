#pragma once

#include"parsed.hpp"
#include"parser.hpp"

namespace parser
{

template<class Expression1, class Expression2>
struct Alternative: Parser<Alternative<Expression1,Expression2>>
{
	Expression1 first;
	Expression2 second;

	Alternative(Expression1 first, Expression2 second): first(first),second(second){}

	Parsed consume(auto &begin, auto end)
	{
		auto backup = begin;
		auto data = first.consume(begin,end);
		if(data){
			return data;
		}
		begin = backup;
		return second.consume(begin,end);
	}
};

auto operator|(auto first, auto second)
{
	return Alternative(first,second);
}

template<class Expression1, class Expression2>
bool operator==(Parsed parsed, Alternative<Expression1,Expression2> alternative)
{
	return parsed == alternative.first || parsed == alternative.second;
}

}