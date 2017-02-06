#include "Ch2_5.h"
#include <iostream>

using namespace Chapter2;

void Ch2_5::Run()
{
	std::cout << std::endl;
	std::cout << "***** 2.5 *****" << std::endl;
	UnreadableBook* ub = Create(std::string("KlsdW was^$SDkwj kahQ"), Type2Type<UnreadableBook>());
	ReadableBook* rb = Create(5, Type2Type<ReadableBook>());

	ub->Read();
	rb->Read();

	delete ub;
	delete rb;
}

void Ch2_5::UnreadableBook::Read()
{
	std::cout << m_text << std::endl;
}

void Ch2_5::ReadableBook::Read()
{
	UnreadableBook::Read();
	std::cout << "Calling function with id: " << m_callBackID << std::endl;
}