#ifndef TYPELISTS_H
#define TYPELISTS_H

//Uses 2.6 and 2.7
#include "../Chapter2_Techniques/Ch2_5.h"
#include "../Chapter2_Techniques/Ch2_6.h"
#include "../Chapter2_Techniques/Ch2_7.h"
class NullType
{};

struct EmptyType
{};


namespace TL
{


	//3.2 Typelist
	//Typelist definition
	//NullType is used to end a typelist and is needed for opertation to work
	template<class T, class U>
	struct Typelist
	{
		typedef T Head;
		typedef U Tail;
	};

	//3.5 Calculating Length
	//Returns the length (number of elements) of a typelist
	template<class TList>
	struct Length;

	template<>
	struct Length<NullType>
	{
		static const int value = 0;
	};

	template<class T, class U>
	struct Length<Typelist<T, U> >
	{
		static const int value = 1 + Length<U>::value;
	};

	//3.6 Indexed Access
	//Returns type at index, or compile-time error for out-of-bounds error
	//TODO: Improve out-of-bounds effor message with static assert
	template <class TList, unsigned int index>
	struct TypeAt;

	template <class Head, class Tail>
	struct TypeAt<Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template <class Head, class Tail, unsigned int i>
	struct TypeAt<Typelist<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};

	template <class TList, unsigned int index,
		typename DefaultType = NullType>
	struct TypeAtNonStrict
	{
		typedef DefaultType Result;
	};

	template <class Head, class Tail, typename DefaultType>
	struct TypeAtNonStrict<Typelist<Head, Tail>, 0, DefaultType>
	{
		typedef Head Result;
	};

	template <class Head, class Tail, unsigned int i, typename DefaultType>
	struct TypeAtNonStrict<Typelist<Head, Tail>, i, DefaultType>
	{
		typedef typename
			TypeAtNonStrict<Tail, i - 1, DefaultType>::Result Result;
	};

	//3.7 Searching Typelists
	//Returns index if the type T is in typelist, otherwise -1
	template <class TList, class T> struct IndexOf;

	template <class T>
	struct IndexOf<NullType, T>
	{
		static const int value = -1;
	};

	template <class T, class Tail>
	struct IndexOf<Typelist<T, Tail>, T>
	{
		static const int value = 0;
	};

	template <class Head, class Tail, class T>
	struct IndexOf<Typelist<Head, Tail>, T>
	{
	private:
		static const int temp = IndexOf<Tail, T>::value;
	public:
		static const int value = (temp == -1 ? -1 : 1 + temp);
	};

	//3.8 Appending to Typelists
	//Appendeds a type to a typelist and returns a new typelist
	template<class TList, class T> struct Append;

	template<>
	struct Append<NullType, NullType>
	{
		typedef NullType Result;
	};

	template<class T>
	struct Append<NullType, T>
	{
		typedef Typelist<T, NullType> Result;
	};

	template<class Head, class Tail>
	struct Append<NullType, Typelist<Head, Tail> >
	{
		typedef Typelist<Head, Tail> Result;
	};

	template<class Head, class Tail, class T>
	struct Append<Typelist<Head, Tail>, T>
	{
		typedef Typelist<Head, typename Append<Tail, T>::Result> Result;
	};

	//3.9 Erasing a Type from a Typelist
	//Erases first occurence of type T from typelist, if it exists
	template <class TList, class T> struct Erase;
	template <class T>                        
	struct Erase<NullType, T>
	{
	    typedef NullType Result;
	};
	
	template <class T, class Tail>             
	struct Erase<Typelist<T, Tail>, T>
	{
	    typedef Tail Result;
	};
	
	template <class Head, class Tail, class T>
	struct Erase<Typelist<Head, Tail>, T>
	{
	    typedef Typelist<Head, typename Erase<Tail, T>::Result> Result;
	};

	//3.10 Erasing Duplicates
	//Erases all instaces of type T from typelist
	template <class TList, class T> struct EraseAll;

	template <class T>
	struct EraseAll<NullType, T>
	{
	    typedef NullType Result;
	};

	template <class T, class Tail>
	struct EraseAll<Typelist<T, Tail>, T>
	{
	        typedef typename EraseAll<Tail, T>::Result Result;
	};

	template <class Head, class Tail, class T>
	struct EraseAll<Typelist<Head, Tail>, T>
	{
		typedef Typelist<Head, typename EraseAll<Tail, T>::Result> Result;
	};

	//Removes all duplicates from a typelist
	template <class TList> struct NoDuplicates;
	
	 template <> struct NoDuplicates<NullType>
	 {
	     typedef NullType Result;
	 };
	
	 template <class Head, class Tail>
	 struct NoDuplicates< Typelist<Head, Tail> >
	 {
	 private:
		typedef typename NoDuplicates<Tail>::Result L1;
	    typedef typename Erase<L1, Head>::Result L2;
	 public:
	    typedef Typelist<Head, L2> Result;
	 };

	//3.11 Replacing an Element in a Typelist
	//Replaces first occurence of T with U, if it is in the typelist
	template <class TList, class T, class U> struct Replace;

	template <class T, class U>
	struct Replace<NullType, T, U>
	{
	    typedef NullType Result;
	};
	
	template <class T, class Tail, class U>
	struct Replace<Typelist<T, Tail>, T, U>
	{
	    typedef Typelist<U, Tail> Result;
	};
	
	template <class Head, class Tail, class T, class U>
	struct Replace<Typelist<Head, Tail>, T, U>
	{
	    typedef Typelist<Head, typename Replace<Tail, T, U>::Result> Result;
	};

	//Replaces all occurences of T with U, if there are any
	template <class TList, class T, class U> struct ReplaceAll;

	template <class T, class U>
	struct ReplaceAll<NullType, T, U>
	{
		typedef NullType Result;
	};

	template <class T, class Tail, class U>
	struct ReplaceAll<Typelist<T, Tail>, T, U>
	{
		typedef Typelist<U, typename ReplaceAll<Tail, T, U>::Result > Result;
	};

	template <class Head, class Tail, class T, class U>
	struct ReplaceAll<Typelist<Head, Tail>, T, U>
	{
		typedef Typelist<Head, typename Replace<Tail, T, U>::Result> Result;
	};

	//3.12 Partially Ordering Typelist

	//Returns the type that is most derived from T
	template <class TList, class T> struct MostDerived;
	
	template <class T>
	struct MostDerived<NullType, T>
	{
	    typedef T Result;
	};
	
	template <class Head, class Tail, class T>
	struct MostDerived<Typelist<Head, Tail>, T>
	{
	private:
	    typedef typename MostDerived<Tail, T>::Result Candidate;
	public:
		typedef typename Chapter2::Select<SUPERSUBCLASS(Candidate, Head), Head, Candidate>::Result Result;
	};

	//Orders a typelist from most derived to least						   
	template <class TList> struct DerivedToFront;

	template <>
	struct DerivedToFront<NullType>
	{
		typedef NullType Result;
	};

	template <class Head, class Tail>
	struct DerivedToFront< Typelist<Head, Tail> >
	{
	private:
		typedef typename MostDerived<Tail, Head>::Result TheMostDerived;
		typedef typename Replace<Tail, TheMostDerived, Head>::Result Temp;
		typedef typename DerivedToFront<Temp>::Result L;
	public:
		typedef Typelist<TheMostDerived, L> Result;
	};

	//Reverses a typelist, used for DerviedToBack
	template <class TList> struct Reverse;

	template <>
	struct Reverse<NullType>
	{
		typedef NullType Result;
	};

	template <class Head, class Tail>
	struct Reverse< Typelist<Head, Tail> >
	{
		typedef typename Append<typename Reverse<Tail>::Result, Head>::Result Result;
	};


	//Orders a typelist from least derived to most
	template <class TList> struct DerivedToBack;

	template <>
	struct DerivedToBack<NullType>
	{
		typedef NullType Result;
	};

	template <class Head, class Tail>
	struct DerivedToBack< Typelist<Head, Tail> >
	{
	private:
		typedef typename MostDerived<Tail, Head>::Result TheMostDerived;
		typedef typename Replace<Tail, TheMostDerived, Head>::Result Temp;
		typedef typename DerivedToFront<Temp>::Result L;
	public:
		typedef typename Reverse<Typelist<TheMostDerived, L> >::Result Result;
	};

	
}

//3.3 Linearizing Typelist Creation
//Some defines to help with creating different sized typelists
#define TYPELIST_1(T1)					TL::Typelist<T1, NullType>
#define TYPELIST_2(T1, T2)				TL::Typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3)			TL::Typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4)		TL::Typelist<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5)\
										TL::Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6)\
										TL::Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7)\
										TL::Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8)\
										TL::Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>

#endif