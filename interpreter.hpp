#pragma once

#include<string>
#include<charconv>

#include"utils.hpp"
#include"recipe.hpp"
#include"parser.hpp"
#include"searcher.hpp"
#include"commands.hpp"

using namespace parser;
using namespace parser::literals;

struct Interpreter
{
	struct Command
	{
		std::string type;
		std::vector<std::variant<Value,std::string>> arguments;
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
		
		auto number = FromChars<float>();
		auto string = Quoted() | Quoted('\'');
		auto identifier = "[^ \t\n\r'\"]+[^ \t\n\r]+"_re;
		auto commands = utils::map(commands::Commands::definitions, [](auto definition){return definition.name;});
		
		auto tokens = (Keyword::keywords(commands)+*(number|identifier|string)).parse(line);

		command.type = tokens.get<0>().match;
		
		auto arguments = utils::map(tokens.get<1>().parsed,[=](auto argument){
			if(argument == number){
				return Value(std::any_cast<float>(argument.value));
			}else if(argument == string){
				return Value(argument.match);
			}else if(argument == identifier){
				return Value(argument.match);
			}else{
				throw "unexpected token format";
			}
		});
		command.arguments.assign(arguments.begin(),arguments.end());

		return command;
	}

	void execute_command(Command &command, std::ostream& out)
	{
		commands::Commands::find(command.type)->execute(searcher,command.arguments);
	}

	void run()
	{
		for(;;){
			auto command = parse_command(in);
			execute_command(command,out);
		}
	}
};