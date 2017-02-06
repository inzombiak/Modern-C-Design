#ifndef CH_2_10_H
#define CH_2_10_h

/*
2.10. Type Traits
I used these in my Lua Wrapper class for dealing with return types.
I'm not sure if it makes sense , but here I use type traits to see if certain items can from certain lists can be combined
TODO: Check that statement
*/

#include "Ch2_4.h"

#include <string>

using Chapter2::Int2Type;

namespace Chapter2
{
	namespace Ch2_10
	{
		void Run();


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
		void CombineImpl(T obj1, U obj2, Int2Type<ProjectileElement>);
		template<class T, class U>
		void CombineImpl(T obj1, U obj2, Int2Type<WeaponElement>);
		template<class T, class U>
		void CombineImpl(T obj1, U obj2, Int2Type<WeaponProjectile>);
		template<class T, class U>
		void CombineImpl(T obj1, U obj2, Int2Type<NULLCombination>);

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
	}
}

#endif