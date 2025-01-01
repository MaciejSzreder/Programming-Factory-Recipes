#include<iostream>
#include<string>
#include<vector>

struct Command
{
	enum
	{
		add,
		find
	} type;
	std::string argument;
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
	command.argument = token;
	return command;
}

void execute_command(Command &command)
{
	static std::vector<std::string> resources;

	if(command.type == Command::add){
		resources.push_back(command.argument);
	}else if(command.type == Command::find){
		auto value = std::ranges::find(resources,command.argument);
		if(value == resources.end()){
			std::cout << "value is not creatable\n";
			return;
		}
		std::cout << *value << '\n';
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