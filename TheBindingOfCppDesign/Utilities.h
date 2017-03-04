#ifndef UTILITIES_H
#define UTILITIES_H

#define STATIC_CHECK(expr, msg) \
		{\
			class ERROR_##msg {} ;\
			(void) sizeof((Utilities::CompileTimeChecker<(expr) != 0> (ERROR_##msg())));\
		}

class NullType {};
struct EmptyType {};

namespace GameDefs
{
	//TODO: Allow combining flags, remove duplicates
	enum AttackTypes
	{
		Shoot = 0,
		Spawn = 1,
		Collide = 2,
		ShootAndCollide = 3,
	};

	enum EntityTypes
	{
		Player,
		Wall,
		Enemy
	};

	enum CollisionTypes
	{
		AttackOnCollision = 0,
		DamageOnCollision = 1,
	};

	enum ModifierTypes
	{
		Fire = 0,
		Poison = 1,
	};

	enum MovementTypes
	{
		Follow = 0,
		Random = 1,
		Stationary = 2,
	};

	enum TrajectoryTypes
	{
		SameDirection = 0,
		ReverseDirection = 1,
	};

}

namespace Utilities
{
	template <class T, class U>
	class Conversion
	{
		typedef char Small;
		class Big { char dummy[2]; };
		static Small Test(const U&);
		static Big Test(...);

		//In case default constructor is is hidden. sizeof doesn't actually try to call this function
		static T MakeT(); 
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

	template <int v>
	struct Int2Type
	{
		static const int value = v;
	};

	template <typename T>
	struct Type2Type
	{
		typedef T OriginalType;
	};

	template <bool> struct CompileTimeChecker
	{
		CompileTimeChecker(...);
	};
	template<> struct CompileTimeChecker<false> { };

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
}

#define SUPERSUBCLASS(T, U)\
		(Utilities::Conversion<const U*, const T*>::exists && \
		!Utilities::Conversion<const T*, const void*>::sameType)
#endif