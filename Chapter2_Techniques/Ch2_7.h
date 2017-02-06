#ifndef CH_2_7_H
#define CH_2_7_H

#include <string>

namespace Chapter2
{
	namespace Ch2_7
	{
		void Run();

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

	}
}

#endif
