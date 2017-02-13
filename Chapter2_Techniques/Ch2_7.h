#ifndef CH_2_7_H
#define CH_2_7_H

#include <string>
/*
2.7. Detecting Convertibility and Inheritance at Compile Time
Checks if a type T can be converted U
In this example I use it to check for convertability for some objects
*/
namespace Chapter2
{
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

	namespace Ch2_7
	{
		void Run();

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

	}
}

//Define to make it more clear what were checking (if U inherits from T)
#define SUPERSUBCLASS(T, U)\
		(Chapter2::Conversion<const U*, const T*>::exists && \
		!Chapter2::Conversion<const T*, const void*>::sameType)

#endif
