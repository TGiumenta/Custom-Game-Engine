#include "pch.h"
#include "Vector.h"
#include "SizeLiteral.h"
#include <cassert>
#include <stdlib.h>
#include <initializer_list>

namespace FieaGameEngine
{
#pragma region Iterator

	template <typename T>
	inline Vector<T>::Iterator::Iterator(Vector& owner, const size_t new_index) :
		owner(&owner),
		index(new_index)
	{}

	template <typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return ((owner != other.owner) || (index != other.index));
	}

	template <typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (index < owner->size)
		{
			++index;
		}

		return *this;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();

		return temp;
	}

	template <typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (index <= owner->size)
		{
			--index;
		}

		return *this;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp(*this);
		operator--();

		return temp;
	}

	template <typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return owner->at(index);
	}

	template <typename T>
	inline T* Vector<T>::Iterator::operator->() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return &(owner->at(index));
	}

#pragma endregion Iterator

#pragma region ConstIterator
	template <typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, const size_t new_index) :
		owner(&owner), index(new_index)
	{}

	template <typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner), index(other.index)
	{}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return ((owner != other.owner) || (index != other.index));
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (index <= owner->size)
		{
			++index;
		}

		return *this;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();

		return temp;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (index <= owner->size)
		{
			--index;
		}

		return *this;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp(*this);
		operator--();

		return temp;
	}

	template <typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return owner->at(index);
	}

	template <typename T>
	inline const T* Vector<T>::ConstIterator::operator->() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return &(owner->at(index));
	}

#pragma endregion ConstIterator

#pragma region Vector	
	template <typename T>
	inline Vector<T>::Vector(const size_t new_capacity)
	{
		if (new_capacity > 0_z)
		{
			Reserve(new_capacity);
		}
	}

	template <typename T>
	inline Vector<T>::Vector(std::initializer_list<T> init_list)
	{
		if (init_list.size() > 0_z)
		{
			Reserve(init_list.size());
			for (const T& value : init_list)
			{
				new(data + size++)T(value);
			}
		}
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& other)
	{
		DeepCopyVector(other);
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept :
		data(other.data),
		size(other.size),
		capacity(other.capacity)
	{
		other.data = nullptr;
		other.size = 0_z;
		other.capacity = 0_z;
	}

	template <typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();
			ShrinkToFit();
			DeepCopyVector(other);
		}

		return *this;
	}

	template <typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			free(data);

			data = other.data;
			size = other.size;
			capacity = other.capacity;

			other.data = nullptr;
			other.size = 0_z;
			other.capacity = 0_z;
		}

		return *this;
	}

	template <typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		free(data);
	}

	template <typename T>
	inline T& Vector<T>::operator[](const size_t search_index)
	{
		if (search_index >= size)
		{
			throw std::runtime_error("The index is out of range.");
		}

		return data[search_index];
	}

	template <typename T>
	inline T& Vector<T>::operator[](const size_t search_index) const
	{
		if (search_index >= size)
		{
			throw std::runtime_error("The index is out of range.");
		}

		return data[search_index];
	}

	template <typename T>
	inline T& Vector<T>::at(const size_t search_index)
	{
		if ((search_index < 0_z) || (search_index >= size))
		{
			throw std::runtime_error("The index is out of range.");
		}

		return data[search_index];
	}

	template <typename T>
	inline T& Vector<T>::at(const size_t search_index) const
	{
		if ((search_index < 0_z) || (search_index >= size))
		{
			throw std::runtime_error("The index is out of range.");
		}

		return data[search_index];
	}

	template <typename T>
	inline void Vector<T>::PopBack()
	{
		if (size != 0_z)
		{
			data[size-1_z].~T();
			--size;
		}
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (size == 0_z);
	}

	template <typename T>
	inline T& Vector<T>::Front()
	{
		if (size == 0_z)
		{
			throw std::runtime_error("The size should not be 0. Is the vector empty?");
		}
		return data[0_z];
	}

	template <typename T>
	inline const T& Vector<T>::Front() const
	{
		if (size == 0_z)
		{
			throw std::runtime_error("The size should not be 0. Is the vector empty?");
		}
		return data[0_z];
	}

	template <typename T>
	inline T& Vector<T>::Back()
	{
		if ((size == 0_z))
		{
			throw std::runtime_error("The last element does not exist. Is the vector empty?");
		}
		return data[size-1_z];
	}

	template <typename T>
	inline const T& Vector<T>::Back() const
	{
		if ((size == 0_z))
		{
			throw std::runtime_error("The last element does not exist. Is the vector empty?");
		}
		return data[size-1_z];
	}

	template <typename T>
	inline size_t Vector<T>::Size() const
	{
		return size;
	}

	template <typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return capacity;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, size);
	}

	template <typename T>
	template <typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		IncrementFunctor incrementor{};

		if (size == capacity)
		{
			size_t new_capacity = capacity + std::max(1_z, incrementor(size, capacity));
			Reserve(new_capacity);
		}

		new(data + size)T(value);
		
		return Iterator(*this, size++);
	}

	template <typename T>
	template <typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(T&& value)
	{
		IncrementFunctor incrementor{};

		if (size == capacity)
		{
			size_t new_capacity = capacity + std::max(1_z, incrementor(size, capacity));
			Reserve(new_capacity);
		}

		new(data + size)T(std::forward<T>(value)); 

		return Iterator(*this, size++);
	}

	template <typename T>
	inline void Vector<T>::Reserve(const size_t new_capacity)
	{
		if (new_capacity > capacity)
		{
			T* new_data = reinterpret_cast<T*>(realloc(data, new_capacity * sizeof(T)));
			assert(new_data != nullptr);

			data = new_data;
			capacity = new_capacity;
		}
	}

	template <typename T>
	void Vector<T>::Resize(const size_t new_size)
	{
		if (new_size < size)
		{
			for (size_t index = new_size; index < size; ++index)
			{
				data[index].~T();
			}
		}
		else
		{
			Reserve(new_size);

			for (size_t index = size; index < new_size; ++index)
			{
				new (data + index)T();
			}
		}

		size = new_size;
	}

	template <typename T>
	template <typename EqualityFunctor>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		EqualityFunctor eq{};

		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (eq(data[it.index], value))
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	template <typename EqualityFunctor>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		return const_cast<Vector*>(this)->Find<EqualityFunctor>(value);
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			data[i].~T();
		}

		size = 0_z;
	}

	template <typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (capacity > size)
		{
			if (size == 0)
			{
				free(data);
				data = nullptr;
			}
			else
			{
				T* new_data = reinterpret_cast<T*>(realloc(data, size * sizeof(T)));
				assert(new_data != nullptr);
				data = new_data;
			}

			capacity = size;
		}
	}

	template <typename T>
	bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& it)
	{
		if (it.owner != this)
		{
			throw std::runtime_error("Invalid iterator, incorrect owner");
		}

		bool found = false;
		if (it.index < size)
		{
			data[it.index].~T();
			size_t bytes_to_move = ((size - 1_z) - it.index) * sizeof(T);
			if (bytes_to_move > 0)
			{
				std::memmove(&data[it.index], &data[it.index + 1], bytes_to_move);
			}

			--size;

			found = true;
		}

		return found;
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& first_it, const Iterator& last_it)
	{
		if ((first_it.owner != this || last_it.owner != this))
		{
			throw std::runtime_error("Invalid iterator, incorrect owner");
		}

		if (first_it.index > last_it.index)
		{
			throw std::runtime_error("The last iterator is present in the vector before the first iterator.");
		}

		const size_t elements_to_remove = last_it.index - first_it.index;
		bool found = false;
		if (elements_to_remove > 0)
		{
			for (size_t index_reference = first_it.index; index_reference != last_it.index; ++index_reference)
			{
				data[index_reference].~T();
			}

			size_t bytes_to_move = (size - last_it.index) * sizeof(T);

			if (bytes_to_move > 0)
			{
				std::memmove(&data[first_it.index], &data[last_it.index], bytes_to_move);
			}

			size -= elements_to_remove;

			found = true;
		}

		return found;
	}
	
	template <typename T>
	inline Vector<T>& Vector<T>::DeepCopyVector(const Vector& other)
	{
		if (other.capacity > 0_z)
		{
			Reserve(other.capacity);

			for (const T& value : other)
			{
				new(data + size++)T(value);
			}
		}

		return *this;
	}

#pragma endregion Vector
}