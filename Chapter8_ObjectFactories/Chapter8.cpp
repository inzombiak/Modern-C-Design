#include "Chapter8.h"
#include "Factories.h"
#include "Singleton.h"

#include <string>
#include <iostream>

/*

	Basic component factory
	I changed the books implementation to Loki's since it supports parameters.

*/

enum ComponentType
{
	Render,
	Physics,
	Inventory,
};

class IComponent
{
public:
	virtual ~IComponent() {};

	void SetOwnerName(const std::string& newOwner)
	{
		m_ownerName = newOwner;
	}
	std::string GetOwnerName() const
	{
		return m_ownerName;
	}

	int GetID() const
	{
		return m_id;
	}

	//Prints the deets
	virtual void PrintDeets()
	{
		//Is this ugly?
		std::cout	<< "Component type: "
					<< (m_type == Render ? "Render" :
					m_type == Physics ? "Phsyics" : "Inventory")
					<< ", ID: " << m_id << ", Owner Name: " << m_ownerName
					<< std::endl;
	}
	virtual IComponent* Clone()
	{
		assert(1);
		return NULL;
	}

protected:
	std::string m_ownerName;
	ComponentType m_type;
	int m_id;
};

class RenderComponent : public IComponent
{

public:
	RenderComponent(std::string ownerName, int id) 
	{
		m_ownerName = ownerName;
		m_id = id;
		m_type = Render;
	}
	RenderComponent* Clone()
	{
		return new RenderComponent(*this);
	}
};

class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(std::string ownerName, int id)
	{
		m_ownerName = ownerName;
		m_id = id;
		m_type = Physics;
	}
	PhysicsComponent* Clone()
	{
		return new PhysicsComponent(*this);
	}
};

class InventoryComponent : public IComponent
{
	
public:

	InventoryComponent(std::string ownerName, int id, int capacity)
	{
		m_ownerName = ownerName;
		m_id = id;
		m_type = Inventory;
		m_capacity = capacity;
	}

	void PrintDeets() override
	{
		std::cout	<< "Component type: Inventory"
					<< ", ID: " << m_id << ", Owner Name: " << m_ownerName
					<< ", Capacity: " << m_capacity << std::endl;
	}
	InventoryComponent* Clone()
	{
		return new InventoryComponent(*this);
	}

protected:
	int m_capacity;
};

namespace Creators
{
	RenderComponent* CreateRenderComponent(std::string ownerName, int id)
	{
		return new RenderComponent(ownerName, id);
	}
	PhysicsComponent* CreatePhysicsComponent(std::string ownerName, int id)
	{
		return new PhysicsComponent(ownerName, id);
	}

	//Phew good thing you put capacity at the start, otherwise I'd have to go back to the functors
	InventoryComponent* CreateInventoryComponent(int capacity, std::string ownerName, int id)
	{
		return new InventoryComponent(ownerName, id, capacity);
	}

	IComponent* ComponentCloner(IComponent* original)
	{
		return original->Clone();
	}
}


void Chapter8::Run()
{
	//Non-Singleton Factory with functors
	//Functors
	Functor<RenderComponent*, TYPELIST_2(std::string, int)>		renderCreator(&Creators::CreateRenderComponent);
	Functor<PhysicsComponent*, TYPELIST_2(std::string, int)>	physicsCreator(&Creators::CreatePhysicsComponent);

	Factory<IComponent, ComponentType, TYPELIST_2(std::string, int)> componentFactory;
	
	componentFactory.Register(Render, renderCreator);
	componentFactory.Register(Physics, physicsCreator);

	IComponent* renderComp			= componentFactory.CreateObject(Render, "Player", 0);
	IComponent* physicsComponent	= componentFactory.CreateObject(Physics, "Player", 1);

	renderComp->PrintDeets();
	physicsComponent->PrintDeets();
	std::cout << std::endl;

	delete renderComp;
	delete physicsComponent;

	//Singleton Factory with bound functors
	//Bind functors
	/*
		RenderComponent and PhysicsComponent are fine,
		but whoever added the inventory component added a 3rd arguement.
		Luckly that person added it to the beiginning of the creator call
		so we can use the magic of ~~~FuNcToRs~~~ without having to add more BinderFunction!
		Thanks Guy!
	*/
	Functor<InventoryComponent*, TYPELIST_3(int, std::string, int)> inventoryCreator(&Creators::CreateInventoryComponent);
	Functor<InventoryComponent*, TYPELIST_2(std::string, int)> inventoryCreatorBound(BindFirst(inventoryCreator, 100));

	typedef SingletonHolder<Factory<IComponent, ComponentType, TYPELIST_2(std::string, int)>, CreationPolicies::CreateWithNew, LifetimePolicies::DefaultLifetime> SinglComponentFactory;
	SinglComponentFactory::Instance().Register(Render, renderCreator);
	SinglComponentFactory::Instance().Register(Physics, physicsCreator);
	SinglComponentFactory::Instance().Register(Inventory, inventoryCreatorBound);

	IComponent* inventoryComponent;
	renderComp			= SinglComponentFactory::Instance().CreateObject(Render, "Player", 0);
	physicsComponent	= SinglComponentFactory::Instance().CreateObject(Physics, "Player", 1);
	inventoryComponent	= SinglComponentFactory::Instance().CreateObject(Inventory, "Player", 2);
	
	renderComp->PrintDeets();
	physicsComponent->PrintDeets();
	inventoryComponent->PrintDeets();
	std::cout << std::endl;

	delete renderComp;
	delete inventoryComponent;

	//Clone factory
	//Here I override the default template arguement for creators
	Functor<IComponent*, TYPELIST_1(IComponent*)> cloneFunctor(&Creators::ComponentCloner);

	CloneFactory<IComponent, Functor<IComponent*, TYPELIST_1(IComponent*)>> componentCloner;
	componentCloner.Register(TypeInfo(typeid(PhysicsComponent)), cloneFunctor);

	std::cout << "Original Physics Component" << std::endl;
	physicsComponent->PrintDeets();
	std::cout << std::endl;

	IComponent* physicsComponentClone = componentCloner.CreateObject(static_cast<IComponent*>(physicsComponent));
	physicsComponentClone->SetOwnerName("Player2");
	
	std::cout << "Cloned Physics Component" << std::endl;
	physicsComponentClone->PrintDeets();

	delete physicsComponent;
	delete physicsComponentClone;
}