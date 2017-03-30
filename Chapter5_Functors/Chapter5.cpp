#include "Chapter5.h"
#include "Functors.h"

#include <iostream>

struct AddDoubles
{
	double operator()(double a, double b)
	{
		double result = a + b;

		std::cout << a << " + " << b << " = " << result << std::endl;

		return result;
	}
};

struct AddInts
{
	int operator()(int a, int b)
	{
		int result = a + b;

		std::cout << a << " + " << b << " = " << result << std::endl;

		return result;
	}

};

int MultInts(int a, int b)
{
	int result = a * b;

	std::cout << a << " * " << b << " = " << result << std::endl;

	return result;
}

struct Vec2
{
	float x;
	float y;
};

float DotVecs(Vec2 v1, Vec2 v2)
{
	float result = (v1.x * v2.x) + (v1.y * v2.y);

	std::cout << "( " << v1.x << ", " << v1.y << ") * (" << v2.x << ", " << v2.y << ") = " << result << std::endl;

	return result;
}

struct Enemy
{
	std::string name;
};

class GasBombCollisionResponder
{
public:
	void Collided(const Enemy& e)
	{
		std::cout << e.name << " is poisoned" << std::endl;
	}
};

void Chapter5::Run()
{
	AddDoubles ad;
	AddInts ai;

	Functor<double, TYPELIST_2(double, double)> addDoubs(ad);
	Functor<int, TYPELIST_2(int, int)> addInt(ai);

	std::cout << "Calling functor with struct \"AddDoubles\" " << std::endl;
	addDoubs(4.4, 5.5);

	std::cout << "Calling functor with struct \"AddInts\" " << std::endl;
	addInt(4, 4);

	Functor<int, TYPELIST_2(int, int)> multInts(&MultInts);
	std::cout << "Calling functor with function \"MultInts\" " << std::endl;
	multInts(3, 2);

	std::cout << "Calling functor with function \"MultInts\" with a floating point number " << std::endl;
	multInts(4.5, 2); //Automatic conversion
	Functor<float, TYPELIST_2(Vec2, Vec2)> dotVecs(&DotVecs);
	Vec2 v1, v2;

	v1.x = 1;
	v1.y = 1;
	v2.x = 2;
	v2.y = 2;

	std::cout << "Calling functor with function \"DotVecs\" " << std::endl;
	dotVecs(v1, v2);

	Enemy abomination;
	abomination.name = "Abomination";

	GasBombCollisionResponder gbcr;
	Functor<void, TYPELIST_1(Enemy)> gbcrFunctor(&gbcr, &GasBombCollisionResponder::Collided);
	std::cout << "Calling functor with member function \"Collided\" of GasBombCollisionResponder " << std::endl;

	gbcrFunctor(abomination);

	std::cout << "Binding first value of \"MultInts\" to 100" << std::endl;
	Functor<int, TYPELIST_1(int)> mult100(BindFirst(multInts, 100));
	mult100(5);
	mult100(19);

	std::cout << "Chaining multInts and addInts to mult and add to numbers \"MultInts\" to 100" << std::endl;
	Functor<int, TYPELIST_2(int, int)> multAndAdd(Chain(multInts, addInt));
	//int x = multAndAdd(20, 5);
}