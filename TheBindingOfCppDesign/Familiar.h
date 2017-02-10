#ifndef FAMILIAR_H
#define FAMILIAR_H

#include "Attacks.h"
#include "Collisions.h"
#include "MovementBehaviors.h"

template
<
typename AttackPolicy,
typename CollisionPolicy,
typename MovementPolicy
>
class Familiar : public CollisionPolicy, public MovementPolicy
{
public:
	std::string m_name;

	void Move()
	{
		MovementPolicy::Move();
	}
	template <class T>
	void SetFollowTarget(T* target)
	{
		MovementPolicy::SetNewTarget(target);
	}

	template<class O>
	void Collided(O* o)
	{
		CollisionPolicy::Collided(o);
	}

	void Attack()
	{
		AttackPolicy::Spawn();
	}
};

#endif