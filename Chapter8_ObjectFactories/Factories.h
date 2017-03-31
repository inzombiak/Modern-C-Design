#ifndef FACTORIES_H
#define FACTORIES_H

#include <map>

/*
	Generic factory

	AbstactProduct - Base class, e.g. "Shape" for "Circle", "Square" etc.
	IdentifierType - A identifier type that has low or no collision (e.g. enum, unique string or a hash value) that can be coupled with a ProductCreator
	ProductCreator - A function or functor (see Chapter 5) that acts as the creator of the object


*/
template
<
	class AbstractProduct,
	typename IdentifierType,
	typename ProductCreator,
>
class Factory
{

public:
	
	bool Register(const IdentifierType& id, ProductCreator creator)
	{
		return m_callbacks.insert(std::pair<IdentifierType, ProductCreator>(id, creator))
	}
	bool Unregister(const IdentifierType& id)
	{
		return m_callbacks.erase(id) == 1;
	}

	AbstractProduct* CreateObject(const IdentifierType& id)
	{
		typename CallbackMap::const_iterator it = m_callbacks.find(id);
		if (it != m_callbacks.end())
		{
			return (it->second)();
		}


	}



private:
	typedef std::map<IdentifierType, ProductCreator> CallbackMap;
	CallbackMap m_callbacks;
};

#endif