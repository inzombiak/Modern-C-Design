#include "Chapter9.h"
#include "../Chapter8_ObjectFactories/Factories.h"
#include "../Chapter8_ObjectFactories/Singleton.h"

#include <iostream>

class IEnemy
{
public:
	virtual ~IEnemy()
	{}
	//virtual IEnemy* Clone() = 0;

	std::string GetName() const
	{
		return m_name;
	}
	int GetPowerLevel() const
	{
		return m_powerLevel;
	}

	std::string m_name;
	int m_powerLevel;
};

class Soldier : public IEnemy
{
public:
	virtual Soldier* Clone() = 0;
};

class Monster : public IEnemy
{
public:
	virtual Monster* Clone() = 0;
};

class SuperMonster : public IEnemy
{
public:
	virtual SuperMonster* Clone() = 0;
};

class EasySoldier : public Soldier
{
public:
	EasySoldier()
	{
		m_name = "EasySoldier";
		m_powerLevel = 100;
	}

	EasySoldier* Clone()
	{
		return new EasySoldier();
	}
};

class EasyMonster : public Monster
{
public:
	EasyMonster()
	{
		m_name = "EasyMonster";
		m_powerLevel = 300;
	}
	EasyMonster* Clone()
	{
		return new EasyMonster();
	}
};

class EasySuperMonster : public SuperMonster
{
public:
	EasySuperMonster()
	{
		m_name = "EasySuperMonster";
		m_powerLevel = 2000; 
	}
	EasySuperMonster* Clone()
	{
		return new EasySuperMonster();
	}
};

class HardSoldier : public Soldier
{
public:
	HardSoldier()
	{
		m_name = "HardSoldier";
		m_powerLevel = 2000;
	}
	HardSoldier* Clone()
	{
		return new HardSoldier();
	}
};

class HardMonster : public Monster
{
public:
	HardMonster()
	{
		m_name = "HardMonster";
		m_powerLevel = 3000;
	} 
	HardMonster* Clone()
	{
		return new HardMonster();
	}
};

class HardSuperMonster : public SuperMonster
{
public:
	HardSuperMonster()
	{
		m_name = "HardSuperMonster";
		m_powerLevel = 9001; //I'm sorry
	}
	HardSuperMonster* Clone()
	{
		return new HardSuperMonster();
	}
};

template <typename T>
struct Type2Type
{
	typedef T OriginalType;
};

/*
	Concrete Factory
*/
template <class T>
class AbstractFactoryUnit
{
public:
	virtual T* DoCreate(Type2Type<T>) = 0;
	virtual ~AbstractFactoryUnit() {}
};
template
<
	class TList,
	template <class> class Unit = AbstractFactoryUnit
>
class AbstractFactory : public HierarchyGenerators::GenScatterHierarchy<TList, Unit>
{
public:
	typedef TList ProductList;
	template <class T> T* Create()
	{
		Unit<T>* unit = this;
		return unit->DoCreate(Type2Type<T>());
	}
};
template<class ConcreteProduct, class Base>
class OpNewFactoryUnit : public Base
{
	typedef typename Base::ProductList BaseProductList;
protected:
	typedef typename BaseProductList::Tail ProductList;
public:
	typedef typename BaseProductList::Head AbstractProduct;
	ConcreteProduct* DoCreate(Type2Type<AbstractProduct>)
	{
		return new ConcreteProduct;
	}
};
template
<
	class AbstractFact,
	template <class, class> class Creator = OpNewFactoryUnit,
	class TList = AbstractFact::ProductList
>
class ConcreteFactory : public HierarchyGenerators::GenLinearHierarchy<typename TL::Reverse<TList>::Result, Creator, AbstractFact>
{
public:
	typedef typename AbstractFact::ProductList ProductList;
	typedef TList ConcreteProductList;
};

/*
	Prototype Factory
*/
template <class ConcreteProduct, class Base>
class PrototypeFactoryUnit : public Base
{
	typedef typename Base::ProductList BaseProductList;
protected:
	typedef typename Base::ProductList::Tail ProductList;
public:
	typedef typename Base::ProductList::Head AbstractProduct;

	PrototypeFactoryUnit(AbstractProduct* p = 0) : m_prototype(p)
	{}

	friend void DoGetPrototype(PrototypeFactoryUnit* me, ConcreteProduct* prototype)
	{
		pPrototype = me->m_prototype;
	}

	friend void DoSetPrototype(PrototypeFactoryUnit* me, ConcreteProduct* pObj)
	{
		me->m_prototype = pObj;
	}

	template <class U>
	void GetPrototype(U*& p)
	{
		return DoGetPrototype(this, p);
	}
	template <class U>
	void SetPrototype(U* pObj)
	{
		DoSetPrototype(this, pObj);
	}
	ConcreteProduct* DoCreate(Type2Type<AbstractProduct>)
	{
		assert(m_prototype);
		return m_prototype->Clone();
	}

private:
	AbstractProduct* m_prototype;

};

void Chapter9::Run()
{
	/*
		Concrete factory
	*/
	std::cout << "Concrete Factory" << std::endl << std::endl;

	typedef TYPELIST_3(Soldier, Monster, SuperMonster) baseTL;
	typedef TYPELIST_3(EasySoldier, EasyMonster, EasySuperMonster) easyTL;
	typedef TYPELIST_3(HardSoldier, HardMonster, HardSuperMonster) hardTL;

	typedef ConcreteFactory<AbstractFactory<baseTL>, OpNewFactoryUnit, easyTL> EasyFactory;
	typedef ConcreteFactory<AbstractFactory<baseTL>, OpNewFactoryUnit, hardTL> HardFactory;

	EasyFactory ef;
	HardFactory hf;

	SuperMonster* easySuperMonster = ef.Create<SuperMonster>();
	Monster* hardMonster = hf.Create<Monster>();

	std::cout << "Easy Enemy Stats: Name - " << easySuperMonster->GetName() << ", Power - " << easySuperMonster->GetPowerLevel() << std::endl;
	std::cout << "Hard Enemy Stats: Name - " << hardMonster->GetName() << ", Power - " << hardMonster->GetPowerLevel() << std::endl;

	/*
		Prototype factory
	*/
	std::cout << std::endl << "Prototype Factory" << std::endl << std::endl;

	typedef ConcreteFactory<AbstractFactory<baseTL>, PrototypeFactoryUnit> CloneFactory;

	CloneFactory cf;
	Monster* easyMonster = ef.Create<Monster>();	
	cf.SetPrototype(easyMonster);

	Monster* easyMonsterClone = cf.Create<Monster>();
	easyMonsterClone->m_name = "EasyMonsterClone";

	std::cout << "Easy Monster Original: Name - " << easyMonster->GetName() << ", Power - " << easyMonster->GetPowerLevel() << std::endl;
	std::cout << "Easy Monster Clone(): Name - " << easyMonsterClone->GetName() << ", Power - " << easyMonsterClone->GetPowerLevel() << std::endl;

	delete easyMonster;
	delete easyMonsterClone;
	delete easySuperMonster;
	delete hardMonster;
}