#include"value.hpp"
#include"operations.hpp"

#include"recipe.hpp"

std::string Recipe::getShortExpression()
{
	std::string expression = operation?operation->symbol:value.stringify();
	for(const auto &recipient:recipients){
		expression += ' ' + recipient.stringify();
	}
	return expression;
}
std::string Recipe::getShortRecipe(){
	return value.stringify() + ": " + getShortExpression();
}
