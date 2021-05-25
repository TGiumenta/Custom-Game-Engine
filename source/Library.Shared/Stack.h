#pragma once

#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		void Push(const T& value);
		void Push(T&& value);
		void Pop();
		T& Top();
		const T& Top() const;

		std::size_t Size() const;
		bool IsEmpty() const;

		void Clear();

	private:
		SList<T> list;
	};
}

#include "Stack.inl"