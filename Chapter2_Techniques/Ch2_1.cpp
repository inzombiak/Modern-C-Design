#include "Ch2_1.h"

#include <iostream>



struct A
{
	int a;
};
struct AB
{
	int a, b;
};

void Chapter2::Ch2_1::Run()
{
	std::cout << std::endl;
	std::cout << "***** 2.1 *****" << std::endl;
	STATIC_CHECK(sizeof(AB) >= sizeof(A), Destination_Too_Narrow);
}