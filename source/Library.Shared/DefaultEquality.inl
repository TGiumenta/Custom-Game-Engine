#include "DefaultEquality.h"
#include <string.h>
#include <utility>

namespace FieaGameEngine
{
	template <typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

	template<>
	struct DefaultEquality<char*>
	{
		bool operator()(char* lhs, char* rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char*>
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char* const>
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};
}