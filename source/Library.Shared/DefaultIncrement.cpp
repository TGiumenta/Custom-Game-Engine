#include "pch.h"
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	size_t DefaultIncrement::operator()(size_t /*size*/, std::size_t capacity) const
	{
		return capacity + 1;
	}
}