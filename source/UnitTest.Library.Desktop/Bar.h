#pragma once
#include "RTTI.h"

namespace UnitTests
{
	class Bar : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI)

	public:
		explicit Bar(int value = 0);
		Bar(const Bar& other);					// Lvalue
		Bar(Bar&& other) noexcept;				// Rvalue
		Bar& operator=(const Bar& other);		// Lvalue
		Bar& operator=(Bar&& other) noexcept;	// Rvalue
		~Bar();

		const int& Data() const;

		//bool Equals(const RTTI* rhs) const override;

	private:
		int *_data;

	};
}