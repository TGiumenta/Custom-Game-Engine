#include "pch.h"
#include "Bar.h"
#include <cassert>
#include <exception>

namespace UnitTests
{
	RTTI_DEFINITIONS(Bar)

	Bar::Bar(int value) :
		_data(new int(value))
	{}
	Bar::Bar(const Bar& other) :
		_data(new int(*other._data))
	{}
	Bar::Bar(Bar&& other) noexcept :
		_data(other._data)	// Shallow copy
	{
		// Without this, you have an aliasing situation
		other._data = nullptr;
	}

	Bar& Bar::operator=(const Bar& other)
	{
		if (this != &other)
		{
			// Not a self assignment
			*_data = *other._data;
		}

		return *this;
	}

	Bar& Bar::operator=(Bar&& other) noexcept
	{
		if (this != &other)	// Sameness test, not equivalence test
		{
			delete _data;
			_data = other._data;
			other._data = nullptr;	// Now, calling delete against nullptr does nothing
		}

		return *this;
	}

	Bar::~Bar()
	{
		delete _data;
	}

	const int& Bar::Data() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you std::move() this instance?");
		}

		return *_data;
	}
}