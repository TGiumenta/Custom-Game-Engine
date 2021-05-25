#pragma once
#include "RTTI.h"

namespace UnitTests
{
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		explicit Foo(int value = 0);
		Foo(const Foo& other);					// Lvalue
		Foo(Foo&& other) noexcept;				// Rvalue
		Foo& operator=(Foo&& other) noexcept;	// Rvalues
		Foo& operator=(const Foo& other);		// Lvalues
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		const int& Data() const;

		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

	private:
		int *_data;

	};
}