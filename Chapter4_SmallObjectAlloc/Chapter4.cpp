#include "Chapter4.h"
#include "SmallObjAllocator.h"

#include <iostream>

class Vector2f : public SmallObjectBase<>
{
public:
	float x;
	float y;
};


class Vector10d : public SmallObjectBase<>
{
public:
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	double e = 0;
	double f = 0;
	double g = 0;
	double h = 0;
	double i = 0;
	double j = 0;
};


void Chapter4::Run()
{
	std::cout << "Creating object Vector2f: contains 2 floats, size is: " << sizeof(Vector2f) << std::endl;

	Vector2f* vec = new Vector2f();

	vec->x = 15;
	std::cout << vec->x << std::endl;

	std::cout << "Creating object Vector10d: contains 10 floats, size is: " << sizeof(Vector10d) << std::endl;

	Vector10d* vec10 = new Vector10d();

	vec10->a = 10;
	std::cout << vec10->a << std::endl;

	delete vec10;
	delete vec;
}