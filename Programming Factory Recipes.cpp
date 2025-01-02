#include<iostream>
#include<string>
#include<vector>
#include<span>
#include<variant>
#include<charconv>
#include<cmath>

using namespace std::literals::string_literals;

static std::string stringify(std::nullptr_t)
{
	return "no value";
}

static std::string stringify(std::string value)
{
	return value;
}

template<typename T>
static std::string stringify(T value)
{
	return std::to_string(value);
}

struct Value
{
	std::variant<std::nullptr_t, std::string, float> value;

	std::string stringify() const
	{
		return std::visit([](auto value){ return ::stringify(value);},value);
	}

	bool empty()
	{
		return std::holds_alternative<std::nullptr_t>(value);
	}
};

Value add(const Value &first, const Value &second)
{
	{
		const float *f, *s;
		if((f = std::get_if<float>(&first.value)) && (s = std::get_if<float>(&second.value))){
			return Value{.value = *f+*s};
		}
	}
	{
		const std::string *f,*s;
		if((f = std::get_if<std::string>(&first.value)) && (s = std::get_if<std::string>(&second.value))){
			return Value{.value = *f+*s};
		}
	}
	{
		const std::string *text;
		const float *number;
		if((text = std::get_if<std::string>(&first.value)) && (number = std::get_if<float>(&second.value))){
			return Value{.value = *text+std::to_string(*number)};
		}
		if((number = std::get_if<float>(&first.value)) && (text = std::get_if<std::string>(&second.value))){
			return Value{.value = *text+std::to_string(*number)};
		}
	}
	throw "unexpected value type " + std::to_string(first.value.index()) + " and " + std::to_string(second.value.index());
}

Value mod(const Value &first, const Value &second)
{
	const float *f, *s;
	if((f = std::get_if<float>(&first.value)) && (s = std::get_if<float>(&second.value))){
		if(*f>*s){
			std::swap(f,s);
		}
		return Value{.value = std::fmod(*s,*f)};
	}
	return {};
}

struct Operation
{
	enum{
		extracting,
		adding,
		remindering
	} operation;
	std::string getSymbol()
	{
		switch (operation){
		case extracting:
			return "";
		case adding:
			return "+";
		case remindering:
			return "%";
		
		default:
			throw "lack symbol for operation " + std::to_string(operation);
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
			expression += ' ' + recipient.stringify();
		}
		return expression;
	}
	std::string getShortRecipe(){
		return value.stringify() + ": " + getShortExpression();
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
	float number;
	if(std::from_chars(token.data(),token.data()+token.size(),number).ec == std::errc()){
		command.argument.value = number;
	}else{
		command.argument.value = token;
	}
	return command;
}

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
				.value = add(first.value, second.value),
				.operation = Operation::adding,
				.recipients = {first.value,second.value}
			});
			Value value = mod(first.value, second.value);
			if(!value.empty()){
				recipes.push_back(Recipe{
					.value = value,
					.operation = Operation::remindering,
					.recipients = {first.value,second.value}
				});
			}
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
					return recipe.value.stringify() == command.argument.stringify();
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