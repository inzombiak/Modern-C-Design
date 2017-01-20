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


/*
	Modifiers
*/
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

/*
	Trajectories
*/
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

/*
	Attacks
*/
template
<
	class ModifierPolicy,
	class TrajectoryPolicy
>
struct SpawnEntity : public ModifierPolicy , public TrajectoryPolicy
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

/*
	Collisions
*/
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

/*
	Movement
*/
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
		std::cout << "moving randomly" << std::endl;
	}
};

template
<
	typename AttackPolicy,
	typename CollisionPolicy,
	typename MovementPolicy
>
class Familiar : public CollisionPolicy , public MovementPolicy
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
typedef Familiar<LilBuddyAttack, LBShootOnCollision, Follow<Player>>				LilBuddyFamiliar;
typedef	Familiar<OrbitingFlyAttack, FlyOnCollision,	 Follow<Enemy>>					Fly;
typedef Familiar<SpawnSpiderAttack, SpawnSpiderOnCollision, RandomMovement>			SpawnSpiderFamiliar;

int main()
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
	//ssf.SetFollowTarget(&e1);

	ssf.Move();
	ssf.Attack();
	ssf.Collided(&e2);

	return 0;
}