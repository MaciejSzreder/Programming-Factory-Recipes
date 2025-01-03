#pragma once

#include<string>
#include<charconv>

#include"recipe.hpp"

struct Interpreter
{
	struct Command
	{
		enum{
			add,
			find
		} type;
		Value argument;
	};

	std::istream &in;
	std::ostream &out;

	Interpreter(std::istream& in, std::ostream& out):in(in),out(out){}

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

	void execute_command(Command &command, std::ostream& out)
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
					out << value->getShortRecipe() << '\n';
					return;
				}
				more_recipes(creatable);
			}
		}else{
			throw "not implemented command";
		}
	}

	void run()
	{
		for(;;){
			auto command = parse_command(in);
			execute_command(command,out);
		}
	}
};