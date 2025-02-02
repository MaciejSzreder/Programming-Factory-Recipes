#pragma once

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

}