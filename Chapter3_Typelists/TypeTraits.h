#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

//I copied this from Loki, removed everything I didnt need

template <typename T>
class TypeTraits
{
private:

	template <class U> struct UnConst
	{
		typedef U Result;
		enum { isConst = 0 };
	};

	template <class U> struct UnConst<const U>
	{
		typedef U Result;
		enum { isConst = 1 };
	};

	template <class U> struct UnConst<const U&>
	{
		typedef U& Result;
		enum { isConst = 1 };
	};

public:
	typedef typename UnConst<T>::Result
		NonConstType;
	enum { isConst = UnConst<T>::isConst };
		
};

#endif