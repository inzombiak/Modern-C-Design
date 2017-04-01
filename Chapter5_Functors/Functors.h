#ifndef FUNCTORS_H
#define FUNCTORS_H

#include "Typelists.h"
#include "TypeTraits.h"

#include <memory>
#include <assert.h>
/*
	The book goes up to 2 parameters per functor.
	I'm going to double that offer and do 4.

	Note: A lot of the stuff here can be replaced with std::function and lambdas
*/

template <typename R>
struct FunctorImplBase
{
	inline FunctorImplBase() {}

	typedef R ResultType;
	typedef FunctorImplBase<R> FunctorImplBaseType;

	typedef EmptyType Param1;
	typedef EmptyType Param2;
	typedef EmptyType Param3;
	typedef EmptyType Param4;

	virtual ~FunctorImplBase()
	{}

	virtual FunctorImplBase* DoClone() const = 0;

	template <class U>
	static U* Clone(U* pObj)
	{
		if (!pObj) return 0;
		U* pClone = static_cast<U*>(pObj->DoClone());
		assert(typeid(*pClone) == typeid(*pObj));
		return pClone;
	}

};

//Base
template <typename ResultType, class TList>
class FunctorImpl;

//0 parameters
template <typename R>
class FunctorImpl<R, NullType> : public FunctorImplBase<R>
{
public:
	typedef R ResultType;
	virtual R operator()() = 0;
};

//1 parameter
template <typename R, typename P1>
class FunctorImpl<R, TYPELIST_1(P1)> : public FunctorImplBase<R>
{
public:
	typedef R ResultType;
	typedef typename TypeTraits<P1>::ParameterType  Param1;

	virtual R operator()(Param1) = 0;
};

//2 parameters
template <typename R, typename P1, typename P2>
class FunctorImpl<R, TYPELIST_2(P1, P2)> : public FunctorImplBase<R>
{
public:
	typedef R ResultType;
	typedef typename TypeTraits<P1>::ParameterType Param1;
	typedef typename TypeTraits<P2>::ParameterType Param2;
	virtual R operator()(Param1, Param2) = 0;
};

//3 parameters
template <typename R, typename P1, typename P2, typename P3>
class FunctorImpl<R, TYPELIST_3(P1, P2, P3)> : public FunctorImplBase<R>
{
public:
	typedef R ResultType;
	typedef typename TypeTraits<P1>::ParameterType Param1;
	typedef typename TypeTraits<P2>::ParameterType Param2;
	typedef typename TypeTraits<P3>::ParameterType Param3;
	virtual R operator()(Param1, Param2, Param3) = 0;
};

//4 parameters
template <typename R, typename P1, typename P2, typename P3, typename P4>
class FunctorImpl<R, TYPELIST_4(P1, P2, P3, P4)> : public FunctorImplBase<R>
{
public:
	typedef R ResultType;
	typedef typename TypeTraits<P1>::ParameterType Param1;
	typedef typename TypeTraits<P2>::ParameterType Param2;
	typedef typename TypeTraits<P3>::ParameterType Param3;
	typedef typename TypeTraits<P4>::ParameterType Param4;
	virtual R operator()(Param1, Param2, Param3, Param4) = 0;
};

template<typename R, class TList>
class Functor
{

public:
	typedef FunctorImpl<R, TList> Impl;
	typedef TList ParamList;
	typedef R ResultType;
	typedef typename Impl::Param1 Param1;
	typedef typename Impl::Param2 Param2;
	typedef typename Impl::Param3 Param3;
	typedef typename Impl::Param4 Param4;


	Functor() : m_spImpl(0){};
	Functor(const Functor& rhs) : m_spImpl(Impl::Clone(rhs.m_spImpl.get()))
	{}

	Functor(std::auto_ptr<Impl> spImpl) : m_spImpl(spImpl)
	{}

	template<typename Fun>
	Functor(const Fun& fun) : m_spImpl(new FunctorHandler<Functor, Fun>(fun))
	{

	}

	template<class PtrObj, typename MemFn>
	Functor(const PtrObj& p, MemFn memFn) : m_spImpl(new MemFunHandler<Functor, PtrObj, MemFn>(p, memFn))
	{

	}
	R operator()()
	{
		return (*m_spImpl)();
	}
	R operator()(Param1 p1)
	{
		return (*m_spImpl)(p1);
	}
	R operator()(Param1 p1, Param2 p2)
	{
		return (*m_spImpl)(p1, p2);
	}
	R operator()(Param1 p1, Param2 p2, Param3 p3)
	{
		return (*m_spImpl)(p1, p2, p3);
	}
	R operator()(Param1 p1, Param2 p2, Param3 p3, Param3 p4)
	{
		return (*m_spImpl)(p1, p2, p3, p4);
	}

private:
	typedef FunctorImpl<R, TList> Impl;
	std::auto_ptr<Impl> m_spImpl;
};

template <class ParentFunctor, typename Fun>
class FunctorHandler : public ParentFunctor::Impl
{
	typedef typename ParentFunctor::Impl Base;

public:
	typedef typename Base::ResultType ResultType;
	typedef typename Base::Param1 Param1;
	typedef typename Base::Param2 Param2;
	typedef typename Base::Param3 Param3;
	typedef typename Base::Param4 Param4;

	FunctorHandler(const Fun& fun) : m_fun(fun) {}
	FunctorHandler* DoClone() const
	{
		return new FunctorHandler(*this);
	}

	ResultType operator()()
	{
		return m_fun();
	}
	ResultType operator()(Param1 p1)
	{
		return m_fun(p1);
	}
	ResultType operator()(Param1 p1, Param2 p2)
	{
		return m_fun(p1, p2);
	}
	ResultType operator()(Param1 p1, Param2 p2, Param3 p3)
	{
		return m_fun(p1, p2, p3);
	}
	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		return m_fun(p1, p2, p3, p4);
	}

private:
	Fun m_fun;

};

template <class ParentFunctor, typename PointerToObj, typename PointerToMemFn>
class MemFunHandler : public FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParamList>
{
public:
	typedef typename ParentFunctor::ResultType ResultType;
	typedef typename ParentFunctor::Param1 Param1;
	typedef typename ParentFunctor::Param2 Param2;
	typedef typename ParentFunctor::Param3 Param3;
	typedef typename ParentFunctor::Param4 Param4;

	MemFunHandler(const PointerToObj& pObj, PointerToMemFn pMemFn) : m_pObj(pObj), m_pMemFn(pMemFn)
	{

	}

	MemFunHandler* DoClone() const
	{
		return new MemFunHandler(*this);
	}

	ResultType operator()()
	{
		return ((*m_pObj).*m_pMemFn)();
	}

	ResultType operator()(Param1 p1)
	{
		return ((*m_pObj).*m_pMemFn)(p1);
	}

	ResultType operator()(Param1 p1, Param2 p2)
	{
		return ((*m_pObj).*m_pMemFn)(p1, p2);
	}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3)
	{
		return ((*m_pObj).*m_pMemFn)(p1, p2, p3);
	}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4);
	}

private:
	PointerToObj m_pObj;
	PointerToMemFn m_pMemFn;

};


template <class Fctor> struct BinderFirstTraits;

template <typename R, class TList>
struct BinderFirstTraits< Functor<R, TList> >
{
	typedef Functor<R, TList> OriginalFunctor;

	typedef typename TL::Erase<TList, typename TL::TypeAt<TList, 0>::Result>
		::Result
		ParamList;

	typedef typename TL::TypeAt<TList, 0>::Result OriginalParam1;

	typedef Functor<R, ParamList> BoundFunctorType;

	typedef typename BoundFunctorType::Impl Impl;

};

template<class T>
struct BinderFirstBoundTypeStorage;

template<class T>
struct BinderFirstBoundTypeStorage
{
	typedef typename TypeTraits<T>::ParameterType RefOrValue;
};

template <typename R, class TList>
struct BinderFirstBoundTypeStorage<Functor<R, TList>>
{
	typedef Functor<R, TList> OriginalFunctor;
	typedef const typename TypeTraits<OriginalFunctor>::ReferredType RefOrValue;
};

template<class Incoming>
class BinderFirst : public BinderFirstTraits<Incoming>::Impl
{
	typedef typename BinderFirstTraits<Incoming>::Impl Outgoing;
	typedef typename Incoming::ResultType ResultType;
	typedef typename BinderFirstBoundTypeStorage<typename BinderFirstTraits<Incoming>::OriginalParam1>::RefOrValue
		BoundTypeStorage;
	typedef typename Incoming::Param1 BoundType;
	typedef typename Incoming::Param2 OutParam1;
	typedef typename Incoming::Param3 OutParam2;
	typedef typename Incoming::Param4 OutParam3;
	typedef EmptyType OutParam4;

public:
	BinderFirst(const Incoming& fun, BoundType bound) : m_fun(fun), m_bound(bound)
	{}
	BinderFirst* DoClone() const
	{
		return new BinderFirst(*this);
	}
	ResultType operator()()
	{
		return m_fun(m_bound);
	}
	ResultType operator()(OutParam1 p1)
	{
		return m_fun(m_bound, p1);
	}
	ResultType operator()(OutParam1 p1, OutParam2 p2)
	{
		return m_fun(m_bound, p1, p2);
	}
	ResultType operator()(OutParam1 p1, OutParam1 p2, OutParam2 p3)
	{
		return m_fun(m_bound, p1, p2, p3);
	}
	ResultType operator()(OutParam1 p1, OutParam1 p2, OutParam2 p3, OutParam4 p4)
	{
		return m_fun(m_bound, p1, p2, p3, p4);
	}

private:
	Incoming m_fun;
	BoundTypeStorage m_bound;
};

template <class Fctor>
typename BinderFirstTraits<Fctor>::BoundFunctorType BindFirst(const Fctor& fun, typename Fctor::Param1 bound)
{
	typedef typename BinderFirstTraits<Fctor>::BoundFunctorType Outgoing;

	return Outgoing(std::auto_ptr<typename Outgoing::Impl>(new BinderFirst<Fctor>(fun, bound)));
}

template <typename Fun1, typename Fun2>
class Chainer : public Fun2::Impl
{
	typedef Fun2 Base;

public:
	typedef typename Base::ResultType ResultType;
	typedef typename Base::Param1 Param1;
	typedef typename Base::Param2 Param2;
	typedef typename Base::Param3 Param3;
	typedef typename Base::Param4 Param4;


	Chainer(const Fun1& fun1, const Fun2& fun2) : m_f1(fun1), m_f2(fun2) {}
	Chainer* DoClone() const
	{
		return new Chainer(*this);
	}
	ResultType operator()()
	{
		return m_f1(), m_f2();
	}

	ResultType operator()(Param1 p1)
	{
		return m_f1(p1), m_f2(p1);
	}

	ResultType operator()(Param1 p1, Param2 p2)
	{
		return m_f1(p1, p2), m_f2(p1, p2);
	}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3)
	{
		return, m_f1(p1, p2, p3); , m_f2(p1, p2, p3);
	}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		return m_f1(p1, p2, p3, p4), m_f2(p1, p2, p3, p4);
	}


private:
	Fun1 m_f1;
	Fun2 m_f2;
};

template <class Fun1, class Fun2>
Fun2 Chain(
	const Fun1& fun1,
	const Fun2& fun2)
{
	return Fun2(std::auto_ptr<typename Fun2::Impl>(
		new Chainer<Fun1, Fun2>(fun1, fun2)));
}

#endif