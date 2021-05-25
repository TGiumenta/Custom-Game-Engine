#pragma once

#include "DefaultHash.h"
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	template <typename T>
	struct DefaultHash final
	{
		size_t operator()(const T& key) const;
	};

	/// <summary>
	/// Determines the hash value for the parameter data 
	/// </summary>
	/// <param name="data"> The actual stored address of the passed in value </param>
	/// <param name="size"> The amount of bytes for the data </param>
	/// <returns> Hash value determined </returns>
	size_t AdditiveHash(const std::uint8_t* data, const size_t size);
};

#include "DefaultHash.inl"