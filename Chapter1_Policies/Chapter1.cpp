#include "Chapter1.h"
#include "Policies.h"

class Player
{
public:
	std::string m_name;
	std::string m_directionFacing;
};

class Enemy
{
public:
	std::string m_name;
};

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

using namespace Policies;

typedef Familiar<Attacks::LilBuddyAttack,		Collisions::LBShootOnCollision,		Movement::Follow<Player>>		LilBuddyFamiliar;
typedef	Familiar<Attacks::OrbitingFlyAttack,	Collisions::FlyOnCollision,			Movement::Follow<Enemy>>		Fly;
typedef Familiar<Attacks::SpawnSpiderAttack,	Collisions::SpawnSpiderOnCollision,	Movement::RandomMovement>		SpawnSpiderFamiliar;

void  Chapter1::Run()
{
	Enemy e1;
	Enemy e2;
	e1.m_name = "E1";
	e2.m_name = "E2";

	Player p1;
	Player p2;
	p1.m_name = "P1";
	p1.m_directionFacing = "Up";

	p2.m_name = "P2";
	p2.m_directionFacing = "Down";

	LilBuddyFamiliar lbf;
	lbf.m_name = "LBF";
	lbf.SetFollowTarget(&p1);

	lbf.Move();
	lbf.Attack();
	lbf.Collided(&e1);

	Fly fly;
	fly.m_name = "Fly";
	fly.SetFollowTarget(&e2);

	fly.Move();
	fly.Attack();
	fly.Collided(&e2);

	SpawnSpiderFamiliar ssf;
	ssf.m_name = "SSF";
	//ssf.SetFollowTarget(&e1); //<< Wont compile

	ssf.Move();
	ssf.Attack();
	ssf.Collided(&e2);
}