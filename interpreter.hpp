#pragma once

#include<string>
#include<ostream>
#include<istream>

#include"searcher.hpp"
#include"commands.hpp"

struct Interpreter
{
	struct Command
	{
		std::string type;
		commands::Commands::Definition::ArgumentList arguments;
	};

	std::istream &in;
	std::ostream &out;

	Searcher searcher;

	Interpreter(std::istream& in, std::ostream& out):in(in),out(out){}

	Command parse_command(std::istream& in);

	void execute_command(Command &command, std::ostream& out);

	void run();
};