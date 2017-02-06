#include "Ch2_7.h"
#include <iostream>
using namespace Chapter2;

void Ch2_7::Run()
{
	std::cout << std::endl;
	std::cout << "***** 2.6 *****" << std::endl;
	RenderComponent renderComp;
	Book book;
	if (Conversion<RenderComponent, IComponent>::exists)
		std::cout << "RenderComponent to IComponent exists" << std::endl;
	if (Conversion<IComponent, RenderComponent>::exists)
		std::cout << "IComponent to RenderComponent exists" << std::endl; // not called
	if (Conversion<RenderComponent, Book>::exists)
		std::cout << "RenderComponent to Book exists" << std::endl; //not called
}
