#include "Chapter10.h"

#include "Achievements.h"
#include "GameDefs.h"

void Chapter10::Run()
{
	//Acyclic Visitor
	std::cout << "*****Acyclic Visitor*****" << std::endl;
	ComponentVisitor visitor;
	PhysicsComponent pc;
	RenderComponent rc;
	VisitableComponent* vcFromRc = &rc;

	pc.Accept(visitor);
	vcFromRc->Accept(visitor);
	std::cout << std::endl;

	std::cout << "*****Cyclic Visitor*****" << std::endl;
	AchievementVisitor av;
	Inventory inventory;
	QuestLog questLog;
	for (int i = 0; i < 5; ++i)
	{
		inventory.items++;
		questLog.completedQuests++;
		inventory.Accept(av);
		questLog.Accept(av);
	}

}