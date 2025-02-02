#pragma once

#include<string>
#include<regex>

#include"parsed.hpp"
#include"parser.hpp"
#include"parserleaf.hpp"

namespace parser
{

struct RegularExpression: Parser<RegularExpression>, ParserLeaf
{
	std::string regex;

	RegularExpression(std::string regex): regex(regex){}

	Parsed consume(auto &begin, auto end)
	{
		std::smatch match;
		if(std::regex_search(begin,end,match,std::regex("^"+regex))){
			begin += match[0].second - match[0].first;
			return {match.str(),*this};
		}
		return {};
	}
};

namespace literals
{

RegularExpression operator""_re(const char *re, std::size_t size)
{
	return RegularExpression(std::string(re,size));
}

}

}