#include "Ch2_4.h"

#include <iostream>

using namespace Chapter2;
void Ch2_4::Run()
{
	std::cout << std::endl;
	std::cout << "***** 2.4 *****" << std::endl;
	Glass g;
	Brick b;
	Wood w;

	Wall<Glass, GlassWall> gw;
	Wall<Brick, BrickWall> bw;
	Wall<Wood, WoodWall> ww;

	gw.Hit(&g);
	bw.Hit(&b);
	ww.Hit(&w);
}

void Ch2_4::Glass::Break()
{
	std::cout << "Glass broke" << std::endl;
}

void Ch2_4::Brick::BreakFist()
{
	std::cout << "Your fist broke" << std::endl;
}

void Ch2_4::Wood::BreakBoth()
{
	std::cout << "Your fist and the wood broke" << std::endl;
}

template <typename T, Ch2_4::WallType isBreakable>
void Ch2_4::Wall<T, isBreakable>::Hit(T* obj)
{
	Hit(obj, Int2Type<isBreakable>());
	//Uncomment and try to compile
	/*
	if(isBreakable == GlassWall)
	{
	obj->Break();
	}
	else if (isBreakable == BrickWall)
	{
	obj->BreakFist();
	}
	else if (isBreakable == WoodWall)
	{
	obj->BreakBoth();
	}
	*/
}
