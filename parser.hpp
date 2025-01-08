#pragma once

#include<string>

namespace parser
{

template<class Expression>
struct Parser
{
	Expression &self = *static_cast<Expression*>(this);
	auto parse(auto begin, auto end)
	{
		return self.consume(begin,end);
	}

	auto parse(const auto &string)
	{
		return self.parse(std::begin(string),std::end(string));
	}
};

struct Parsed
{
	bool succeed;
	std::string match;

	Parsed():succeed(false),match(""){}

	Parsed(std::string match):succeed(true),match(match){}

	operator bool()
	{
		return succeed;
	}

	bool operator== (const std::string &other)
	{
		return succeed && match == other;
	}
};

struct Keyword: Parser<Keyword>
{
	std::string keyword;

	Keyword(std::string keyword): keyword(keyword){}

	Parsed consume(auto &begin, auto end)
	{
		for(const auto & character: keyword){
			if(begin == end || *begin!=character){
				return {};
			}
			++begin;
		}
		if( begin == end || !std::isalnum(*begin)){
			return keyword;
		}
		return {};
	}
};

template<class Expression1, class Expression2>
struct Alternative: Parser<Alternative<Expression1,Expression2>>
{
	Expression1 first;
	Expression2 second;

	Alternative(Expression1 first, Expression2 second): first(first),second(second){}

	auto consume(auto &begin, auto end)
	{
		auto backup = begin;
		auto data = first.consume(begin,end);
		if(data){
			return data;
		}
		begin = backup;
		return second.consume(begin,end);
	}
	

	auto parse(auto begin, auto end)
	{
		return consume(begin,end);
	}

	auto parse(const auto &string)
	{
		return parse(std::begin(string),std::end(string));
	}
};

auto operator|(auto first, auto second)
{
	return Alternative(first,second);
}

namespace literals
{

Keyword operator""_k(const char *keyword,std::size_t size)
{
	return Keyword(std::string(keyword,size));
}

} // literals

} // parser