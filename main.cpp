#include "container.h"
#include "base.h"
#include "shapes.h"
#include "factory.h"

#include <cmath>
#include <iostream>

int main()
{
	ShapeFactory factory;
	factory.Add<Point>("Point");
	factory.Add<Circle>("Circle");
	factory.Add<Rect>("Rect");
	factory.Add<Square>("Square");
	factory.Add<Polyline>("Polyline");
	factory.Add<Polygon>("Polygon");

	Container<Shapes*> shapes;
	uint16_t count = 20 + RandomGenerator<uint16_t>::GetRandomValue() % 10;
	for (uint16_t i = 0; i < count; ++i) {
		shapes.PushBack(factory.CreateRandom());
	}

	std::cout << std::string("Number of generated shapes: ") << Shapes::GetCount() << std::endl;

	Container<Shapes*>::Iterator it = shapes.Begin();
	Container<Shapes*>::Iterator end = shapes.End();
	for (; it != end; ++it) {
		std::cout << std::endl;
		std::cout << **it;
	}
	std::cout << std::endl;

	shapes.Clear();
	std::cout << std::string("Remaining number of shapes: ") << Shapes::GetCount() << std::endl;

	std::cin.get();

	return 0;
}