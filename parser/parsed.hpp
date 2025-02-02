#pragma once

#include<any>
#include<string>
#include<cstdint>

#include"parserleaf.hpp"

namespace parser
{

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
};

bool operator== (const Parsed &parsed, const ParserLeaf &parser)
{
	return parsed.succeed && parsed.parserId == parser.id;
}

}