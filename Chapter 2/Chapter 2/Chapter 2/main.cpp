/*
	Chapter 2 covers generic programming techniques.
	I don't know if I'll be able to come up with a 
	theme for these, so I'll seperate them into sections and if I
	come up with an idea merge them.
	Some of the ideas may not fit into game dev, but I'm going to do them anyway
*/

#include <iostream>
#include <string>

/*

	2.1. Compile-Time Assertions

	TODO: Merge with policies for better messages
*/

template <bool> struct CompileTimeChecker
{
	CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false> { };
#define STATIC_CHECK(expr, msg) \
{\
	 class ERROR_##msg {} ;\
	(void) sizeof((::CompileTimeChecker<(expr) != 0> (ERROR_##msg())));\
}

struct A
{
	int a;
};
struct AB
{
	int a, b;
};

/*
	End of 2.1.
*/

/*
	2.2. Partial Template Specialization

	TODO: Merge to policies with above, nothing special here
*/

/*
	End of 2.2.
*/

/*
	2.3. Local Classes
*/
class Global
{
public:
	Global(int n)
	{
		m_n = n;
	}

	virtual void Run()
	{
		std::cout << "I'm global number: " << m_n << std::endl;
	}
protected:
	int m_n;
};

Global* MakeALocalGlobal(int n)
{
	class Local : public Global
	{
	public:
		Local(int n) : Global(n)
		{
			
		}

		virtual void Run()
		{
			std::cout << "I'm local number: " << m_n << std::endl;
		}

		void TryMeFromMain()
		{

		}
	};

	return new Local(n);
}
/*
	End of 2.3.
*/

/*
	2.4. Mapping Integral Constants to Types

	TODO: Merge with policies use to ease familiar restrictions
*/

template <int v>
struct Int2Type
{
	static const int value = v;
};

enum WallType
{
	GlassWall = 0,
	BrickWall = 1,
	WoodWall = 2,
};

class Glass
{
public:
	void Break()
	{
		std::cout << "Glass broke" << std::endl;
	}
};

class Brick
{
public:
	void BreakFist()
	{
		std::cout << "Your fist broke" << std::endl;
	}
};

class Wood
{
public:
	void BreakBoth()
	{
		std::cout << "Your fist and the wood broke" << std::endl;
	}
};

template <typename T, WallType isBreakable>
class Wall
{
public:
	void Hit(T* obj)
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
private:
	void Hit(T* obj, Int2Type<GlassWall>)
	{
		obj->Break();
	}

	void Hit(T* obj, Int2Type<BrickWall>)
	{
		obj->BreakFist();
	}

	void Hit(T* obj, Int2Type<WoodWall>)
	{
		obj->BreakBoth();
	}
};

/*
	End 2.4.
	*/

/*
	2.5. Type-to-Type Mapping

	For this example we assume we have 2 types of books
	1. Unreadable - displays what looks like gibberish when "Read" is invoked.
	2. Readable - displays text and invokes a function when "Read" is invoked.

	Also assume that we need to pass the ID of function that is called through "Use"
	to the constructor
*/

template <typename T>
struct Type2Type
{
	typedef T OriginalType;
};

class UnreadableBook
{
public:

	UnreadableBook(std::string textOnRead)
	{
		m_text = textOnRead;
	}

	virtual void Read()
	{
		std::cout << m_text << std::endl;
	}

protected:
	std::string m_text;
};

//Callback ID could be replaced with a funciton ptr or lambda in real code
class ReadableBook : public UnreadableBook
{
public:

	ReadableBook(int callBackID, std::string textOnRead) : UnreadableBook(textOnRead)
	{
		m_callBackID = callBackID;
	}

	virtual void Read()
	{
		UnreadableBook::Read();
		std::cout << "Calling function with id: " << m_callBackID << std::endl;
	}

private:
	int m_callBackID;
};

template<class T, class A>
T* Create(const A& arg, Type2Type<T>)
{
	return new T(arg);
}

template<class A>
ReadableBook* Create(const A& arg, Type2Type<ReadableBook>)
{
	/*
		Complex code that chooses appropriate text
		based on the function callback ID provided in arg
	*/
	std::string text = "Guess you're not completely illiterate, looks like you can read " + std::to_string(arg) + " words";

	return new ReadableBook(arg, text);
}

/*
	End 2.5.
*/

/*
	2.6. Type Selection
	TODO: FIGURE IT OUT
*/

template <bool flag, typename T, typename U>
struct Select
{
	typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	typedef U Result;
};


/*
	End 2.6.
*/

/*
	2.7. Detecting Convertibility and Inheritance at Compile Time

	TODO: Test for components, merge with the policies code
*/

template <class T, class U>
class Conversion
{
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test(const U&);
	static Big Test(...);
	static T MakeT(); //In case default constructor is id hidden. sizeof doesn't actually try to call this function
public:

	static const int exists = sizeof(Test(MakeT())) == sizeof(Small);
	static const int sameType = false;
};

template<class T>
class Conversion<T, T>
{
public:
	static const int exists = 1;
	static const int sameType = 1;
};


struct IComponent
{
	int m_id;
};

struct RenderComponent : public IComponent
{
	int m_vertexCount;
};

struct Book
{
	std::string title;
};

/*
	End 2.7.
*/

/*
	2.8. A Wrapper Around std::type_info
	TODO: copy paste?
*/



/*
	End 2.8.
*/

/*
	2.9. NullType and EmptyType
*/

class NullType {};

struct EmptyType {};

/*
	End 2.9.
*/

/*
	2.10. Type Traits
	I used these in my Lua Wrapper class for dealing with return types.
	I'm not sure if it makes sense , but here I use type traits to see if certain items can from certain lists can be combined
	TODO: Check that statement
*/

struct Poison
{
	std::string text = "Poison";
};

struct Water
{
	std::string text = "Water";
};

struct Rock
{
	std::string text = "Rock";
};

struct Fire
{
	std::string text = "Fire";
};

struct Paper
{
	std::string text = "Paper";
};

struct Sword
{
	std::string text = "Sword";
};

struct Arrow
{
	std::string text = "Arrow";;
};

struct Bow
{
	std::string text = "Bow";;
};

template <class T>
struct is_element
{
	static const int value = false;
};
template <>
struct is_element<Fire>
{
	static const int value = true;
};
template <>
struct is_element<Water>
{
	static const int value = true;
};
template <>
struct is_element<Poison>
{
	static const int value = true;
};

template<class T>
struct is_projectile
{
	static const int value = false;
};
template<>
struct is_projectile<Rock>
{
	static const int value = true;
};
template<>
struct is_projectile<Arrow>
{
	static const int value = true;
};
template<>
struct is_projectile<Paper>
{
	static const int value = true;
};

template<class T>
struct is_melee
{
	static const int value = false;
};
template<>
struct is_melee<Sword>
{
	static const int value = true;
};

template <typename T>
class TypeTraits
{
private:


public:
	static const int isElement = is_element<T>::value;
	static const int isMelee = is_melee<T>::value;
	static const int isProjectile = is_projectile<T>::value;
};

enum CombineFuncSelector
{
	WeaponElement,
	ProjectileElement,
	WeaponProjectile,
	NULLCombination,
};

template<class T, class U>
void CombineImpl(T obj1, U obj2, Int2Type<WeaponElement>)
{
	std::cout << obj1.text << " has the " << obj2.text << " modifier " << std::endl;
}

template<class T, class U>
void CombineImpl(T obj1, U obj2, Int2Type<ProjectileElement>)
{
	std::cout << "Now firing " <<obj1.text << "s that have the " << obj2.text << " modifier " << std::endl;
}

template<class T, class U>
void CombineImpl(T obj1, U obj2, Int2Type<WeaponProjectile>)
{
	std::cout << obj1.text << " shoots " << obj2.text << "s now " << std::endl;
}

template<class T, class U>
void CombineImpl(T obj1, U obj2, Int2Type<NULLCombination>)
{
	std::cout << obj1.text << " can't be combined with " << obj2.text << std::endl;
}

template<class T, class U>
void Combine(T base, U component)
{
	enum
	{
		cfs = 
		TypeTraits<T>::isMelee &&
		TypeTraits<U>::isElement ? WeaponElement :
		!TypeTraits<T>::isMelee &&
		TypeTraits<U>::isProjectile ? WeaponProjectile :
		TypeTraits<T>::isProjectile &&
		TypeTraits<U>::isElement ? ProjectileElement : NULLCombination
	};

	
	CombineImpl(base, component, Int2Type<cfs>());
}

/*
	End 2.10.
*/

int main()
{
	/*
		2.1
	*/
	std::cout << std::endl;
	std::cout << "***** 2.1 *****" << std::endl;
	STATIC_CHECK(sizeof(AB) >= sizeof(A), Destination_Too_Narrow);

	/*
		2.3
	*/
	std::cout << std::endl;
	std::cout << "***** 2.3 *****" << std::endl;

	Global* global1;
	Global* global2 = new Global(2);

	global1 = MakeALocalGlobal(1);
	global1->Run();
	//global->TryMeFromMain(); //NOPE
	global2->Run();

	delete global1;
	delete global2;

	/*
		2.4
	*/
	std::cout << std::endl;
	std::cout << "***** 2.4 *****" << std::endl;
	Glass g;
	Brick b;
	Wood w;

	Wall<Glass, GlassWall> gw;
	Wall<Brick, BrickWall> bw;
	Wall<Wood,	WoodWall> ww;

	gw.Hit(&g);
	bw.Hit(&b);
	ww.Hit(&w);

	/*
		2.5
	*/
	std::cout << std::endl;
	std::cout << "***** 2.5 *****" << std::endl;
	UnreadableBook* ub = Create(std::string("KlsdW was^$SDkwj kahQ"), Type2Type<UnreadableBook>());
	ReadableBook* rb = Create(5, Type2Type<ReadableBook>());

	ub->Read();
	rb->Read();

	delete ub;
	delete rb;

	/*
		2.6
	*/
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

	/*
		2.10
	*/
	std::cout << std::endl;
	std::cout << "***** 2.10 *****" << std::endl;
	
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