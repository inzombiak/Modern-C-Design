#include "Chapter11.h"
#include "CallDispatchers.h"

#include <iostream>
#include <string>

class Shape
{
public:
	IMPLEMENT_INDEXABLE_CLASS(Shape);

	const std::string& GetName() const
	{
		return m_name;
	}

	virtual ~Shape(){};

protected:
	std::string m_name = "";
};
class Rectangle : public Shape
{
public:
	IMPLEMENT_INDEXABLE_CLASS(Rectangle);

	Rectangle(int id)
	{
		m_name = "Rectangle " + std::to_string(id);
	}
};
class Ellipse : public Shape
{
public:
	IMPLEMENT_INDEXABLE_CLASS(Ellipse);

	Ellipse(int id)
	{
		m_name = "Ellipse " + std::to_string(id);
	}

	virtual ~Ellipse(){}
};
class Circle : public Ellipse
{
public:
	IMPLEMENT_INDEXABLE_CLASS(Circle);

	Circle(int id) : Ellipse(id)
	{
		m_name = "Circle " + std::to_string(id);
	}
};
class Triangle : public Shape
{
public:
	IMPLEMENT_INDEXABLE_CLASS(Triangle);

	Triangle(int id)
	{
		m_name = "Triangle" + std::to_string(id);
	}
};
class WeirdPolygonThing : public Shape
{
public:
	IMPLEMENT_INDEXABLE_CLASS(WeirdPolygonThing);

	WeirdPolygonThing(int id)
	{
		m_name = "WeirdPolygonThing " + std::to_string(id);
	}
};

class CollisionHandler
{
public:
	std::string Fire(Shape& lhs, Shape& rhs)
	{
		return "What do I do with these?";
	}
	std::string Fire(Rectangle& lhs, Rectangle& rhs)
	{
		return "AABB: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Rectangle& lhs, Ellipse& rhs)
	{
		return "Rect-Ellipse: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Rectangle& lhs, Triangle& rhs)
	{
		return "Rect-Triangle: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Ellipse& lhs, Ellipse& rhs)
	{
		return "Ellipse-Ellipse: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Ellipse& lhs, Triangle& rhs)
	{
		return "Ellipse-Triangle: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Circle& lhs, Circle& rhs)
	{
		return "Circle-Circle: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string Fire(Triangle& lhs, Triangle& rhs)
	{
		return "Triangle-Triangle: " + lhs.GetName() + " collided with " + rhs.GetName();
	}
	std::string OnError(Shape& lhs, Shape& rhs)
	{
		return "No handler for the combination: " + lhs.GetName() + " collided with " + rhs.GetName();
	}


	
	
};

//For use with FunctorDispatcher
std::string HandleCircleCircle(const Circle& lhs, const Circle& rhs)
{
	return "Handling Circle-Circle: " + lhs.GetName() + " collided with " + rhs.GetName();
}

std::string HandleRectEllipse(const Rectangle lhs, const Ellipse& rhs)
{
	return "Handle Rect-Ellipse: " + lhs.GetName() + " collided with " + rhs.GetName();
}

class Food
{
public:
	const std::string& GetName() const
	{
		return m_name;
	}

	virtual ~Food(){};

protected:
	std::string m_name;
};

class Apple : public Food
{
public:
	Apple()
	{
		m_name = "Apple";
	}
};

class Chocolate : public Food
{
public:
	Chocolate()
	{
		m_name = "Chocolate";
	}
};

class Rat : public Food
{
public:
	Rat()
	{
		m_name = "Rat";
	}
};

class Being
{
public:
	const std::string& GetName() const
	{
		return m_name;
	}

	virtual ~Being(){};

protected:
	std::string m_name;
};

class Doctor : public Being
{
public:
	Doctor()
	{
		m_name = "Doctor";
	}
};

class Dog : public Being
{
public:
	Dog()
	{
		m_name = "Dog";
	}
};

class Person : public Being
{
public:
	Person()
	{
		m_name = "Person";
	}
};

namespace EdibleHandlers
{
	std::string AppleDoctor(Apple& apple, Doctor& doc)
	{
		return apple.GetName() + " is super effective against " + doc.GetName();
	}

	std::string AppleDog(Apple& apple, Dog& dog)
	{
		return apple.GetName() + " is edible by " + dog.GetName();
	}

	std::string ApplePerson(Apple& apple, Person& person)
	{
		return apple.GetName() + " is edible by " + person.GetName();
	}

	std::string ChocolateDog(Chocolate& choc, Dog& dog)
	{
		return "Good job, you killed the dog";
	}

	std::string RatPerson(Rat& rat, Person& person)
	{
		return "I mean, if you want to, go ahead eat the " + rat.GetName();
	}

}

typedef TYPELIST_3(Doctor, Dog, Person) beingTL;
typedef TYPELIST_3(Apple, Chocolate, Rat) foodTl;
typedef TYPELIST_4(Rectangle, Ellipse, Triangle, Circle) shapesTL;
void Chapter11::Run()
{
	std::cout << "~~~ Static Dispatcher Start ~~~" << std::endl;
	std::cout << std::endl;

	{
		typedef StaticDispatcher<CollisionHandler, true, Shape, shapesTL, Shape, shapesTL, std::string> SymmShapeDispatcher;

		SymmShapeDispatcher m_symmShapeDispatcher;

		CollisionHandler m_ch;

		Rectangle rect1(1);
		Rectangle rect2(2);
		Ellipse ellipse1(1);
		Circle circle1(1);
		Circle circle2(2);
		Triangle triangle1(1);
		WeirdPolygonThing wpt1(1);

		std::cout << m_symmShapeDispatcher.Go(rect1, rect2, m_ch) << std::endl;
		std::cout << m_symmShapeDispatcher.Go(circle2, circle1, m_ch) << std::endl;
		std::cout << m_symmShapeDispatcher.Go(triangle1, ellipse1, m_ch) << std::endl;
		std::cout << m_symmShapeDispatcher.Go(wpt1, rect2, m_ch) << std::endl;
	}

	std::cout << std::endl;
	std::cout << "~~~ Static Dispatcher End ~~~" << std::endl;
	std::cout << std::endl;

	std::cout << "~~~ Logarithmic Dispatcher Start ~~~" << std::endl;
	std::cout << std::endl;

	{
		typedef FnDispatcher<Food, Being, std::string> FoodDispatcher;
		FoodDispatcher m_foodDispatcher;

		m_foodDispatcher.Add<Apple, Doctor, EdibleHandlers::AppleDoctor, false>();
		m_foodDispatcher.Add<Apple, Dog, EdibleHandlers::AppleDog, false>();
		m_foodDispatcher.Add<Apple, Person, EdibleHandlers::ApplePerson, false>();
		m_foodDispatcher.Add<Chocolate, Dog, EdibleHandlers::ChocolateDog, false>();
		m_foodDispatcher.Add<Rat, Person, EdibleHandlers::RatPerson, false>();

		Apple apple;
		Chocolate choc;
		Rat rat;
		Dog dog;
		Doctor doc;
		Person person;

		std::cout << m_foodDispatcher.Go(apple, doc) << std::endl;
		std::cout << m_foodDispatcher.Go(apple, dog) << std::endl;
		std::cout << m_foodDispatcher.Go(apple, person) << std::endl;
		std::cout << m_foodDispatcher.Go(choc, dog) << std::endl;
		std::cout << m_foodDispatcher.Go(rat, person) << std::endl;
	}

	std::cout << std::endl;
	std::cout << "~~~ Logarithmic Dispatcher End ~~~" << std::endl;
	std::cout << std::endl;

	std::cout << "~~~ Functor + Fast Backend Dispatcher Start ~~~" << std::endl;
	std::cout << std::endl;

	{
		CollisionHandler m_ch;

		typedef FunctorDispatcher<Shape&, Shape&, std::string> FastFunctorDispatcher;
		FastFunctorDispatcher m_ffd;

		Functor<std::string, TYPELIST_2(Circle&, Circle&)>	 ccFunctor(&HandleCircleCircle);
		Functor<std::string, TYPELIST_2(Rectangle&, Ellipse&)> reFunctor(&HandleRectEllipse);
		
		m_ffd.Add<Circle&, Circle&, Functor<std::string, TYPELIST_2(Circle&, Circle&)>>(ccFunctor);
		m_ffd.Add<Rectangle&, Ellipse&, Functor<std::string, TYPELIST_2(Rectangle&, Ellipse&)>>(reFunctor);

		Circle circle(1);
		Ellipse ellipse(1);
		Rectangle rect(1);

		std::cout << m_ffd.Go(circle, circle) << std::endl;
		std::cout << m_ffd.Go(rect, ellipse) << std::endl;

	}

	std::cout << std::endl;
	std::cout << "~~~ Functor + Fast Backend  End ~~~" << std::endl;
	std::cout << std::endl;
}