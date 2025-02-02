#pragma once

#include<vector>

#include"parsed.hpp"
#include"parser.hpp"

namespace parser
{

template<class _Parser>
struct Alternatives: Parser<Alternatives<_Parser>>
{
	std::vector<_Parser> expressions;

	Alternatives(auto begin, auto end): expressions(begin,end){}

	Parsed consume(auto &begin, auto end)
	{
		auto backup = begin;
		for(auto &expression:expressions){
			if(auto data = expression.consume(begin,end)){
				return data;
			}
			begin = backup;
		}
		return {};
	}
};

}