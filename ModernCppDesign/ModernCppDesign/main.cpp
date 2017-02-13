#include "Policies.h"
#include "Chapter2.h"
#include "Chapter3.h"
#include "Game.h"

#include <iostream>

int main()
{
	/*
	std::cout << "****Chapter 1 Start****" << std::endl;
	std::cout << std::endl;
	Chapter1::Policies::Run();
	std::cout << std::endl;
	std::cout << "****Chapter 1 End****" << std::endl;
	*/
	/*
	std::cout << "****Chapter 2 Start****" << std::endl;
	std::cout << std::endl;
	Chapter2::Run();
	std::cout << std::endl;
	std::cout << "****Chapter 2 End****" << std::endl;
	*/

	std::cout << "****Chapter 3 Start****" << std::endl;
	std::cout << std::endl;
	Chapter3::Run();
	std::cout << std::endl;
	std::cout << "****Chapter 3 End****" << std::endl;

	Game g;
	g.Run();
}