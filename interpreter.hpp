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
		std::variant<Value,std::string> argument;
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
		auto string = Quoted() | Quoted('\'');
		auto identifier = "[^ \t\n\r'\"]+[^ \t\n\r]+"_re;

		auto tokens = (("add"_k|"find"_k)+(number|"NaN"_k|"Infinity"_k|"-Infinity"_k|identifier|string)).parse(line);
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
			command.argument = Value(number);
		}else if(argument == string){
			command.argument = Value(argument.match);
		}else if(argument == identifier){
			command.argument = argument.match;
		}else if(argument == "NaN"){
			command.argument = Value(std::nanf(""));
		}else if(argument == "Infinity"){
			command.argument = Value(std::numeric_limits<float>::infinity());
		}else if(argument == "-Infinity"){
			command.argument = Value(-std::numeric_limits<float>::infinity());
		}else{
			throw "unexpected token format";
		}
		return command;
	}

	void execute_command(Command &command, std::ostream& out)
	{
		if(command.type == Command::add){
			if(auto identifier = std::get_if<std::string>(&command.argument)){
				if(auto operation = Operations::operation(*identifier)){
					searcher.add(*operation);
				}else{
					searcher.add(*identifier);
				}
			}else{
				searcher.add(std::get<Value>(command.argument));
			}
		}else if(command.type == Command::find){
			Value argument;
			if(auto identifier = std::get_if<std::string>(&command.argument)){
				argument = *identifier;
			}else{
				argument = std::get<Value>(command.argument);
			}
			out << searcher.find(argument).getShortRecipe() << '\n';
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