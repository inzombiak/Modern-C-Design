#ifndef TYPELISTS_H
#define TYPELISTS_H

class NullType
{};

template<class T, class U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

#define TYPELIST_1(T1)					Typelist<T1, NullType>
#define TYPELIST_2(T1, T2)				Typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3)			Typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4)		Typelist<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5)\
										Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6)\
										Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7)\
										Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8)\
										Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>

#endif