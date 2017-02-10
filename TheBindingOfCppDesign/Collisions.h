#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Attacks.h"

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
typedef AttackOnCollision<SpawnSpiderAttack>	SpawnSpiderOnCollision;
typedef DamageOnCollision<OrbitingFlyAttack>	FlyOnCollision;
typedef AttackOnCollision<LilBuddyAttack>		LBShootOnCollision;


#endif