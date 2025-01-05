#include<iostream>

#include"interpreter.hpp"

int main()
{
	try{
		Interpreter(std::cin, std::cout).run();
	}catch(char const *message){
		std::cout<<message;
	}
}