#include "Ch2_10.h"
#include "Ch2_4.h"

#include <iostream>

using namespace Chapter2;

void Ch2_10::Run()
{
	Poison poison;
	Water water;
	Rock rock;
	Fire fire;
	Paper paper;
	Sword sword;
	Arrow arrow;
	Bow bow;

	Combine(poison, fire);
	Combine(sword, sword);
	Combine(sword, water);
	Combine(bow, rock);
	Combine(rock, fire);
}

template<class T, class U>
void Ch2_10::CombineImpl<T, U>(T obj1, U obj2, Int2Type<WeaponElement>)
{
	std::cout << obj1.text << " has the " << obj2.text << " modifier " << std::endl;
}

template<class T, class U>
void Ch2_10::CombineImpl<T, U>(T obj1, U obj2, Int2Type<ProjectileElement>)
{
	std::cout << "Now firing " << obj1.text << "s that have the " << obj2.text << " modifier " << std::endl;
}

template<class T, class U>
void Ch2_10::CombineImpl<T, U>(T obj1, U obj2, Int2Type<WeaponProjectile>)
{
	std::cout << obj1.text << " shoots " << obj2.text << "s now " << std::endl;
}

template<class T, class U>
void Ch2_10::CombineImpl<T, U>(T obj1, U obj2, Int2Type<NULLCombination>)
{
	std::cout << obj1.text << " can't be combined with " << obj2.text << std::endl;
}
