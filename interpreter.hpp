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
		
		auto number = FromChars<float>();
		auto string = Quoted() | Quoted('\'');
		auto identifier = "[^ \t\n\r'\"]+[^ \t\n\r]+"_re;

		auto commands = utils::map(commands::Commands::definitions, [](auto definition){return definition.name;});
		auto tokens = (Keyword::keywords(commands)+(number|identifier|string)).parse(line);
		Parsed argument = tokens[1];
		command.type = tokens[0].match;
		
		if(argument == number){
			command.argument = Value(std::any_cast<float>(argument.value));
		}else if(argument == string){
			command.argument = Value(argument.match);
		}else if(argument == identifier){
			command.argument = argument.match;
		}else{
			throw "unexpected token format";
		}
		return command;
	}

	void execute_command(Command &command, std::ostream& out)
	{
		commands::Commands::find(command.type)->execute(searcher,{command.argument});
	}

	void run()
	{
		for(;;){
			auto command = parse_command(in);
			execute_command(command,out);
		}
	}
};