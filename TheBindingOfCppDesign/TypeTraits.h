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

	static const int isModifier		= is_modifier<T>::value;
	static const int isTrajectory	= is_trajectory<T>::value;
	static const int isAttack		= is_attack<T>::value;
	static const int isMovement		= is_movement<T>::value;
	static const int isCollision	= is_collision<T>::value;
};

#endif