#pragma once

#include<optional>
#include<vector>
#include<string>
#include<functional>

#include"../register.hpp"

namespace operations
{

struct Operations:Register<Operations>{
	static std::optional<Operations::Definition> find(std::string name);
};

using OperationList = std::vector<Operations::Definition>;

}

template<>
struct Register<operations::Operations>::Definition
{
	using ArgumentList = std::vector<Value>;
	using function = std::function<Value(const ArgumentList&)>;
	int id;
	std::string name,symbol;
	int arity;
	function eval;

	Definition(auto definition):
		name(definition.name),
		symbol(definition.symbol),
		arity(definition.arity),
		eval(definition.eval)
	{}
};

std::optional<operations::Operations::Definition> operations::Operations::find(std::string name)
{
	return Register<Operations>::find(name, &Register<Operations>::Definition::name);
}