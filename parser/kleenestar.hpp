#pragma once

#include"parsed.hpp"
#include"parser.hpp"
#include"parsedlist.hpp"
#include"regularexpression.hpp"

namespace parser
{

template<class Expression>
struct KleeneStar: Parser<KleeneStar<Expression>>
{
	Expression arg;

	KleeneStar(Expression arg): arg(arg){}

	ParsedList consume(auto &begin, auto end)
	{
		ParsedList parsed;
		auto backup = begin;
		while( auto data = arg.consume(begin,end) ){
			parsed.append(data);
			backup = begin;
			RegularExpression("[ \t\n\r]*").consume(begin,end);
		}
		begin = backup;
		return parsed;
	}
};

auto operator*(auto arg)
{
	return KleeneStar(arg);
}

}