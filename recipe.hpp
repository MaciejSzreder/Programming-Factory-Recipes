#pragma once

#include<vector>

#include"value.hpp"
#include"operations.hpp"

using operations::Operations;

struct Recipe
{
	Value value;
	const Operations::Definition *operation;
	std::vector<Value> recipients;

	std::string getShortExpression();
	std::string getShortRecipe();
};

using Recipes = std::vector<Recipe>;