#pragma once

#include<string>
#include<iomanip>
#include<sstream>

#include"parsed.hpp"
#include"parser.hpp"
#include"parserleaf.hpp"

namespace parser
{

struct Quoted: Parser<Quoted>, ParserLeaf
{
	char quote, escape;

	Quoted(char quote = '"', char escape = '\\'): quote(quote), escape(escape) {}

	Parsed consume(auto &begin, auto end)
	{
		auto backup = begin;
		std::string string;
		std::istringstream stream(std::string(begin,end));
		stream>>std::quoted(string, quote, escape);
		bool succeed = stream.good();
		if(succeed){
			begin += stream.tellg();
		}
		return {std::string(backup, begin), *this, string, succeed};
	}
};

}