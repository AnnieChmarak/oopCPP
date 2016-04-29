#ifndef BASE_HEADER
#define BASE_HEADER
#pragma once

#include <string>

class Printable
{
public:

public:
	virtual ~Printable() {};
	virtual std::string GetPrintableInfo() = 0;
};

class Named : private Printable
{
private:
	std::string m_name;
public:
	Named() = delete;

	Named(std::string name)
	{
		m_name = name;
	}

	virtual std::string GetPrintableInfo()
	{
		return m_name;
	}
};

#endif