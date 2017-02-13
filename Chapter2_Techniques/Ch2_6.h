#ifndef CH_2_6_H
#define CH_2_6_H

/*
2.6. Type Selection
Selects T or U based on valeu of flag
See Typelitsts (Chapter 3) for use
*/

namespace Chapter2
{
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
}

#endif