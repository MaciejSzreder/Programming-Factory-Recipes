#pragma once

#include<string>
#include<regex>
#include<cstdint>
#include<iomanip>
#include<any>

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

struct ParserLeaf
{
	std::intptr_t id = reinterpret_cast<std::intptr_t>(this);
};

struct Parsed
{
	bool succeed;
	std::string match;
	std::intptr_t parserId;
	std::any value;

	Parsed():succeed(false),match(""),parserId(0){}

	Parsed(std::string match, const ParserLeaf &parser, bool succeed = true):
		succeed(succeed),
		match(match),
		parserId(parser.id)
	{}

	template<typename T>
	Parsed(std::string match, const ParserLeaf &parser, T value, bool succeed = true):
		succeed(succeed),
		match(match),
		value(value),
		parserId(parser.id)
	{}

	operator bool()
	{
		return succeed;
	}

	bool operator== (const std::string &other)
	{
		return succeed && match == other;
	}

	bool operator== (const ParserLeaf &parser)
	{
		return succeed && parserId == parser.id;
	}
};

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

auto operator|(auto first, auto second)
{
	return Alternative(first,second);
}

template<class Expression1, class Expression2>
bool operator==(Parsed parsed, Alternative<Expression1,Expression2> alternative)
{
	return parsed == alternative.first || parsed == alternative.second;
}

template<class Expression1, class Expression2>
struct Sequence: Parser<Sequence<Expression1, Expression2>>
{
	using ParsedList = std::vector<Parsed>;

	Expression1 first;
	Expression2 second;

	Sequence(Expression1 first, Expression2 second): first(first), second(second){}

	ParsedList consume(auto &begin, auto end)
	{
		auto data1 = first.consume(begin,end);
		if(!data1){
			return {};
		}
		RegularExpression("[ \t\n\r]*").consume(begin,end);
		auto data2 = second.consume(begin,end);
		if(!data2){
			return {};
		}
		return {data1,data2};
	}
};

auto operator+(auto first, auto second)
{
	return Sequence(first,second);
}

struct Quoted: Parser<Quoted>, ParserLeaf
{
	char quote, escape;

	Quoted(char quote = '"', char escape = '\\'): quote(quote), escape(escape) {}

	Parsed consume(auto &begin, auto end)
	{
		std::string string;
		std::istringstream stream(std::string(begin,end));
		stream>>std::quoted(string, quote, escape);
		bool succeed = stream.good();
		begin += stream.tellg();
		return {string, *this, succeed};
	}
};

template<typename T>
struct FromChars: Parser<FromChars<T>>, ParserLeaf
{

	Parsed consume(auto &begin, auto end)
	{
		T value;
		std::string string(begin,end);
		auto [parsedEnd, error] = std::from_chars(string.data(), string.data()+string.size(), value);
		bool succeed = error == std::errc();
		std::string parsed((const char*)string.data(),parsedEnd);
		begin += parsedEnd - string.data();
		return {parsed, *this, value, succeed};
	}
};

namespace literals
{

Keyword operator""_k(const char *keyword,std::size_t size)
{
	return Keyword(std::string(keyword,size));
}

RegularExpression operator""_re(const char *re, std::size_t size)
{
	return RegularExpression(std::string(re,size));
}

} // literals

} // parser