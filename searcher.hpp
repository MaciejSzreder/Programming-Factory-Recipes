#pragma once

#include"recipe.hpp"

struct Searcher
{
	Recipes creatable;
	OperationList operations;

	void add(Value value)
	{
		creatable.push_back({.value = value});
	}

	void add(Operations::Definition operation)
	{
		operations.push_back(operation);
	}
	
	void moreRecipes()
	{
		const OperationList &operationList = operations.empty() ? Operations::definitions : operations;
		try{
			creatable.reserve(creatable.size()+creatable.size()*creatable.size()*operationList.size());
		}catch(std::bad_alloc&){
			throw "Not enough memory to create now recipes";
		}
		std::span origin(creatable);
		for(const auto &operation: operationList){
			if(operation.arity == 1){
				for(const auto &first: origin){
					Value value = operation.eval({first.value});
					if(!value.empty()){
						creatable.push_back(Recipe{
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
							creatable.push_back(Recipe{
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
		std::ranges::stable_sort(creatable,Value::Order(),&Recipe::value);
		creatable.erase(std::ranges::unique(creatable,Value::Equality(),&Recipe::value).begin(),creatable.end());
	}

	Recipe find(Value value)
	{
		for(;;){
			auto recipe = std::ranges::find_if(
				creatable,[=](const Recipe& recipe){
					return recipe.value == value || recipe.value!=recipe.value && value!=value;
				}
			);
			if(recipe != creatable.end()){
				return *recipe;
			}
			moreRecipes();
		}
	}
};