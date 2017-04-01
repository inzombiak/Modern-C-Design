#ifndef C_2_5_H
#define C_2_5_H

#include <string>

/*
2.5. Type-to-Type Mapping

For this example we assume we have 2 types of books
1. Unreadable - displays what looks like gibberish when "Read" is invoked.
2. Readable - displays text and invokes a function when "Read" is invoked.

Also assume that we need to pass the ID of function that is called through "Read"
to the constructor.

This mapping allows us to specialzie the construction of the object.
In this case it allows us to pass the id of a function to call to Readable book
*/


namespace Chapter2
{
	template <typename T>
	struct Type2Type
	{
		typedef T OriginalType;
	};

	namespace Ch2_5
	{
		class UnreadableBook
		{
		public:
			UnreadableBook(std::string textOnRead)
			{
				m_text = textOnRead;
			}

			virtual void Read();
		protected:
			std::string m_text;
		};

		//Callback ID could be replaced with a funciton ptr or lambda in real code
		class ReadableBook : public UnreadableBook
		{
		public:

			ReadableBook(int callBackID, std::string textOnRead) : UnreadableBook(textOnRead)
			{
				m_callBackID = callBackID;
			}

			virtual void Read();

		private:
			int m_callBackID;
		};

		//If its unreadble this is called
		template<class T, class A>
		T* Create(const A& arg, Type2Type<T>)
		{
			return new T(arg);
		}

		//Otherwise this is
		template<class A>
		ReadableBook* Create(const A& arg, Type2Type<ReadableBook>)
		{
			/*
			Complex code that chooses appropriate text
			based on the function callback ID provided in arg
			*/
			std::string text = "Guess you're not completely illiterate, looks like you can understand " + std::to_string(arg) + " words";

			return new ReadableBook(arg, text);
		}

		void Run();
	}
}


#endif