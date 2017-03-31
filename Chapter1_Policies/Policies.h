#ifndef POLICIES_H
#define POLICIES_H
#include <iostream>
#include <string>

/*
	I was playing Binding of Isaac at the time on iOS and thought
	policies may be an interesting way to implement familiars
	This may not be the best way to solve the problem, but its an interesting way to test policy based design
*/


/*
Familiar Policies:
	AttackPolicy	- governs how "attacks" work, be it spawning something or shooting tears
	CollisionPolicy - governs how to deal with collisions
	Movement Policy	- governs how the familiar moves e.g. follow Isaac, random etc.
*/

/*
Attack Policies:
	ModifierPolicy		- governs what modifier (if any) are applied on collision with enemy
	TrajectoryPolicy	- governs distance, direction etc.
*/

namespace Policies
{
	namespace Modifiers
	{
		struct NullModifier
		{
			template<class T>
			static void Apply(T* other)
			{
				printf("No modifier to apply \n");
			}
		};
		struct FireModifier
		{
			template<class T>
			static void Apply(T* other)
			{
				std::cout << other->m_name << " was burning" << std::endl;
			}
		};
		struct PoisonModifier
		{
			template<class T>
			static void Apply(T* other)
			{
				std::cout << other->m_name << " was poisoned" << std::endl;
			}
		};

	};


	namespace Trajectories
	{
		struct NullTrajectory
		{
			static void Spawn()
			{
				printf("Spawn in place \n");
			}
		};
		struct ReverseTrajectory
		{
			static void Spawn()
			{
				printf("Spawn in opposite direction \n");
			}
		};
		struct SameTrajectory
		{
			static void Spawn()
			{
				printf("Spawn in same direction  \n");
			}
		};
	};

	namespace Attacks
	{
		template
			<
				class ModifierPolicy,
				class TrajectoryPolicy
			>
		struct SpawnEntity : public ModifierPolicy, public TrajectoryPolicy
		{
			static void Spawn()
			{
				TrajectoryPolicy::Spawn();
			}
		};

		template
			<
				class ModifierPolicy,
				class TrajectoryPolicy
			>
		struct Collide : public ModifierPolicy, public TrajectoryPolicy
		{
			template<class T>
			static void Collided(T* other)
			{
				ModifierPolicy::Apply(other);
			}
		};
		template
			<
				class ModifierPolicy,
				class TrajectoryPolicy
			>
		struct ShootAndCollide : public ModifierPolicy, public TrajectoryPolicy
		{
			static void Spawn()
			{
				TrajectoryPolicy::Spawn();
			}

			template<class T>
			static void Collided(T* other)
			{
				ModifierPolicy::Apply(other);
			}
		};

		typedef SpawnEntity<Modifiers::NullModifier,Trajectories::NullTrajectory>		SpawnSpiderAttack;
		typedef Collide<Modifiers::PoisonModifier, Trajectories::NullTrajectory>			OrbitingFlyAttack;
		typedef ShootAndCollide<Modifiers::FireModifier, Trajectories::SameTrajectory>	LilBuddyAttack;

	};

	namespace Collisions
	{
		template <typename AttackPolicy>
		struct NullCollision : public AttackPolicy
		{
			static void Collided()
			{};
		};
		template <typename AttackPolicy>
		struct DamageOnCollision : public AttackPolicy
		{
			template <class T>
			static void Collided(T* other)
			{
				std::cout << other->m_name << " hit" << std::endl;
				AttackPolicy::Collided(other);
			};
		};
		template <typename AttackPolicy>
		struct AttackOnCollision : public AttackPolicy
		{
			template<class T>
			static void Collided(T* other)
			{
				AttackPolicy::Spawn();
			}
		};
		typedef AttackOnCollision<Attacks::SpawnSpiderAttack>	SpawnSpiderOnCollision;
		typedef DamageOnCollision<Attacks::OrbitingFlyAttack>	FlyOnCollision;
		typedef AttackOnCollision<Attacks::LilBuddyAttack>		LBShootOnCollision;

	};

	namespace Movement
	{
		template <class Target>
		struct Follow
		{
			void Move()
			{
				std::cout << "Following " << t->m_name << std::endl;
			}

			void SetNewTarget(Target* newT)
			{
				t = newT;
			}

		private:
			Target* t;
		};
		struct RandomMovement
		{
			static void Move()
			{
				std::cout << "Moving randomly" << std::endl;
			}
		};

	};

};

#endif