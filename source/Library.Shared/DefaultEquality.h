#pragma once

namespace FieaGameEngine
{
	template <typename T>
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const;
	};
};

#include "DefaultEquality.inl"