#include "Chapter2.h"

#include "Ch2_1.h"
#include "Ch2_3.h"
#include "Ch2_4.h"
#include "Ch2_5.h"
#include "Ch2_7.h"
#include "Ch2_10.h"

/*
Chapter 2 covers generic programming techniques.
I don't know if I'll be able to come up with a
theme for these, so I'll seperate them into sections and if I
come up with an idea merge them.
Some of the ideas may not fit into game dev, but I'm going to do them anyway.



*/

/*
2.2. Partial Template Specialization
*/

/*
End of 2.2.
*/

/*
2.6. Type Selection
TODO: FIGURE IT OUT
*/
template <bool flag, typename T, typename U>
struct Select
{
	typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	typedef U Result;
};
/*
End 2.6.
*/
/*
2.8. A Wrapper Around std::type_info
TODO: copy paste?
*/


/*
End 2.8.
*/

/*
2.9. NullType and EmptyType
*/

/*
End 2.9.
*/

void Chapter2::Run()
{
	/*
	2.1
	*/
	Chapter2::Ch2_1::Run();
	/*
	2.3
	*/
	Chapter2::Ch2_3::Run();
	/*
	2.4
	*/
	Chapter2::Ch2_4::Run();
	/*
	2.5
	*/
	Chapter2::Ch2_5::Run();
	/*
	2.7
	*/
	Chapter2::Ch2_7::Run();

	/*
	2.10
	*/
	Chapter2::Ch2_10::Run();

}