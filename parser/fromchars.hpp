#pragma once

#include<string>
#include<charconv>

#include"parsed.hpp"
#include"parser.hpp"
#include"parserleaf.hpp"

namespace parser
{

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

}