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


void more_recipes(Recipes &recipes, const OperationList &operations)
{
	const OperationList &operationList = operations.empty() ? Operations::operations : operations;
	try{
		recipes.reserve(recipes.size()+recipes.size()*recipes.size()*operationList.size());
	}catch(std::bad_alloc&){
		throw "Not enough memory to create now recipes";
	}
	std::span origin(recipes);
	for(const auto &operation: operationList){
		if(operation.arity == 1){
			for(const auto &first: origin){
				Value value = operation.eval({first.value});
				if(!value.empty()){
					recipes.push_back(Recipe{
						.value = value,
						.operation = &operation,
						.recipients = {first.value}
					});
				}
			}
		} else if(operation.arity == 2){	
			for(auto &first: origin){
				for(const auto &second: origin){
					Value value = operation.eval({first.value, second.value});
					if(!value.empty()){
						recipes.push_back(Recipe{
							.value = value,
							.operation = &operation,
							.recipients = {first.value,second.value}
						});
					}
				}
			}
		} else {
			throw "Arity " + std::to_string(operation.arity) + " is not implemented";
		}
	}
	std::ranges::stable_sort(recipes,Value::Order(),&Recipe::value);
	recipes.erase(std::ranges::unique(recipes,Value::Equality(),&Recipe::value).begin(),recipes.end());
}
