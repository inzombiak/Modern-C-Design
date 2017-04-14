#ifndef VISITOR_H
#define VISITOR_H

/*
	Acyclic Visitor
*/
#define DEFINE_VISITIBLE() \
	virtual ReturnType Accept(BaseVisitor& guest)\
	{return AcceptImpl(*this, guest);}

class BaseVisitor
{
public:
	virtual ~BaseVisitor() {}
};

template<class T, typename R = void>
class Visitor
{
public:
	typedef R ReturnType;
	virtual ReturnType Visit(T&) = 0;
};

template <typename R = void>
class BaseVisitable
{
public:
	typedef R ReturnType;
	virtual ~BaseVisitable(){}
	virtual ReturnType Accept(BaseVisitor&) = 0;
protected:
	template <class T>
	static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)
	{
		if (Visitor<T, R>* p = dynamic_cast<Visitor<T, R>*>(&guest))
		{
			return p->Visit(visited);
		}
		return ReturnType();
	}
};

#include "../Chapter3_Typelists/Typelists.h" 

template <class Head, class Tail, typename R>
class Visitor<TL::Typelist<Head, Tail>, R>
	: public Visitor<Head, R>, public Visitor<Tail, R>
{
public:
	typedef R ReturnType;
	using Visitor<Head, R>::Visit;
};

template <class Head, typename R>
class Visitor<TL::Typelist<Head, NullType>, R> : public Visitor<Head, R>
{
public:
	typedef R ReturnType;
	using Visitor<Head, R>::Visit;
};

template <typename R, class TList>
class CyclicVisitor : public Visitor<TList, R>
{
public:
	typedef R ReturnType;

	template <class Visited>
	ReturnType GenericVisit(Visited& host)
	{
		Visitor<Visited, ReturnType>& subObj = *this;
		return subObj.Visit(host);
	}
};

#define DEFINE_CYCLIC_VISITABLE(SomeVisitor) \
    virtual SomeVisitor::ReturnType Accept(SomeVisitor& guest) \
    { return guest.GenericVisit(*this); }


#endif