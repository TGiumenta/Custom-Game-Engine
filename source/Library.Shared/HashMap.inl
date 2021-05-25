#include "pch.h"
#include "HashMap.h"
#include "SizeLiteral.h"
#include <tuple>

namespace FieaGameEngine
{
#pragma region Iterator

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::Iterator(HashMap& owner, const size_t new_index, const ChainIteratorType& list_it) :
		owner(&owner),
		index(new_index),
		list_iterator(list_it)
	{}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator!=(const Iterator& other) const
	{
		return ((owner != other.owner) || (index != other.index) || (list_iterator != other.list_iterator));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator&
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (owner->size != 0)
		{
			++list_iterator;

			while (list_iterator == owner->buckets.at(index).end())
			{
				++index;
				if (index == owner->buckets.Size())
				{
					list_iterator = owner->buckets.at(index - 1).end();
					break;
				}

				list_iterator = owner->buckets.at(index).begin();
			}
		}

		return *this;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();

		return temp;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType&
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return *list_iterator;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType*
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator->() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return &(*list_iterator);
	}

#pragma endregion Iterator

#pragma region ConstIterator

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const HashMap& owner, const size_t new_index, const ConstChainIteratorType& list_it) :
		owner(&owner),
		index(new_index),
		list_constiterator(list_it)
	{}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner),
		index(other.index),
		list_constiterator(other.list_iterator)
	{}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return ((owner != other.owner) || (index != other.index) || (list_constiterator != other.list_constiterator));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator&
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		if (owner->size != 0)
		{
			++list_constiterator;

			while (list_constiterator == owner->buckets.at(index).end())
			{
				++index;
				if (index == owner->buckets.Size())
				{
					list_constiterator = owner->buckets.at(index - 1).end();
					break;
				}

				list_constiterator = owner->buckets.at(index).begin();
			}
		}

		return *this;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();

		return temp;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline const typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType&
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return *list_constiterator;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline const typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType*
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator->() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Owner should not be null. Does this iterator have an owner?");
		}

		return &(*list_constiterator);
	}

#pragma endregion ConstIterator

#pragma region HashMap
	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::HashMap(const size_t bucket_size)
	{
		if (bucket_size == 0_z)
		{
			throw std::invalid_argument("Do not create an empty HashMap.");
		}

		buckets.Resize(bucket_size);
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Find(const TKey& key)
	{
		auto [was_found, bucket_index, chain_iterator] = KeySearch(key);
		return was_found ? Iterator(*this, bucket_index, chain_iterator) : end();
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Find(const TKey& key) const
	{
		return const_cast<HashMap*>(this)->Find(key);
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline std::tuple<bool, typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator>
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Insert(const PairType& pair)
	{
		auto [was_found, bucket_index, chain_iterator] = KeySearch(pair.first);
		if (was_found)
		{
			return std::make_tuple(false, Iterator(*this, bucket_index, chain_iterator));
		}

		ChainIteratorType it_new = buckets.at(bucket_index).PushBack(pair);
		++size;

		return std::make_tuple(true, Iterator(*this, bucket_index, it_new));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline std::tuple<bool, typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator>
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Insert(PairType&& pair)
	{
		auto [was_found, bucket_index, chain_iterator] = KeySearch(pair.first);
		if (was_found)
		{
			return std::make_tuple(false, Iterator(*this, bucket_index, chain_iterator));
		}

		ChainIteratorType it_new = buckets.at(bucket_index).PushBack(std::move(pair));
		++size;

		return std::make_tuple(true, Iterator(*this, bucket_index, it_new));
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::operator[](const TKey& key)
	{
		auto [was_inserted, it] = Insert(PairType(key, TValue{}));
		return it->second;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::operator[](const TKey& key) const
	{
		return at(key);
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline void HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Remove(const TKey& key)
	{
		auto [was_found, bucket_index, chain_iterator] = KeySearch(key);
		if (was_found)
		{
			buckets.at(bucket_index).Remove(chain_iterator);
			--size;
		}
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline void HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Resize(const size_t new_size)
	{
		HashMap map(new_size);

		for (auto& item : *this)
		{
			map.Insert(std::move(item));
		}

		*this = std::move(map);
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline void HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Clear()
	{
		for (size_t i = 0_z; i < buckets.Size(); ++i)
		{
			buckets.at(i).Clear();
		}

		size = 0_z;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline size_t HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Size() const
	{
		return size;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline size_t HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::BucketSize() const
	{
		return buckets.Size();
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ContainsKey(const TKey& key)
	{
		return (Find(key) != end());
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::at(const TKey& key)
	{
		auto [was_found, bucket_index, chain_iterator] = KeySearch(key);
		if (!was_found)
		{
			throw std::runtime_error("Value not found at this index");
		}

		return (*chain_iterator).second;
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::at(const TKey& key) const
	{
		return const_cast<HashMap*>(this)->at(key);
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::begin()
	{
		if (size != 0_z)
		{
			for (size_t index_ref = 0_z; index_ref < buckets.Size(); ++index_ref)
			{
				auto& bucket = buckets.at(index_ref);

				if (!bucket.IsEmpty())
				{
					return Iterator(*this, index_ref, bucket.begin());
				}
			}
		}

		return end();
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::end()
	{
		return Iterator(*this, buckets.Size(), buckets.at(buckets.Size() - 1).end());
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::begin() const
	{
		if (size != 0_z)
		{
			for (size_t index_ref = 0_z; index_ref < buckets.Size(); ++index_ref)
			{
				auto& bucket = buckets.at(index_ref);

				if (!bucket.IsEmpty())
				{
					return ConstIterator(*this, index_ref, bucket.begin());
				}
			}
		}

		return end();
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::cbegin() const
	{
		if (size != 0_z)
		{
			for (size_t index_ref = 0_z; index_ref < buckets.Size(); ++index_ref)
			{
				auto& bucket = buckets.at(index_ref);

				if (!bucket.IsEmpty())
				{
					return ConstIterator(*this, index_ref, bucket.begin());
				}
			}
		}

		return cend();
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::end() const
	{
		return ConstIterator(*this, buckets.Size(), buckets.at(buckets.Size() - 1).end());
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::cend() const
	{
		return ConstIterator(*this, buckets.Size(), buckets.at(buckets.Size() - 1).cend());
	}

	template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
	inline std::tuple<bool, size_t, typename SList<std::pair<const TKey, TValue>>::Iterator> 
		HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::KeySearch(const TKey& key) const
	{
		HashFunctor hash_functor{};
		EqualityFunctor eq{};

		const size_t index_ref = (hash_functor(key)) % (buckets.Size());
		ChainType& list = buckets.at(index_ref);

		ChainIteratorType it = list.begin();
		for (; it != list.end(); ++it)
		{
			if (eq((*it).first, key))
			{
				return std::make_tuple(true, index_ref, it);
			}
		}

		return std::make_tuple(false, index_ref, it);
	}

#pragma endregion HashMap
}