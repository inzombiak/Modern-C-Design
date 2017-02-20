#ifndef TRAJECTORIES_H
#define TRAJECTORIES_H

#include <iostream>

class NullTrajectory
{
public:
	static void Spawn()
	{
		printf("Spawn in place \n");
	}
};

class ReverseTrajectory
{
public:
	static void Spawn()
	{
		printf("Spawn in opposite direction \n");
	}
};

class SameTrajectory
{
public:
	static void Spawn()
	{
		printf("Spawn in same direction  \n");
	}
};

#endif