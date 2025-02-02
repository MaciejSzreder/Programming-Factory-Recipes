#pragma once

#include<cstdint>

#include"parsed.hpp"

namespace parser
{

struct ParserLeaf
{
	std::intptr_t id = reinterpret_cast<std::intptr_t>(this);
};

}