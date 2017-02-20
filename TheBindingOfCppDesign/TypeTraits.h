#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

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

	static const int isElement = is_element<T>::value;
	static const int isMelee = is_melee<T>::value;
	static const int isProjectile = is_projectile<T>::value;
};

#endif