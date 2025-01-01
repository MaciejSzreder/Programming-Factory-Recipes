#include<iostream>
#include<string>
#include<vector>
#include<span>

struct Value
{
	std::string str;
};

struct Operation
{
	enum{
		extracting,
		adding
	} operation;
	std::string getSymbol()
	{
		switch (operation){
		case extracting:
			return "";
		case adding:
			return "+";
		
		default:
			throw "lack symbol for operation " + operation;
		}
	}
};

struct Recipe
{
	Value value;
	Operation operation{Operation::extracting};
	std::vector<Value> recipients;
	std::string getShortExpression()
	{
		std::string expression = operation.getSymbol();
		for(const auto &recipient:recipients){
			expression += ' ' + recipient.str;
		}
		return expression;
	}
	std::string getShortRecipe(){
		return value.str + ": " + getShortExpression();
	}
};

struct Command
{
	enum{
		add,
		find
	} type;
	Value argument;
};

Command parse_command(std::istream& in)
{
	Command command;
	std::string token;
	in >> token;
	if(token == "add"){
		command.type = Command::add;
	}else if(token == "find"){
		command.type = Command::find;
	}else{
		throw "unknown command";
	}

	in >> token;
	command.argument.str = token;
	return command;
}

using Recipes = std::vector<Recipe>;
void print_span(std::span<const int> s)
{
    for (int n : s)
        std::cout << n << ' ';
    std::cout << '\n';
}
void more_recipes(Recipes &recipes)
{
	try{
		recipes.reserve(recipes.size()+recipes.size()*recipes.size());
	}catch(std::bad_alloc&){
		throw "Not enough memory to create now recipes";
	}
	std::span origin(recipes);
	for(auto &first: origin){
		for(auto &second: origin){
			recipes.push_back(Recipe{
				.value = first.value.str + second.value.str,
				.operation = Operation::adding,
				.recipients = {first.value,second.value}
			});
		}
	}
}

void execute_command(Command &command)
{
	static Recipes creatable;

	if(command.type == Command::add){
		creatable.push_back(Recipe{.value=command.argument});
	}else if(command.type == Command::find){
		for(;;){
			auto value = std::ranges::find_if(
				creatable,[=](const Recipe& recipe){
					return recipe.value.str == command.argument.str;
				}
			);
			if(value != creatable.end()){
				std::cout << value->getShortRecipe() << '\n';
				return;
			}
			more_recipes(creatable);
		}
	}else{
		throw "not implemented command";
	}
}

int main()
{
	for(;;){
		auto command = parse_command(std::cin);
		execute_command(command);
	}
}