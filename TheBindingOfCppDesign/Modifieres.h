#ifndef MODIFIERES_H
#define MODIFIERES_H

#include <iostream>

class NullModifier
{
public:
	template<class T>
	static void Apply(T* other)
	{
		printf("No modifier to apply \n");
	}
};

class FireModifier
{
public:
	template<class T>
	static void Apply(T* other)
	{
		std::cout << other->m_name << " was burning" << std::endl;
	}
};

class PoisonModifier
{
public:
	template<class T>
	static void Apply(T* other)
	{
		std::cout << other->m_name << " was poisoned" << std::endl;
	}
};

#endif