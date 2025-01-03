#pragma once

#include<vector>
#include<span>

#include"value.hpp"
#include"operations.hpp"

struct Recipe
{
	Value value;
	Operation operation{Operation::extracting};
	std::vector<Value> recipients;
	std::string getShortExpression()
	{
		std::string expression = operation.getSymbol();
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


void more_recipes(Recipes &recipes)
{
	try{
		recipes.reserve(recipes.size()+recipes.size()*recipes.size()*2);
	}catch(std::bad_alloc&){
		throw "Not enough memory to create now recipes";
	}
	std::span origin(recipes);
	for(const auto &first: origin){
		for(const auto &second: origin){
			recipes.push_back(Recipe{
				.value = Operation::add(first.value, second.value),
				.operation = Operation::adding,
				.recipients = {first.value,second.value}
			});
			Value value = Operation::mod(first.value, second.value);
			if(!value.empty()){
				recipes.push_back(Recipe{
					.value = value,
					.operation = Operation::remaindering,
					.recipients = {first.value,second.value}
				});
			}
		}
	}
}
