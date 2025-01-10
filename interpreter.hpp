#pragma once

#include<string>
#include<charconv>

#include"recipe.hpp"
#include"parser.hpp"
#include"searcher.hpp"

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

	Searcher searcher;

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
		if(command.type == Command::add){
			if(auto operation = Operations::operation(command.argument.stringify())){
				searcher.add(*operation);
			}else{
				searcher.add(command.argument);
			}
		}else if(command.type == Command::find){
			out << searcher.find(command.argument).getShortRecipe() << '\n';
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