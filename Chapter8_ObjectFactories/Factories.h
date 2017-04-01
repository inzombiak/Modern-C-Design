#ifndef FACTORIES_H
#define FACTORIES_H

#include "TypeInfo.h"

#include <map>
#include <iostream>

/*
	Taken from Loki

	Allows factories to use functors that cna pass arguements to constructors
*/
struct FactoryImplBase
{
	typedef EmptyType Param1;
	typedef EmptyType Param2;
};

template <typename AbstractProduct, typename Id, typename TList>
struct FactoryImpl;

template<typename AbstractProduct, typename Id>
struct FactoryImpl<AbstractProduct, Id, NullType>
	: public FactoryImplBase
{
	virtual ~FactoryImpl() {}
	virtual AbstractProduct* CreateObject(const Id & id) = 0;
};
template <typename AbstractProduct, typename Id, typename P1>
struct FactoryImpl<AbstractProduct, Id, TYPELIST_1(P1) >
	: public FactoryImplBase
{
	typedef typename TypeTraits<P1>::ParameterType Param1;
	virtual ~FactoryImpl() {}
	virtual AbstractProduct* CreateObject(const Id& id, Param1) = 0;
};

template<typename AbstractProduct, typename Id, typename P1, typename P2>
struct FactoryImpl<AbstractProduct, Id, TYPELIST_2(P1, P2) >
	: public FactoryImplBase
{
	typedef typename TypeTraits<P1>::ParameterType Param1;
	typedef typename TypeTraits<P2>::ParameterType Param2;
	virtual ~FactoryImpl() {}
	virtual AbstractProduct* CreateObject(const Id& id, Param1, Param2) = 0;
};

/*
	
	Default error policy
	Note: The book uses the code that is commented out, but since exception throwing isn't recommended for games
	I changed it to logging
*/

template <class IdentifierType, class ProductType>
class DefaultFactoryError
{

public:
	
	/*class Exception : public std::exception
	{
	public:

		Exception(const IdentifierType& unknownId) : m_unknownId(unknownId) {}
		
		virtual const char* what()
		{
			return "Unknown object type passed to Factory. ";
		}

		const IdentifierType& GetID()
		{
			return m_unknownId;
		}

	private:

		IdentifierType m_unknownId;

	};
	*/

protected:

	/*
	static ProductType* OnUnknownType(const IdenfierType& id)
	{
		throw Exception(id);
	}
	*/

	virtual ProductType* OnUnknownType(const IdentifierType& id)
	{
		//I know this requiers Id to implement the << operator, but whatever
		std::cout << "Invalid id/no callback for: " << id << std::endl;

		return NULL;
	}

};


/*

	Generic factory

	AbstactProduct		- Base class, e.g. "Shape" for "Circle", "Square" etc.
	IdentifierType		- A identifier type that has low or no collision (e.g. enum, unique string or a hash value) that can be coupled with a ProductCreator
	ProductCreator		- Removed to use functors
	CreatorParamTList	- Typelist of the parameters taken by creator functors
	FactoryErrorPolicy	- Policy used to handle errors on creation (invalid ID or ProductCreator)

	Can handle creators with up to 2 parameters
*/
template
<
	class AbstractProduct,
	typename IdentifierType,
	typename CreatorParamTList = NullType,
	template <typename, class>
		class FactoryErrorPolicy = DefaultFactoryError
>
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{

	typedef FactoryImpl<AbstractProduct, IdentifierType, CreatorParamTList> Impl;

	typedef typename Impl::Param1 Param1;
	typedef typename Impl::Param2 Param2;

	typedef Functor<AbstractProduct*, CreatorParamTList> ProductCreator;

public:
	
	bool Register(const IdentifierType& id, ProductCreator creator)
	{
		return m_callbacks.insert(std::pair<IdentifierType, ProductCreator>(id, creator)).second;
	}
	bool Unregister(const IdentifierType& id)
	{
		return m_callbacks.erase(id) == 1;
	}

	AbstractProduct* CreateObject(const IdentifierType& id)
	{
		typename CallbackMap::iterator it = m_callbacks.find(id);
		if (it != m_callbacks.end())
		{
			return (it->second)();
		}

		return OnUnknownType(id);
	}


	AbstractProduct* CreateObject(const IdentifierType& id, Param1 p1)
	{
		typename CallbackMap::iterator it = m_callbacks.find(id);
		if (it != m_callbacks.end())
		{
			return (it->second)(p1);
		}

		return OnUnknownType(id);
	}

	AbstractProduct* CreateObject(const IdentifierType& id, Param1 p1, Param2 p2)
	{
		typename CallbackMap::iterator it = m_callbacks.find(id);
		if (it != m_callbacks.end())
		{
			return (it->second)(p1, p2);
		}

		return OnUnknownType(id);
	}

private:

	typedef std::map<IdentifierType, ProductCreator> CallbackMap;
	CallbackMap m_callbacks;

};


/*

	Clone factory
	Returns a clone of the object
*/


template
<
	class AbstractProduct,
	typename ProductCreator = AbstractProduct* (*)(AbstractProduct*),
	template <typename, class>
		class FactoryErrorPolicy = DefaultFactoryError
>
class CloneFactory : public FactoryErrorPolicy<TypeInfo, AbstractProduct>
{

public:
	bool Register(const TypeInfo& id, ProductCreator creator)
	{
		return m_callbacks.insert(std::pair<TypeInfo, ProductCreator>(id, creator)).second;
	}
	bool Unregister(const TypeInfo& id)
	{
		return m_callbacks.erase(id) == 1;
	}

	AbstractProduct* CreateObject(AbstractProduct* model)
	{
		if (model == NULL)
			return NULL;

		typename CallbackMap::iterator it = m_callbacks.find(typeid(*model));
		if (it != m_callbacks.end())
		{
			return (it->second)(model);
		}

		return OnUnknownType(typeid(*model));
	}

private:
	typedef std::map<TypeInfo, ProductCreator> CallbackMap;
	CallbackMap m_callbacks;

};

#endif