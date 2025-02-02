#pragma once

#include<string>
#include<initializer_list>

#include"parsed.hpp"
#include"parser.hpp"
#include"parserleaf.hpp"
#include"alternatives.hpp"

namespace parser
{

struct Keyword: Parser<Keyword>, ParserLeaf
{
	std::string keyword;

	Keyword(std::string keyword): keyword(keyword){}

	template<class Collection = std::initializer_list<std::string>>
	static auto keywords(Collection collection)
	{
		return Alternatives<Keyword>(collection.begin(),collection.end());
	}

	Parsed consume(auto &begin, auto end)
	{
		for(const auto & character: keyword){
			if(begin == end || *begin!=character){
				return {};
			}
			++begin;
		}
		if( begin == end || !std::isalnum(*begin)){
			return {keyword,*this};
		}
		return {};
	}
};

namespace literals
{

Keyword operator""_k(const char *keyword,std::size_t size)
{
	return Keyword(std::string(keyword,size));
}

}

}