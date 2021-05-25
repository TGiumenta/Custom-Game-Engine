#include "pch.h"
#include "Stack.h"

namespace FieaGameEngine
{
	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		list.PushFront(value);
	}

	template <typename T>
	inline void Stack<T>::Push(T&& value)
	{
		list.PushFront(std::move(value));
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		list.PopFront();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return list.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Top() const
	{
		return list.Front();
	}

	template <typename T>
	inline std::size_t Stack<T>::Size() const
	{
		return list.Size();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return list.IsEmpty();
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		return list.Clear();
	}
}