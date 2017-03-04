#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include "Utilities.h"

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

	static const bool isModifier		= is_modifier<T>::value;
	static const bool isTrajectory		= is_trajectory<T>::value;
	static const bool isAttack			= is_attack<T>::value;
	static const bool isMovement		= is_movement<T>::value;
	static const bool isCollision		= is_collision<T>::value;
};

#endif