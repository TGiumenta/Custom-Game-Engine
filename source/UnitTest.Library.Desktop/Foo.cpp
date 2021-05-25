#include "pch.h"
#include "Foo.h"
#include <cassert>
#include <exception>

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo)
	
	Foo::Foo(int value) :
		_data(new int(value))
	{}
	Foo::Foo(const Foo& other) :
		_data(new int(*other._data))
	{}
	Foo::Foo(Foo&& other) noexcept :
		_data(other._data)	// Shallow copy
	{
		// Without this, you have an aliasing situation
		other._data = nullptr;
	}
	Foo& Foo::operator=(Foo&& other) noexcept
	{
		if (this != &other)	// Sameness test, not equivalence test
		{
			delete _data;
			_data = other._data;
			other._data = nullptr;	// Now, calling delete against nullptr does nothing
		}

		return *this;
	}
	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			// Not a self assignment
			*_data = *other._data;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete _data;
	}
	bool Foo::operator==(const Foo& rhs) const
	{
		if (_data == nullptr || rhs._data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you std::move() one of these instance?");
		}

		// In every case every foo will have its own pointer
		return *_data == *rhs._data;
	}
	bool Foo::operator!=(const Foo& rhs) const
	{
		return !(operator==(rhs));
	}
	const int& Foo::Data() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you std::move() this instance?");
		}

		return *_data;
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		const Foo* other = rhs->As<Foo>();
		return (other != nullptr) ? *this ==*other : false;
	}

	std::string Foo::ToString() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you std::move() this instance?");
		}

		return std::to_string(*_data);
	}
}