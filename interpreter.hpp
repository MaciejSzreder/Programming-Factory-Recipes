#pragma once

#include<string>
#include<charconv>

#include"recipe.hpp"
#include"parser.hpp"

using namespace parser;
using namespace parser::literals;

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
		std::string line;
		std::getline(in,line);
		
		auto number = "-?[0-9]+([.][0-9]+)?([eE]-?[0-9]+)?"_re;
		auto string = "'([^']|\\')+'"_re;
		auto identifier = "[^ \t\n\r]+"_re;

		auto tokens = (("add"_k|"find"_k)+(number|string|identifier)).parse(line);
		Parsed opcode = tokens[0], argument = tokens[1];
		if(opcode == "add"){
			command.type = Command::add;
		}else if(opcode == "find"){
			command.type = Command::find;
		}else{
			throw "unknown command";
		}
		
		if(argument == number){
			float number;
			std::from_chars(argument.match.data(),argument.match.data()+argument.match.size(),number);
			command.argument.value = number;
		}else if(argument == string || argument == identifier){
			command.argument.value = argument.match;
		}else{
			throw "unexpected token format";
		}
		return command;
	}

	void execute_command(Command &command, std::ostream& out)
	{
		static Recipes creatable;
		static OperationList operations;

		if(command.type == Command::add){
			if(auto operation = Operations::operation(command.argument.stringify())){
				operations.push_back(*operation);
			}else{
				creatable.push_back(Recipe{.value=command.argument});
			}
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
				more_recipes(creatable, operations);
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