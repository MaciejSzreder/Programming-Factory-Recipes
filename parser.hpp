#pragma once

#include<string>

namespace parser
{

struct Keyword
{
	std::string keyword;

	bool consume(auto &begin, auto end)
	{
		for(const auto & character: keyword){
			if(begin == end || *begin!=character){
				return false;
			}
			++begin;
		}
		return begin == end || !std::isalnum(*begin);
	}
	

	bool parse(auto begin, auto end)
	{
		return consume(begin,end);
	}

	bool parse(const auto &string)
	{
		return parse(std::begin(string),std::end(string));
	}
};

namespace literals
{

Keyword operator""_k(const char *keyword,std::size_t size)
{
	return Keyword(std::string(keyword,size));
}

} // literals

} // parser