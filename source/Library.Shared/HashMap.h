#pragma once

#include "DefaultHash.h"
#include "Vector.h"
#include "SList.h"
#include "SizeLiteral.h"
#include <tuple>
#include <utility>

namespace FieaGameEngine
{
	template <typename TKey, typename TValue, typename HashFunctor = DefaultHash<TKey>, typename EqualityFunctor = DefaultEquality<TKey>>
	class HashMap
	{
	public:
		using PairType = std::pair<const TKey, TValue>;
		using value_type = PairType;

	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIteratorType = typename ChainType::Iterator;
		using ConstChainIteratorType = typename ChainType::ConstIterator;

	public:
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:
			/// <summary>
			/// The default constructor with an HashMap owner reference to the vector index and SList iterator 
			/// </summary>
			Iterator() = default;
			
			/// <summary>
			/// The default copy constructor which creates a new Iterator based on the existing other Iterator
			/// </summary>
			/// <param name="other"> The original Iterator to copy </param>
			Iterator(const Iterator& other) = default;
			
			/// <summary>
			/// The default copy constructor which creates a new Iterator based on the existing other Iterator
			/// </summary>
			/// <param name="other"> The original Iterator to copy </param>
			/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
			Iterator(Iterator&& other) noexcept = default;
			
			/// <summary>
			/// The default equality for Iterators
			/// </summary>
			/// <param name = "other"> The Iterator to equate this Iterator to </param>
			/// <returns> The lhs hashmap (this) after equalizing them </returns>
			Iterator& operator=(const Iterator& other) = default;
			
			/// <summary>
			/// The default equality for Iterators
			/// </summary>
			/// <param name = "other"> The Iterator to equate this Iterator to </param>
			/// <remarks> Equality operator uses r-values instead of l-values </remarks>
			/// <returns> The lhs hashmap (this) after equalizing them </returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			
			/// <summary>
			/// The default destructor, destroying the Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Determines if two Iterator's are the same (owner, index, and list iterator)
			/// </summary>
			/// <param name = "other"> The Iterator to compare this too </param>
			/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
			/// <returns> A boolean to indicate if two Iterator are equivalent </returns>
			bool operator==(const Iterator& other) const;
			
			/// <summary>
			/// Determines if two Iterator's are not the same (owner, index, and list iterator)
			/// </summary>
			/// <param name = "other"> The Iterator to compare this too </param>
			/// <returns> A boolean to indicate if two Iterators are not the same </returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Increments a Iterator by altering the list Iterator to look at the next element in that list
			/// </summary>
			/// <remarks> Specifically handles the prefix increment case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered Iterator </returns>
			Iterator& operator++();
			
			/// <summary>
			/// Increments a Iterator by altering the list Iterator to look at the next element in that list
			/// </summary>
			/// <remarks> Specifically handles the postfix increment case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old Iterator </returns>
			Iterator operator++(int);
			
			/// <summary>
			/// The overloaded dereference operator which retrieves the Iterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the owner is null </exception>
			/// <remarks> Does not alter the list in the Iterator </remarks>
			/// <returns> The data that the List's Iterator's node points at </returns>
			PairType& operator*() const;
			
			/// <summary>
			/// The overloaded dereference operator which retrieves the Iterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the node is null </exception>
			/// <remarks> Does not alter the list in the Iterator </remarks>
			/// <returns> A pointer to the data that the Iterator's vector (at index) points at </returns>
			PairType* operator->() const;

		private:
			/// <summary>
			/// Private constructor that's used to assign hashmaps, indexes, and list Iterators from only within the HashMap class
			/// <param name = "owner"> The HashMap that will own this Iterator </param>
			/// <param name = "index"> The index in the vector this Iterator has a reference to </param>
			/// <param name = "list_it"> The list Iterator this Iterator has a reference to </param>
			/// </summary>
			Iterator(HashMap& owner, const size_t new_index, const ChainIteratorType& list_it);

			HashMap* owner = nullptr;
			size_t index = 0_z;
			ChainIteratorType list_iterator;
		};

		class ConstIterator final
		{
			friend HashMap;

		public:
			/// <summary>
			/// The default constructor with an HashMap owner reference to the vector index and SList ConstIterator 
			/// </summary>
			ConstIterator() = default;
			
			/// <summary>
			/// A copy constructor which creates a ConstIterator based on an ITERATOR
			/// </summary>
			/// <param name="other"> The original Iterator to copy into a ConstIterator </param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// The default copy constructor which creates a new ConstIterator based on the existing other ConstIterator
			/// </summary>
			/// <param name="other"> The original Iterator to copy </param>
			ConstIterator(const ConstIterator& other) = default;
			
			/// <summary>
			/// The default copy constructor which creates a new ConstIterator based on the existing other ConstIterator
			/// </summary>
			/// <param name="other"> The original ConstIterator to copy </param>
			/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
			ConstIterator(ConstIterator&& other) noexcept = default;
			
			/// <summary>
			/// The default equality for ConstIterators
			/// </summary>
			/// <param name = "other"> The ConstIterator to equate this ConstIterator to </param>
			/// <returns> The lhs hashmap (this) after equalizing them </returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			
			/// <summary>
			/// The default equality for ConstIterators
			/// </summary>
			/// <param name = "other"> The ConstIterator to equate this ConstIterator to </param>
			/// <remarks> Equality operator uses r-values instead of l-values </remarks>
			/// <returns> The lhs hashmap (this) after equalizing them </returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;
			
			/// <summary>
			/// The default destructor, destroying the ConstIterator
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Determines if two ConstIterator's are the same (owner, index, and list iterator)
			/// </summary>
			/// <param name = "other"> The ConstIterator to compare this too </param>
			/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
			/// <returns> A boolean to indicate if two ConstIterator's are equivalent </returns>
			bool operator==(const ConstIterator& other) const;
			
			/// <summary>
			/// Determines if two ConstIterator's are not the same (owner, index, and list iterator)
			/// </summary>
			/// <param name = "other"> The ConstIterator to compare this too </param>
			/// <returns> A boolean to indicate if two ConstIterator's are not the same </returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Increments a ConstIterator by altering the list ConstIterator to look at the next element in that list
			/// </summary>
			/// <remarks> Specifically handles the prefix increment case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered Iterator </returns>
			ConstIterator& operator++();
			
			/// <summary>
			/// Increments a ConstIterator by altering the list ConstIterator to look at the next element in that list
			/// </summary>
			/// <remarks> Specifically handles the postfix increment case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old ConstIterator </returns>
			ConstIterator operator++(int);

			/// <summary>
			/// The overloaded dereference operator which retrieves the ConstIterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the owner is null </exception>
			/// <remarks> Does not alter the list in the Iterator </remarks>
			/// <returns> The data that the List's ConstIterator's node points at </returns>
			const PairType& operator*() const;
			
			/// <summary>
			/// The overloaded dereference operator which retrieves the ConstIterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the owner is null </exception>
			/// <remarks> Does not alter the list in the Iterator </remarks>
			/// <returns> A pointer to the data that the Iterator's vector (at index) points at </returns>
			const PairType* operator->() const;

		private:
			/// <summary>
			/// Private constructor that's used to assign hashmaps, indexes, and list Iterators from only within the HashMap class
			/// <param name = "owner"> The HashMap that will own this ConstIterator </param>
			/// <param name = "index"> The index in the vector this ConstIterator has a reference to </param>
			/// <param name = "list_it"> The list Iterator this ConstIterator has a reference to </param>
			/// </summary>
			ConstIterator(const HashMap& owner, const size_t new_index, const ConstChainIteratorType& list_it);

			const HashMap* owner = nullptr;
			size_t index = 0_z;
			ConstChainIteratorType list_constiterator;
		};

		/// <summary>
		/// Constructor for object initialization
		/// <param name = "bucket_size"> The amount of space to initially allocate to the vector of lists </param>
		/// </summary>
		/// <remarks> Simply calls resize on the vector class to create the space for all the lists and equate size/capacity </remarks>
		explicit HashMap(const size_t bucket_size = 11_z);

		/// <summary>
		/// Copy constructor which creates a new hashmap based on the existing other hashmap
		/// </summary>
		/// <param name="other"> The original hashmap to copy </param>
		HashMap(const HashMap& other) = default;
		
		/// <summary>
		/// Copy constructor which creates a new hashmap based on the existing other hashmap
		/// </summary>
		/// <param name="other"> The original hashmap to copy </param>
		/// <remarks> Copy constructor uses rvalues instead of lvalues </remarks>
		HashMap(HashMap&& other) noexcept = default;
		
		/// <summary>
		/// Default equality operator for hashmaps
		/// </summary>
		/// <param name = "other"> The hashmap to equate this hashmap to </param>
		/// <returns> The lhs hashmap (this) after equalizing them </returns>
		HashMap& operator=(const HashMap& other) = default;
		
		/// <summary>
		/// Default equality operator for hashmaps
		/// </summary>
		/// <param name = "other"> The hashmap to equate this hashmap to </param>
		/// <remarks> Equality operator uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs hashmap (this) after equalizing them </returns>
		HashMap& operator=(HashMap&& other) noexcept = default;
		
		/// <summary>
		/// Defaulted destructor
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Finds the value associated with the key 
		/// </summary>
		/// <param name="key"> The key associated with the pair that could be in the hashmap </param>
		/// <remarks> The returned Iterator will point past the end of the list if the key isn't found </remarks>
		/// <returns> An Iterator that contains a reference to the hashmap, the index of the data, and the Iterator pointing at the list </returns>
		Iterator Find(const TKey& key);
		
		/// <summary>
		/// Finds the value associated with the key 
		/// </summary>
		/// <param name="key"> The key associated with the pair that could be in the hashmap </param>
		/// <remarks> The returned ConstIterator will point past the end of the list if the key isn't found </remarks>
		/// <returns> A ConstIterator that contains a reference to the hashmap, the index of the data, and the ConstIterator pointing at the list </returns>
		ConstIterator Find(const TKey& key) const;
		
		/// <summary>
		/// Adds a new element to the hashmap if the pair is not found
		/// </summary>
		/// <param name="other_data">The data object that will associated with the index </param>
		/// <returns> A tuple with a bool indicating whether or not the key was inserted, and an
		///  iterator that contains this index and the current Hashmap </returns>
		std::tuple<bool, Iterator> Insert(const PairType& pair);
		
		/// <summary>
		/// Adds a new element to the hashmap if the pair is not found
		/// </summary>
		/// <param name="other_data">The data object that will associated with the index </param>
		/// <remarks> Explicitly uses r-values instead of l-values </remarks>
		/// <returns> A tuple with a bool indicating whether or not the key was inserted, and an
		///  iterator that contains this index and the current Hashmap </returns>
		std::tuple<bool, Iterator> Insert(PairType&& pair);
		
		/// <summary>
		/// Finds the value associated with the key 
		/// </summary>
		/// <param name="key"> The key associated with the pair that could be in the hashmap </param>
		/// <returns> A TValue reference to the value found OR inserted (if the key is not found) </returns>
		TValue& operator[](const TKey& key);
		
		/// <summary>
		/// Finds the value associated with the key 
		/// </summary>
		/// <param name="key"> The key associated with the pair that could be in the hashmap </param>
		/// <remarks> Simply calls at() which calls Find </remarks>
		/// <returns> A TValue reference to the value found </returns>
		TValue& operator[](const TKey& key) const;
		
		/// <summary>
		/// Removes a data member in the hashmap given some key, and does nothing if it isn't found
		/// </summary>
		/// <param name="key"> The key to search for, and remove </param>
		/// <remarks> Calls find and then immediately calls the iterator overload for Remove </remarks>
		/// <returns> A boolean indicating whether the data element was removed or not </returns>
		void Remove(const TKey& key);

		/// <summary>
		/// Resizes the map to be of the parameter's size. Creates a new map and re-hashes all of them.
		/// </summary>
		/// <param name="new_size"> The new size of the map on initialize </param>
		void Resize(const size_t new_size);
		
		/// <summary>
		/// Removes all elements from the hashmap
		/// </summary>
		void Clear();
		
		/// <summary>
		/// Queries the hashmap and determines the amount of objects in the hashmap
		/// </summary>
		/// <returns> Amount of elements in the hashmap </returns>
		size_t Size() const;

		/// <summary>
		/// Queries the hashmap and retrieves the number of buckets that is allocated for the hashmap
		/// </summary>
		/// <returns> Amount of buckets allocated for the hashmap </returns>
		size_t BucketSize() const;
		
		/// <summary>
		/// Queries the hashmap by calling find and comparing it to the end of the map
		/// </summary>
		/// <param name = "key"> The key to query </param>
		/// <returns> A boolean indicating whether or not the key was found </returns>
		bool ContainsKey(const TKey& key);
		
		/// <summary>
		/// Queries the hashmap and retrieves the data member at the parameter's key
		/// </summary>
		/// <param name = "key"> The key to query </param>
		/// <exception cref="std::runtime_error"> If the key is not found </exception>
		/// <returns> A mutable reference to the value of associated with the key </returns>
		TValue& at(const TKey& key);

		/// <summary>
		/// Queries the hashmap and retrieves the data member at the parameter's key
		/// </summary>
		/// <param name = "key"> The key to query </param>
		/// <remarks> This is the const version of the other at method </remarks>
		/// <returns> A mutable reference to the value of associated with the key </returns>
		TValue& at(const TKey& key) const;

		/// <summary>
		/// Creates an Iterator from this hashmap and the first element of the hashmap
		/// </summary>
		/// <returns> An Iterator referencing to the front of the hashmap </returns>
		Iterator begin();
		
		/// <summary>
		/// Creates an Iterator from this hashmap and PAST the last element of the hashmap
		/// </summary>
		/// <returns> An Iterator referencing beyond the back/end of the hashmap </returns>
		Iterator end();
		
		/// <summary>
		/// Creates a ConstIterator from this hashmap and the first element of the hashmap
		/// </summary>
		/// <returns> A ConstIterator referencing to the front of the hashmap </returns>
		ConstIterator begin() const;
		
		/// <summary>
		/// Creates a ConstIterator from this hashmap and the first element of the hashmap
		/// </summary>
		/// <remarks> In a non-const vector, you can still get a ConstIterator out of a mutable hashmap </remarks>
		/// <returns> A ConstIterator referencing to the front of the hashmap </returns>
		ConstIterator cbegin() const;
		
		/// <summary>
		/// Creates a ConstIterator from this hashmap and PAST the last element of the hashmap
		/// </summary>
		/// <returns> A ConstIterator referencing beyond the back/end of the hashmap </returns>
		ConstIterator end() const;
		
		/// <summary>
		/// Creates a ConstIterator from this hashmap and PAST the last element of the hashmap
		/// </summary>
		/// <remarks> In a non-const hashmap, you can still get a ConstIterator out of a mutable hashmap </remarks>
		/// <returns> A ConstIterator referencing beyond the back/end of the hashmap </returns>
		ConstIterator cend() const;

	private:
		std::tuple<bool, size_t, ChainIteratorType> KeySearch(const TKey& key) const;

		BucketType buckets;
		size_t size = 0_z;
	};
}

#include "HashMap.inl"