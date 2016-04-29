#ifndef FACTORY_HEADER
#define FACTORY_HEADER
#pragma once

#include "shapes.h"
#include <map>
#include <limits>
#include <time.h>

template<typename T>
class RandomGenerator
{
public:
	static T GetRandomValue()
	{
		static bool initialized = false;
		if (!initialized) {
			srand(unsigned int(time(NULL)));
			initialized = true;
		}
		int randomValue = rand() % std::numeric_limits<T>::max();
		if (std::numeric_limits<T>::is_signed)
			randomValue = rand() % 2
			? randomValue
			: -randomValue;

		return T(randomValue);
	}
};

class AbstractShapeGenerator
{
public:
	virtual ~AbstractShapeGenerator() {};
	virtual Shapes* Create() const = 0;
};

template<typename Tshape>
class ShapeGenerator : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		return new T();
	}
};

template<>
class ShapeGenerator<Point> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		int x = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		int y = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		return new Point(x, y);
	}
};

template<>
class ShapeGenerator<Circle> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		int centerX = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		int centerY = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		uint32_t radius = RandomGenerator<uint32_t>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		return new Circle(centerX, centerY, radius);
	}
};

template<>
class ShapeGenerator<Rect> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		int left = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		int top = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		uint32_t width = RandomGenerator<uint32_t>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		uint32_t height = RandomGenerator<uint32_t>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		return new Rect(left, top, width, height);
	}
};

template<>
class ShapeGenerator<Square> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		int left = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		int top = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		uint32_t side = RandomGenerator<uint32_t>::GetRandomValue() % Shapes::FIELD_CAPACITY;
		return new Square(left, top, side);
	}
};

template<>
class ShapeGenerator<Polyline> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		Polyline* polyline = new Polyline();
		uint16_t count = 2 + RandomGenerator<uint16_t>::GetRandomValue() % 10; // just for readability
		for (uint16_t i = 0; i < count; ++i) {
			int coordX = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
			int coordY = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
			polyline->AddPoint(coordX, coordY);
		}

		return polyline;
	}
};

template<>
class ShapeGenerator<Polygon> : public AbstractShapeGenerator
{
public:
	virtual Shapes* Create() const
	{
		Polygon* polygon = new Polygon();
		uint16_t count = 3 + RandomGenerator<uint16_t>::GetRandomValue() % 10; // just for readability
		for (uint16_t i = 0; i < count; ++i) {
			int coordX = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
			int coordY = RandomGenerator<int>::GetRandomValue() % Shapes::FIELD_CAPACITY;
			polygon->AddVertice(coordX, coordY);
		}

		return polygon;
	}
};

//---------------------------------------------------//

class ShapeFactory
{
private:
	typedef	std::map<std::string, AbstractShapeGenerator*> FactoryItems;
	FactoryItems m_factory;

public:
	template<typename T>
	void Add(const std::string& name)
	{
		if (m_factory.find(name) == m_factory.end())
			m_factory[name] = new ShapeGenerator<T>();
	}

	Shapes* Create(const std::string& name)
	{
		if (m_factory.find(name) != m_factory.end()) {
			try {
				return m_factory[name]->Create();
			}
			catch (const std::domain_error& ex) {
				std::cout << ex.what() << std::string(" -- Retry shape creation") << std::endl;
				return Create(name);
			}
		}

		return nullptr;
	}

	Shapes* CreateRandom()
	{
		uint16_t choice = RandomGenerator<uint16_t>::GetRandomValue() % m_factory.size();
		FactoryItems::iterator it = m_factory.begin();
		FactoryItems::iterator end = m_factory.end();
		for (uint16_t i = 0; it != end; ++it, ++i) {
			if (i == choice) break;
		}
		if (it == end)
			return nullptr;

		return Create(it->first);
	}
};

#endif // !FACTORY_HEADER