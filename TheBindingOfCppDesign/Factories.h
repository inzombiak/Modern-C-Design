#ifndef FACTORIES_H
#define FACTORIES_H

#include "Attacks.h"
#include "Trajectories.h"
#include "Modifieres.h"

#include "Utilities.h"
#include "Typelists.h"
#include "TypeTraits.h"

typedef TYPELIST_3(NullModifier, PoisonModifier, FireModifier) MODIFIERS;
typedef TYPELIST_3(NullTrajectory, ReverseTrajectory, SameTrajectory) TRAJECTORIES;
//typedef TYPELIST_3(SpawnEntity<>, ShootAndCollide, Collide) ATTACKS

#endif