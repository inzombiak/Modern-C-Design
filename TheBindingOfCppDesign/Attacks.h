#ifndef ATTACKS_H
#define ATTACKS_H

#include "Modifieres.h"
#include "Trajectories.h"

;
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

typedef SpawnEntity<NullModifier, NullTrajectory>		SpawnSpiderAttack;
typedef Collide<PoisonModifier, NullTrajectory>			OrbitingFlyAttack;
typedef ShootAndCollide<FireModifier, SameTrajectory>	LilBuddyAttack;

#endif