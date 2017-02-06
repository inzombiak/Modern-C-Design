#include "Ch2_3.h"

#include <iostream>

void Chapter2::Ch2_3::Global::Run()
{
	std::cout << "I'm global number: " << m_n << std::endl;
}

Chapter2::Ch2_3::Global* Chapter2::Ch2_3::MakeALocalGlobal(int n)
{
	class Local : public Global
	{
	public:
		Local(int n) : Global(n)
		{

		}

		virtual void Run()
		{
			std::cout << "I'm local number: " << m_n << std::endl;
		}

		void TryMeFromMain()
		{

		}
	};

	return new Local(n);
}

void Chapter2::Ch2_3::Run()
{
	std::cout << std::endl;
	std::cout << "***** 2.3 *****" << std::endl;

	Global* global1;
	Global* global2 = new Global(2);

	global1 = MakeALocalGlobal(1);
	global1->Run();
	//global->TryMeFromMain(); //NOPE
	global2->Run();

	delete global1;
	delete global2;
}