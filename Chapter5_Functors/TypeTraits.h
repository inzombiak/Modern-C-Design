#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

//I copied this from Loki, removed everything I didnt need
#include "Typelists.h"

namespace Private
{
	typedef TYPELIST_4(unsigned char, unsigned short int, unsigned int, unsigned long int)
		StdUnsignedInts;
	typedef TYPELIST_4(signed char, short int, int, long int)
		StdSignedInts;
	typedef TYPELIST_3(bool, char, wchar_t)
		StdOtherInts;
	typedef TYPELIST_3(float, double, long double)
		StdFloats;

	template <class U> struct AddParameterType
	{
		typedef const U & Result;
	};

	template <class U> struct AddParameterType<U &>
	{
		typedef U & Result;
	};

	template <> struct AddParameterType<void>
	{
		typedef NullType Result;
	};

	template <typename T>
	struct IsFunctionPointerRaw
	{
		enum{ result = 0 };
	};

	template <typename T>
	struct IsFunctionPointerRaw<T(*)()>
	{
		enum { result = 1 };
	};

	template <typename T,
		typename P01>
	struct IsFunctionPointerRaw<T(*)(P01)>
	{
		enum { result = 1 };
	};

	template <typename T,
		typename P01, typename P02>
	struct IsFunctionPointerRaw<T(*)(
		P01, P02)>
	{
		enum { result = 1 };
	};

	template <typename T,
		typename P01, typename P02, typename P03>
	struct IsFunctionPointerRaw<T(*)(
		P01, P02, P03)>
	{
		enum { result = 1 };
	};

	template <typename T,
		typename P01, typename P02, typename P03, typename P04>
	struct IsFunctionPointerRaw<T(*)(
		P01, P02, P03, P04)>
	{
		enum { result = 1 };
	};

	template <typename T>
	struct IsMemberFunctionPointerRaw
	{
		enum{ result = 0 };
	};

	template <typename T, typename S>
	struct IsMemberFunctionPointerRaw<T(S::*)()>
	{
		enum { result = 1 };
	};

	template <typename T, typename S,
		typename P01>
	struct IsMemberFunctionPointerRaw<T(S::*)(P01)>
	{
		enum { result = 1 };
	};

	template <typename T, typename S,
		typename P01, typename P02>
	struct IsMemberFunctionPointerRaw<T(S::*)(
		P01, P02)>
	{
		enum { result = 1 };
	};

	template <typename T, typename S,
		typename P01, typename P02, typename P03>
	struct IsMemberFunctionPointerRaw<T(S::*)(
		P01, P02, P03)>
	{
		enum { result = 1 };
	};

	template <typename T, typename S,
		typename P01, typename P02, typename P03, typename P04>
	struct IsMemberFunctionPointerRaw<T(S::*)(
		P01, P02, P03, P04)>
	{
		enum { result = 1 };
	};
}

template <typename T>
class TypeTraits
{
private:

	template <class U> struct ReferenceTraits
	{
		enum { result = false };
		typedef U ReferredType;
	};

	template <class U> struct ReferenceTraits<U&>
	{
		enum { result = true };
		typedef U ReferredType;
	};

	template <class U> struct PointerTraits
	{
		enum { result = false };
		typedef NullType PointeeType;
	};

	template <class U> struct PointerTraits<U*>
	{
		enum { result = true };
		typedef U PointeeType;
	};

	template <class U> struct PointerTraits<U*&>
	{
		enum { result = true };
		typedef U PointeeType;
	};

	template <class U> struct PToMTraits
	{
		enum { result = false };
	};

	template <class U, class V> struct PToMTraits<U V::*>
	{
		enum { result = true };
	};

	template <class U, class V> struct PToMTraits<U V::*&>
	{
		enum { result = true };
	};

	template <class U> struct FunctionPointerTraits
	{
		enum{ result = Private::IsFunctionPointerRaw<U>::result };
	};

	template <typename U> struct PToMFunctionTraits
	{
		enum{ result = Private::IsMemberFunctionPointerRaw<U>::result };
	};

	template <class U> struct UnConst
	{
		typedef U Result;
		enum { isConst = 0 };
	};

	template <class U> struct UnConst<const U>
	{
		typedef U Result;
		enum { isConst = 1 };
	};

	template <class U> struct UnConst<const U&>
	{
		typedef U& Result;
		enum { isConst = 1 };
	};

	template <class U> struct UnVolatile
	{
		typedef U Result;
		enum { isVolatile = 0 };
	};

	template <class U> struct UnVolatile<volatile U>
	{
		typedef U Result;
		enum { isVolatile = 1 };
	};

	template <class U> struct UnVolatile<volatile U&>
	{
		typedef U& Result;
		enum { isVolatile = 1 };
	};

public:

	typedef typename UnConst<T>::Result
		NonConstType;

	typedef typename UnVolatile<typename UnConst<T>::Result>::Result
		UnqualifiedType;

	typedef typename ReferenceTraits<T>::ReferredType
		ReferredType;

	enum 
	{ 
		isConst = UnConst<T>::isConst 
	};

	enum 
	{
		isFunctionPointer = FunctionPointerTraits<
		typename ReferenceTraits<UnqualifiedType>::ReferredType >::result
	};
	enum 
	{
		isMemberFunctionPointer = PToMFunctionTraits<
		typename ReferenceTraits<UnqualifiedType>::ReferredType >::result
	};
	enum
	{
		isMemberPointer = PToMTraits<
		typename ReferenceTraits<UnqualifiedType>::ReferredType >::result ||
		isMemberFunctionPointer
	};
	enum 
	{
		isPointer = PointerTraits<
		typename ReferenceTraits<UnqualifiedType>::ReferredType >::result ||
		isFunctionPointer
	};

	enum 
	{
		isStdUnsignedInt = TL::IndexOf<Private::StdUnsignedInts, UnqualifiedType>::value >= 0 ||
						   TL::IndexOf<Private::StdUnsignedInts,
						   typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
	};
	enum 
	{
		isStdSignedInt	= TL::IndexOf<Private::StdSignedInts, UnqualifiedType>::value >= 0 ||
						  TL::IndexOf<Private::StdSignedInts,
						  typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
	};
	enum 
	{ 
		isUnsignedInt   = isStdUnsignedInt
	};
	enum 
	{
		isStdIntegral	= isStdUnsignedInt || isStdSignedInt ||
						  TL::IndexOf<Private::StdOtherInts, UnqualifiedType>::value >= 0 ||
						  TL::IndexOf<Private::StdOtherInts,
						  typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
	};

	enum 
	{
		isStdFloat		= TL::IndexOf<Private::StdFloats, UnqualifiedType>::value >= 0 ||
						  TL::IndexOf<Private::StdFloats,
						  typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
	};
	enum 
	{ 
		isStdArith		= isStdIntegral || isStdFloat 
	};

	typedef typename Chapter2::Select<isStdArith || isPointer || isMemberPointer, T,
			typename Private::AddParameterType<T>::Result>::Result
		ParameterType;

};

#endif