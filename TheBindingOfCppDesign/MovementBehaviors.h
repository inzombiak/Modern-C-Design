#ifndef MOVEMENT_BEHAVIORS_H
#define MOVEMENT_BEHAVIORS_H

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

#endif