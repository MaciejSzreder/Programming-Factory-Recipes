#pragma once

#include"parser.hpp"
#include"parsedtuple.hpp"
#include"regularexpression.hpp"

namespace parser
{

template<class Expression1, class Expression2>
struct Sequence: Parser<Sequence<Expression1, Expression2>>
{

	Expression1 first;
	Expression2 second;

	Sequence(Expression1 first, Expression2 second): first(first), second(second){}

	auto consume(auto &begin, auto end)
	{
		using Parsed = decltype(ParsedTuple(first.consume(begin,end),second.consume(begin,end)));

		auto data1 = first.consume(begin,end);
		if(!data1){
			return Parsed{};
		}
		RegularExpression("[ \t\n\r]*").consume(begin,end);
		auto data2 = second.consume(begin,end);
		if(!data2){
			return Parsed{data1};
		}
		return Parsed{data1,data2};
	}
};

auto operator+(auto first, auto second)
{
	return Sequence(first,second);
}

}