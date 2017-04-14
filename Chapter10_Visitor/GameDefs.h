#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include "Visitor.h"

#include <iostream>

namespace Chapter10
{
	class VisitableComponent : public BaseVisitable<>
	{
	public:
		DEFINE_VISITIBLE();
	};

	class PhysicsComponent : public VisitableComponent
	{
	public:
		DEFINE_VISITIBLE();
	};

	class RenderComponent : public VisitableComponent
	{
	public:
		DEFINE_VISITIBLE();
	};

	class ComponentVisitor :
		public BaseVisitor,
		public Visitor<VisitableComponent>,
		public Visitor<PhysicsComponent>,
		public Visitor<RenderComponent>
	{
	public:
		void Visit(VisitableComponent&)
		{
			std::cout << " Visit(VisitableComponent&)" << std::endl;
		}
		void Visit(PhysicsComponent&)
		{
			std::cout << " Visit(PhysicsComponent&)" << std::endl;
		}
		void Visit(RenderComponent&)
		{
			std::cout << " Visit(RenderComponent&)" << std::endl;
		}
	};
}


#endif