#pragma once

#include<vector>

#include"parsed.hpp"

namespace parser
{

struct ParsedList
{
	bool succeed;
	std::vector<Parsed> parsed;

	ParsedList(std::vector<Parsed> parsed = {}): parsed(parsed),succeed(true) {}

	void append(Parsed data)
	{
		parsed.push_back(data);
	}

	operator bool()
	{
		return succeed;
	}
};

}