#ifndef CH_2_3_H
#define CH_2_3_h

/*
2.3. Local Classes
A class defined locally.
Book says this will be valuable later, so check later chapters for what this is good for

TODO: Add reference
*/

namespace Chapter2
{
	namespace Ch2_3
	{
		class Global
		{
		public:
			Global(int n)
			{
				m_n = n;
			}

			virtual void Run();
		protected:
			int m_n;
		};

		Global* MakeALocalGlobal(int n);
		void Run();
	}
}

#endif