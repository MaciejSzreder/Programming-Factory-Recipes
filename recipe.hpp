#pragma once

#include<vector>
#include<span>
#include<algorithm>

#include"value.hpp"
#include"operations.hpp"

struct Recipe
{
	Value value;
	const Operations::Operation *operation;
	std::vector<Value> recipients;
	std::string getShortExpression()
	{
		std::string expression = operation?operation->symbol:value.stringify();
		for(const auto &recipient:recipients){
			expression += ' ' + recipient.stringify();
		}
		return expression;
	}
	std::string getShortRecipe(){
		return value.stringify() + ": " + getShortExpression();
	}
};

using Recipes = std::vector<Recipe>;