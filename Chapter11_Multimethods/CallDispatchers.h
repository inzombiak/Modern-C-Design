#ifndef CALL_DISPATCHERS_H
#define CALL_DISPATCHERS_H

#include "../Chapter5_Functors/Typelists.h"

/*
	
	Static Dispatcher

*/

template
<
	class Executor, //Callback object
	bool Symmetric, //Are the typelists symemtric?
	class BaseLhs,  //Base class of the types passed as left-hand arguments
	class TypesLhs, //Typelist for all left-hand side arguments
	class BaseRhs = BaseLhs, //Base class of the types passed as right-hand arguments
	class TypesRhs = TypesLhs,//Typelist for all right-hand side arguments
	class ResultType = void //Return type
>
class StaticDispatcher
{
	//Each StaticDispatcher tries the Head of the appropriate typelist
	//If it fails, the Tail is passed to the next one
	//We sort the typelist to avoid calling a function on a base class before reaching the derived type

	typedef typename TL::DerivedToFront<TypesLhs>::Result SortedTypesLhs;
	typedef typename SortedTypesLhs::Head HeadLhs;
	typedef typename SortedTypesLhs::Tail TailLhs;

	typedef typename TL::DerivedToFront<TypesRhs>::Result SortedTypesRhs;
	typedef typename SortedTypesRhs::Head HeadRhs;
	typedef typename SortedTypesRhs::Tail TailRhs;

	//"Hiddwen" struct used to swap arguments
	template <bool swapArgs, class SomeLhs, class SomeRhs>
	struct InvocationTraits
	{
		static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec)
		{
			return exec.Fire(lhs, rhs);
		}
	};

	template <class SomeLhs, class SomeRhs>
	struct InvocationTraits<true, SomeLhs, SomeRhs>
	{
		static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec)
		{
			return exec.Fire(rhs, lhs);
		}
	};

public:

	//Entry point, processes the left-hand side
	static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
	{
		
		//If HeadLhs and lhs are same type, dispatch the call and begin processing rhs
		//Otherwise recurse
		if (HeadLhs* p1 = dynamic_cast<HeadLhs*>(&lhs))
			return StaticDispatcher<Executor, Symmetric, BaseLhs, TypesLhs, BaseRhs, TypesRhs, ResultType>::DispatchRhs(*p1, rhs, exec);
		else
			return StaticDispatcher<Executor, Symmetric, BaseLhs, TailLhs, BaseRhs, TypesRhs, ResultType>::Go(lhs, rhs, exec);

	}
	//After a suitable left-hand side is found, this is called
	template <class SomeLhs>
	static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec)
	{
		//If HeadRhs and rhs are same type, fire the callback
		//Otherwise recurse
		if (HeadRhs* p2 = dynamic_cast<HeadRhs*>(&rhs))
		{
			//If we can swap, try swapping
			enum { swap = Symmetric && (TL::IndexOf<TypesRhs, HeadRhs>::value < TL::IndexOf<TypesLhs, SomeLhs>::value) };
			typedef InvocationTraits<swap, SomeLhs, HeadRhs> CallTraits;

			return CallTraits::DoDispatch(lhs, *p2, exec);
		}
		else
			return StaticDispatcher<Executor, Symmetric, SomeLhs, TypesLhs, BaseRhs, TailRhs, ResultType>::DispatchRhs(lhs, rhs, exec);

	}
};

//Specialization for NullType
template
<
	class Executor,
	bool Symmetric,
	class BaseLhs, 
	class BaseRhs,
	class TypesRhs,
	class ResultType
>
class StaticDispatcher <Executor, Symmetric, BaseLhs, NullType, BaseRhs, TypesRhs, ResultType>
{
public:
	static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
	{
		return exec.OnError(lhs, rhs);
	}
};

template
<
	class Executor,
	bool Symmetric,
	class BaseLhs,
	class TypesLhs,
	class BaseRhs,
	class ResultType
>
class StaticDispatcher <Executor, Symmetric, BaseLhs, TypesLhs, BaseRhs, NullType, ResultType>
{
public:
	static ResultType DispatchRhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
	{
		return exec.OnError(lhs, rhs);
	}
};

/*

	Logarithmic Dispatcher

	Note: The book recommends using a sorted vector for faster lookup, but I'm going to use a map for simplicity
*/

#include "../Chapter8_ObjectFactories/TypeInfo.h"

#include <map>


/*
	Basic Dispatcher

	Used as a base for other dispatchers
*/

template 
<
	class BaseLhs, //Base type of left-hand side
	class BaseRhs = BaseLhs, //Base type of right-hand side
	typename ResultType = void, //Return type
	typename CallbackType = ResultType(*)(BaseLhs&, BaseRhs&) //Callback function
>
class BasicDispatcher
{
	typedef std::pair<TypeInfo, TypeInfo> KeyType;
	typedef CallbackType MappedType;
	typedef std::map<KeyType, MappedType> MapType;
	MapType m_callbackMap;

public:

	//Add a new callback function
	template <class SomeLhs, class SomeRhs>
	void Add(CallbackType fun)
	{
		const KeyType key(typeid(SomeLhs), typeid(SomeRhs));
		m_callbackMap[key] = fun;
	}

	//Call callback
	ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
	{
		MapType::iterator it = m_callbackMap.find(KeyType(typeid(lhs), typeid(rhs)));
		if (it == m_callbackMap.end())
		{
			//TODO: ERROR POLICY
		}
		
		return (it->second)(lhs, rhs);
	}

};

/*
	Basic Fast Dispatcher

	Same as above excpet it uses a matrix for faster access
	*/

#include <vector>

//This macro must be called in each class that uses this dispatcher
#define IMPLEMENT_INDEXABLE_CLASS(SomeClass)\
	static int& GetClassIndexStatic()\
	{\
		static int index = -1;\
		return index;\
	}\
	virtual int& GetClassIndex()\
	{\
		assert(typeid(*this) == typeid(SomeClass));\
		return GetClassIndexStatic();\
	}

template
<
	class BaseLhs, //Base type of left-hand side
	class BaseRhs = BaseLhs, //Base type of right-hand side
	typename ResultType = void, //Return type
	typename CallbackType = ResultType(*)(BaseLhs&, BaseRhs&) //Callback function
>
class BasicFastDispatcher
{
	typedef std::vector<CallbackType> Row;
	typedef std::vector<Row> Matrix;
	Matrix m_callbacks;
	int m_nextIndex;
public:
	BasicFastDispatcher() : m_nextIndex(0) {}
	template <class SomeLhs, class SomeRhs>
	void Add(CallbackType pFun)
	{
		int& indexLhs = SomeLhs::GetClassIndexStatic();
		if (indexLhs < 0)
		{
			m_callbacks.resize(++m_nextIndex);
			indexLhs = m_callbacks.size() - 1;
		}
		else if (m_callbacks.size() <= indexLhs)
			m_callbacks.resize(indexLhs + 1);

		Row& thisRow = m_callbacks[indexLhs];
		int& indexRhs = SomeRhs::GetClassIndexStatic();
		if (indexRhs < 0)
		{
			m_callbacks.resize(++m_nextIndex);
			indexRhs = m_callbacks.size() - 1;
		}
		else if (m_callbacks.size() <= indexRhs)
			m_callbacks.resize(indexRhs + 1);

		thisRow[indexRhs] = pFun;

	}

	ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
	{
		int& indexLhs = lhs.GetClassIndex();
		int& indexRhs = rhs.GetClassIndex();

		if (indexLhs < 0 || indexRhs < 0 ||
			indexLhs >= m_callbacks.size() || indexRhs >= m_callbacks[indexLhs].size())

			return m_callbacks[indexLhs][indexRhs](lhs, rhs);
	}
};

/*
	Function dispatcher
*/
template
<
	class BaseLhs,
	class BaseRhs = BaseLhs,
	typename ResultType = void,
	template <class, class, class, class> class DispatcherBackend = BasicDispatcher //backend policy
>
class FnDispatcher
{
	//FnDispatcher uses a BasicDispatcher for actual calls
	DispatcherBackend<BaseLhs, BaseRhs, ResultType, ResultType(*)(BaseLhs&, BaseRhs&)> m_backEnd;

public:
	
	//Uses a trampoline function to extract and register base types
	//Note: The book uses a local class, but I've replaced it with a lambda because this is 2017

	//If types are symmetrix
	template<class ConcreteLhs, class ConcreteRhs, ResultType(*Callback)(ConcreteLhs&, ConcreteRhs&), bool Symmetric, typename std::enable_if<Symmetric>::type* = nullptr>
	void Add()
	{
		auto trampoline = [](BaseLhs& lhs, BaseRhs& rhs)
		{
			return Callback(dynamic_cast<ConcreteLhs&>(lhs), dynamic_cast<ConcreteRhs&>(rhs));
		};
		
		m_backEnd.Add<ConcreteLhs, ConcreteRhs>(trampoline);

		if (Symmetric)
		{
			auto trampolineR = [](BaseLhs& lhs, BaseRhs& rhs)
			{
				return Callback(dynamic_cast<ConcreteRhs&>(rhs), dynamic_cast<ConcreteLhs&>(lhs));
			};

			m_backEnd.Add<ConcreteLhs, ConcreteRhs>(trampolineR);
		}
	}

	//Otherwise
	template<class ConcreteLhs, class ConcreteRhs, ResultType(*Callback)(ConcreteLhs&, ConcreteRhs&), bool Symmetric, typename std::enable_if<!Symmetric>::type* = nullptr>
	void Add()
	{
		auto trampoline = [](BaseLhs& lhs, BaseRhs& rhs)
		{
			return Callback(dynamic_cast<ConcreteLhs&>(lhs), dynamic_cast<ConcreteRhs&>(rhs));
		};

		m_backEnd.Add<ConcreteLhs, ConcreteRhs>(trampoline);
	}


	//Call callback
	template<class ConcreteLhs, class ConcreteRhs>
	ResultType Go(ConcreteLhs& lhs, ConcreteRhs& rhs)
	{
		return m_backEnd.Go(dynamic_cast<BaseLhs&>(lhs), dynamic_cast<BaseRhs&>(rhs));
	}

};

/*
	Functor Dispatcher

	Uses functors for callbacks
*/
#include "../Chapter5_Functors/Functors.h"

template<class To, class From>
struct DynamicCaster
{
	static To& Cast(From& obj)
	{
		return dynamic_cast<To&>(obj);
	}
};

template 
<
	class BaseLhs, 
	class BaseRhs = BaseLhs, 
	typename ResultType = void,
	template <class, class, class, class> class DispatcherBackend = BasicDispatcher,
	template <class, class> class CastingPolicy = DynamicCaster>
class FunctorDispatcher
{
	typedef Functor<ResultType, TYPELIST_2(BaseLhs, BaseRhs)> FunctorType;
	DispatcherBackend<BaseLhs, BaseRhs, ResultType, FunctorType> m_backEnd;

public:

	template<class SomeLhs, class SomeRhs, class Fun>
	void Add(const Fun& fun)
	{
		typedef FunctorImpl<ResultType, TYPELIST_2(BaseLhs&, BaseRhs&)> FunctorImplType;
		class Adapter : public FunctorImplType
		{
			Fun m_fun;
			
			
		public:
			virtual FunctorImplType* DoClone() const
			{
				return new Adapter(m_fun);
			}
			virtual ResultType operator()(BaseLhs& lhs, BaseRhs& rhs)
			{
				return m_fun(CastingPolicy<SomeLhs, BaseLhs>::Cast(lhs), CastingPolicy<SomeRhs, BaseRhs>::Cast(rhs));
			}
			Adapter(const Fun& fun) : m_fun(fun) {}
		};

		m_backEnd.Add<SomeLhs, SomeRhs>(FunctorType(Adapter(fun)));
	}

	//template<class ConcreteLhs, class ConcreteRhs>
	ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
	{
		return m_backEnd.Go(lhs, rhs);
		//return m_backEnd.Go(CastingPolicy<BaseLhs, ConcreteLhs>::Cast(lhs), CastingPolicy<BaseRhs, ConcreteRhs>::Cast(rhs));
	}
};

#endif