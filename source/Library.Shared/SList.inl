#include "pch.h"
#include "SList.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
#pragma region Node 

	template <typename T>
	inline SList<T>::Node::Node(const T& new_data, Node* node_next) :
		data(new_data),
		next(node_next)
	{}

	template <typename T>
	inline SList<T>::Node::Node(T&& new_data, Node* node_next) :
		data(std::forward<T>(new_data)),
		next(node_next)
	{}

#pragma endregion Node

#pragma region Iterator
	template <typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		owner(&owner), 
		current(node)
	{}

	template <typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return ((owner != other.owner) || (current != other.current));
	}

	template <typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("owner shouldn't be null. Is this the correct iterator?");
		}

		if (current != nullptr)
		{
			current = current->next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();

		return temp;
	}

	template <typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (current == nullptr)
		{
			throw std::runtime_error("current shouldn't be null. Is this iterator == end()?");
		}

		return current->data;
	}

#pragma endregion Iterator

#pragma region ConstIterator

	template <typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		owner(&owner), current(node)
	{}

	template <typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner), current(other.current)
	{}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator & other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return ((owner != other.owner) || (current != other.current));
	}

	template <typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("owner shouldn't be null. Is this the correct iterator?");
		}

		if (current != nullptr)
		{
			current = current->next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();

		return temp;
	}

	template <typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (current == nullptr)
		{
			throw std::runtime_error("current shouldn't be null. Is this iterator == end()?");
		}

		return current->data;
	}

#pragma endregion ConstIterator

#pragma region SList

	template <typename T>
	inline SList<T>::SList(const SList<T>& other) :
		front(nullptr),
		back(nullptr),
		size(0_z)
	{
		DeepCopyList(other);
	}

	template <typename T>
	inline SList<T>::SList(SList<T>&& other) noexcept : 
		front(other.front),
		back(other.back),
		size(other.size)
	{
		other.front = nullptr;
		other.back = nullptr;
		other.size = 0_z;
	}

	template <typename T>
	inline SList<T>& SList<T>::operator=(const SList<T>& other)
	{
		if (this != &other)
		{
			Clear();
			DeepCopyList(other);
		}

		return *this;
	}

	template <typename T>
	inline SList<T>& SList<T>::operator=(SList<T>&& other) noexcept
	{
		if (this != &other)
		{
			Clear();

			front = other.front;
			back = other.back;
			size = other.size;

			other.front = nullptr;
			other.back = nullptr;
			other.size = 0_z;
		}

		return *this;
	}

	template <typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(const T& other_data)
	{
		front = new Node(other_data, front);

		if (IsEmpty())
		{
			back = front;
		}

		++size;

		return begin();
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(T&& other_data)
	{
		front = new Node(std::forward<T>(other_data), front);

		if (IsEmpty())
		{
			back = front;
		}

		++size;

		return begin();
	}

	template <typename T>
	inline void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* temp_head_next = front->next;
			delete front;
			front = temp_head_next;

			--size;

			if (IsEmpty())
			{
				back = nullptr;
				front = nullptr;
			}
		}
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(const T& other_data)
	{
		Node* old_tail = back;
		back = new Node(other_data);
		if (IsEmpty())
		{
			front = back;
		}
		else
		{
			old_tail->next = back;
		}

		++size;

		return Iterator(*this, back);
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(T&& other_data)
	{
		Node* old_tail = back;
		back = new Node(std::forward<T>(other_data), nullptr);
		if (IsEmpty())
		{
			front = back;
		}
		else
		{
			old_tail->next = back;
		}

		++size;

		return Iterator(*this, back);
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return (size == 0_z);
	}

	template <typename T>
	inline T& SList<T>::Front()
	{
		if (front == nullptr)
		{
			throw std::runtime_error("front should not be null. Is the list empty?");
		}
		return front->data;
	}

	template <typename T>
	inline const T& SList<T>::Front() const
	{
		if (front == nullptr)
		{
			throw std::runtime_error("front should not be null. Is the list empty?");
		}
		return front->data;
	}

	template <typename T>
	inline T& SList<T>::Back()
	{
		if (back == nullptr)
		{
			throw std::runtime_error("back should not be null. Is the list empty?");
		}
		return back->data;
	}

	template <typename T>
	inline const T& SList<T>::Back() const
	{
		if (back == nullptr)
		{
			throw std::runtime_error("back should not be null. Is the list empty?");
		}
		return back->data;
	}

	template <typename T>
	inline size_t SList<T>::Size() const
	{
		return size;
	}

	template <typename T>
	inline void SList<T>::Clear()
	{
		Node* currentNode = front;
		while (currentNode != nullptr)
		{
			Node* nodeToDelete = currentNode;
			currentNode = currentNode->next;
			delete nodeToDelete;
		}

		size = 0_z;
		front = back = nullptr;
	}

	template <typename T>
	void SList<T>::PopBack()
	{
		if (!IsEmpty())
		{
			Node* temp_pre_tail = front;

			if (size != 1)
			{
				while (temp_pre_tail->next->next != nullptr)
				{
					temp_pre_tail = temp_pre_tail->next;
				}
			}

			delete back;
			temp_pre_tail->next = nullptr;
			back = temp_pre_tail;

			--size;

			if (IsEmpty())
			{
				front = nullptr;
			}
		}
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}

	template <typename T>
	inline SList<T>& SList<T>::DeepCopyList(const SList<T>& other)
	{
		for (const T& value : other)
		{
			PushBack(value);
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const T& other_data, const Iterator& it)
	{
		if (it.owner != this)
		{
			throw std::runtime_error("owners are not equivalent. Are you inserting on the correct list?");
		}

		if (it == end() || it.current == back)
		{
			return PushBack(other_data);
		}

		Node* node = new Node(other_data, it.current->next);
		it.current->next = node;
		++size;

		return Iterator(*this, node);
	}

	template <typename T>
	template <typename EqualityFunctor>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		EqualityFunctor eq{};

		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (eq(*it,value))
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	template <typename EqualityFunctor>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		return const_cast<SList*>(this)->Find<EqualityFunctor>(value);
	}

	template <typename T>
	template <typename EqualityFunctor>
	inline bool SList<T>::Remove(const T& value)
	{
		return Remove(Find<EqualityFunctor>(value));
	}

	template <typename T>
	inline bool SList<T>::Remove(const Iterator& it)
	{
		if (it.owner != this)
		{
			throw std::runtime_error("Invalid iterator, incorrect owner");
		}

		bool found = false;
		if (it != end())
		{
			if (it.current == back)
			{
				PopBack();
			}

			else
			{
				Node* node_to_delete = it.current->next;

				it.current->data.~T();
				new(&it.current->data)T(std::move(node_to_delete->data));
				it.current->next = node_to_delete->next;
				delete node_to_delete;

				if (it.current->next == nullptr)
				{
					back = it.current;
				}
				--size;
			}

			found = true;
		}

		return found;
	}
}

#pragma endregion SList