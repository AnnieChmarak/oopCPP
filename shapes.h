#ifndef SHAPES_HEADER
#define SHAPES_HEADER
#pragma once

#include "base.h"
#include <iostream>
#include <math.h>
#include <stdexcept>

class Shapes : public Printable
{
private:
	static uint32_t m_count;

public:
	Shapes() { m_count++; }
	virtual ~Shapes() { m_count--; }

	virtual std::string GetDescription() = 0;
	static uint32_t GetCount() { return m_count; }

	static const int FIELD_CAPACITY = 500;
};
size_t Shapes::m_count = 0;

//---------------------------------------------------//

class Point : public Shapes, public Named
{
private:
	int m_coordX;
	int m_coordY;
	
public:
	Point() = delete;

	Point(int x, int y)
		: Named("Point")
	{
		if ((x > FIELD_CAPACITY) ||
			(y > FIELD_CAPACITY) ||
			(x < -FIELD_CAPACITY) ||
			(y < -FIELD_CAPACITY)) {
			throw std::domain_error::domain_error("!!! Out of field");
		}

		m_coordX = x;
		m_coordY = y;
	}

	int GetCoordX()
	{
		return m_coordX;
	}

	int GetCoordY()
	{
		return m_coordY;
	}

	virtual std::string GetPrintableInfo()
	{
		std::string info;
		info += "Coordinates: (";
		info += std::to_string(m_coordX);
		info += ", ";
		info += std::to_string(m_coordY);
		info += ")";
		info += "\n";
		return info;
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
};

//---------------------------------------------------//

class Circle : public Shapes, public Named
{
private:
	Point* m_center;
	uint32_t m_radius;
	
public:
	Circle(int centerX, int centerY, uint32_t radius)
		: Named("Circle")
		, m_center(nullptr)
	{
		if ((centerX + radius > FIELD_CAPACITY) ||
			(centerY + radius > FIELD_CAPACITY) ||
			(centerX - radius < -FIELD_CAPACITY) ||
			(centerY - radius < -FIELD_CAPACITY)) {
			throw std::domain_error::domain_error("!!! Out of field");
		}

		m_center = new Point(centerX, centerY);
		m_radius = radius;
	}

	~Circle()
	{
		delete m_center;
	}

	virtual std::string GetPrintableInfo()
	{
		std::string info;
		info += "Center:\t";
		info += m_center->GetPrintableInfo();
		info += "Radius: ";
		info += std::to_string(m_radius);
		info += "\n";
		return info;
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
};

//---------------------------------------------------//

class Rect : public Shapes, public Named
{
private:
	Point* m_leftTopCorner;
	uint32_t m_width;
	uint32_t m_height;
	
public:
	Rect(int left, int top, uint32_t width, uint32_t height)
		: Named("Rectangle")
		, m_leftTopCorner(nullptr)
	{
		if ((left + width > FIELD_CAPACITY) ||
			(top + height > FIELD_CAPACITY)) {
			throw std::domain_error::domain_error("!!! Out of field");
		}

		m_leftTopCorner = new Point(left, top);
		m_width = width;
		m_height = height;
	}

	~Rect()
	{
		delete m_leftTopCorner;
	}

	uint32_t GetArea()
	{
		return m_width * m_height;
	}

	virtual std::string GetPrintableInfo()
	{
		std::string info;
		int coordX = m_leftTopCorner->GetCoordX();
		int coordY = m_leftTopCorner->GetCoordY();

		info += "Corners:\n\t";
		info += m_leftTopCorner->GetPrintableInfo();
		info += "\t";
		info += Point(coordX + m_width, coordY).GetPrintableInfo();
		info += "\t";
		info += Point(coordX, coordY + m_height).GetPrintableInfo();
		info += "\t";
		info += Point(coordX + m_width, coordY + m_height).GetPrintableInfo();
		info += "Area: ";
		info += std::to_string(GetArea());
		info += "\n";
		return info;
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
};

//---------------------------------------------------//

class Square : public Shapes, public Named
{
private:
	Rect* m_squaredRect;

public:
	Square(int left, int top, uint32_t side)
		: Named("Square")
		, m_squaredRect(nullptr)
	{
		m_squaredRect = new Rect(left, top, side, side);
	}

	~Square()
	{
		delete m_squaredRect;
	}

	virtual std::string GetPrintableInfo()
	{
		return m_squaredRect->GetPrintableInfo();
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
};

//---------------------------------------------------//

class Polyline : public Shapes, public Named
{
private:
	Container<Point*> m_points;

public:
	Polyline()
		: Named("Polyline")
	{}

	void AddPoint(int coordX, int coordY)
	{
		m_points.PushBack(new Point(coordX, coordY));
	}

	double GetLength()
	{
		double length = 0;

		Container<Point*>::Iterator it = m_points.Begin();
		Container<Point*>::Iterator itNext = m_points.Begin();
		Container<Point*>::Iterator end = m_points.End();
		if (itNext != end)
			++itNext;

		for (; it != end, itNext != end; ++it, ++itNext) {
			double diffX = std::pow((*it)->GetCoordX() - (*itNext)->GetCoordX(), 2);
			double diffY = std::pow((*it)->GetCoordY() - (*itNext)->GetCoordY(), 2);
			length += std::sqrt(diffX + diffY);
		}

		return length;
	}

	virtual std::string GetPrintableInfo()
	{
		std::string info;

		info += "Connected points:\n";
		Container<Point*>::Iterator it = m_points.Begin();
		Container<Point*>::Iterator end = m_points.End();
		for (; it != end; ++it) {
			info += "\t";
			info += (*it)->GetPrintableInfo();
		}

		info += "Length:\n\t";
		info += std::to_string(GetLength());
		info += "\n";

		return info;
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
};

//---------------------------------------------------//

class Polygon : public Shapes, public Named
{
private:
	Container<Point*> m_vertices;

public:
	Polygon()
		: Named("Polygon")
	{}

	void AddVertice(int coordX, int coordY)
	{
		m_vertices.PushBack(new Point(coordX, coordY));
	}

	double GetPerimeter()
	{
		double length = 0;

		Container<Point*>::Iterator it = m_vertices.Begin();
		Container<Point*>::Iterator itNext = m_vertices.Begin();
		Container<Point*>::Iterator end = m_vertices.End();
		if (itNext != end)
			++itNext;

		Point* first = *it;

		for (; itNext != end; ++it, ++itNext) {
			double diffX = std::pow((*it)->GetCoordX() - (*itNext)->GetCoordX(), 2);
			double diffY = std::pow((*it)->GetCoordY() - (*itNext)->GetCoordY(), 2);
			length += std::sqrt(diffX + diffY);
		}

		Point* last = *it;

		double diffX = std::pow((*first).GetCoordX() - (*last).GetCoordX(), 2);
		double diffY = std::pow((*first).GetCoordY() - (*last).GetCoordY(), 2);
		length += std::sqrt(diffX + diffY);

		return length;
	}

	virtual std::string GetPrintableInfo()
	{
		std::string info;

		info += "Vertices:\n";
		Container<Point*>::Iterator it = m_vertices.Begin();
		Container<Point*>::Iterator end = m_vertices.End();
		for (; it != end; ++it) {
			info += "\t";
			info += (*it)->GetPrintableInfo();
		}

		info += "Perimeter:\n\t";
		info += std::to_string(GetPerimeter());
		info += "\n";

		return info;
	}

	std::string GetName()
	{
		return Named::GetPrintableInfo();
	}

	std::string GetDescription()
	{
		std::string desc;
		desc += "-=- ";
		desc += Named::GetPrintableInfo();
		desc += " -=-\n";
		desc += GetPrintableInfo();
		return desc;
	}
	
};

//---------------------------------------------------//

template<typename T>
std::ostream& operator<< (std::ostream& os, T& shape)
{
	os << shape.GetDescription();
	return os;
}

#endif