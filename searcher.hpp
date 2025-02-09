#pragma once

#include"recipe.hpp"
#include"operations.hpp"

using operations::Operations;
using operations::OperationList;

struct Searcher
{
	Recipes creatable;
	OperationList operations;

	void add(Value value);
	void add(Operations::Definition operation);
	
	void moreRecipes();

	Recipe find(Value value);

	void clear();
};