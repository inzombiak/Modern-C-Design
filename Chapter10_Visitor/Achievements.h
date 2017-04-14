#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Visitor.h"

#include <iostream>

namespace Chapter10
{
	class BaseElement;
	class Inventory;
	class QuestLog;

	typedef CyclicVisitor<void, TYPELIST_3(BaseElement, Inventory, QuestLog)> AVisitor;

	

	class BaseElement
	{
	public:
		DEFINE_CYCLIC_VISITABLE(AVisitor);
	};

	class Inventory : public BaseElement
	{
	public:
		DEFINE_CYCLIC_VISITABLE(AVisitor);
		int items = 0;
	};

	class QuestLog : public BaseElement
	{
	public:
		DEFINE_CYCLIC_VISITABLE(AVisitor);
		int completedQuests = 0;
	};

	class AchievementVisitor : public AVisitor
	{
	public:
		void Visit(BaseElement& be)
		{

		}
		void Visit(Inventory& inventory)
		{
			std::cout << "Hoard Master Achievement: ";
			if (inventory.items >= 5)
				std::cout << "Congrats, you're a hoarder 10/10" << std::endl;
			else
				std::cout << "Get more items: " << inventory.items << "/5" << std::endl;
		}
		void Visit(QuestLog& questLog)
		{
			std::cout << "Quest-A-Lot Achievement: ";
			if (questLog.completedQuests >= 5)
				std::cout << "Congrats, go outside" << std::endl;
			else
				std::cout << "You should do more quests: " << questLog.completedQuests  << "/5"<< std::endl;
		}
	};
}

#endif