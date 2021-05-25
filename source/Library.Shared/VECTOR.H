#pragma once

#include "DefaultEquality.h"
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A class which defines a templated Vector. 
	/// Stores a reference to the array that's used, the size of the vector, and the capacity
	/// Size refers to the amount of elements in the vector
	/// Capacity refers to the amount of space that's available in the vector
	/// </summary>
	template <typename T>
	class Vector final
	{
	public:
		using value_type = T;

		/// <summary>
		/// A templated class which defines a non-inheritable Iterator
		/// Stores a reference to the owning vector and the index this iterator is referring to
		/// This will be mainly used to traverse the vector 
		/// </summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::random_access_iterator_tag;

			/// <summary>
			/// The default constructor with a reference to the vector and the index (position) 
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
			/// <returns> The lhs vector (this) after equalizing them </returns>
			Iterator& operator=(const Iterator& other) = default;
			
			/// <summary>
			/// The default equality for Iterators
			/// </summary>
			/// <param name = "other"> The Iterator to equate this Iterator to </param>
			/// <remarks> Equality operator uses r-values instead of l-values </remarks>
			/// <returns> The lhs vector (this) after equalizing them </returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			
			/// <summary>
			/// The default destructor, destroying the Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Determines if two Iterator's are the same (owner and index)
			/// </summary>
			/// <param name = "other"> The Iterator to compare this too </param>
			/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
			/// <returns> A boolean to indicate if two Iterator's are equivalent </returns>
			bool operator==(const Iterator& other) const;
			
			/// <summary>
			/// Determines if two Iterator's are not the same (owner and index)
			/// </summary>
			/// <param name = "other"> The Iterator to compare this too </param>
			/// <returns> A boolean to indicate if two Iterator's are not the same </returns>
			bool operator!=(const Iterator& other) const;
			
			/// <summary>
			/// Increments a Iterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the prefix increment case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered Iterator </returns>
			Iterator& operator++();
			
			/// <summary>
			/// Increments a Iterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the postfix increment case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old Iterator </returns>
			Iterator operator++(int);

			/// <summary>
			/// Decrements a Iterator by altering the index to look at the previous element
			/// </summary>
			/// <remarks> Specifically handles the prefix decrement case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered Iterator </returns>
			Iterator& operator--();
			
			/// <summary>
			/// Decrements a Iterator by altering the index to look at the previous element
			/// </summary>
			/// <remarks> Specifically handles the postfix decrement case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old Iterator </returns>
			Iterator operator--(int);

			/// <summary>
			/// The overloaded dereference operator which retrieves the Iterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the node is null </exception>
			/// <remarks> Does not alter the vector in the iterator </remarks>
			/// <returns> The data that the Iterator's vector (at index) points at </returns>
			T& operator*() const;

			/// <summary>
			/// The overloaded dereference operator which retrieves the Iterator's data at the index
			/// </summary>
			/// <exception cref="std::runtime_error"> If the node is null </exception>
			/// <remarks> Does not alter the vector in the iterator </remarks>
			/// <returns> A pointer to the data that the Iterator's vector (at index) points at </returns>
			T* operator->() const;

		private:
			/// <summary>
			/// Private constructor that's used to assign vectors and indexes from only within the Vector class
			/// <param name = "owner"> The vector that will own this Iterator </param>
			/// <param name = "index"> The index in the vector this Iterator has a reference to </param>
			/// </summary>
			Iterator(Vector& owner, const size_t new_index);

			Vector* owner = nullptr;
			size_t index = 0_z;
		};

		using value_type = T;

		/// <summary>
		/// A templated class which defines a non-inheritable Iterator
		/// Stores a reference to the owning vector and the index this iterator is referring to
		/// The difference between this and a regular iterator, is that its vector is const and cannot be mutated
		/// This will be mainly used to traverse the vector 
		/// </summary>
		class ConstIterator final
		{
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::random_access_iterator_tag;

			/// <summary>
			/// The default constructor with a const list and a node pointer 
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
			/// <param name="other"> The original ConstIterator to copy </param>
			ConstIterator(const ConstIterator& other) = default;
			
			/// <summary>
			/// The default copy constructor which creates a new ConstIterator based on the existing other ConstIterator
			/// </summary>
			/// <param name="other"> The original ConstIterator to copy </param>
			/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
			ConstIterator(ConstIterator&& other) noexcept = default;
			
			/// <summary>
			/// An overloaded equality for ConstIterators
			/// </summary>
			/// <param name = "other"> The ConstIterator to equate this ConstIterator to </param>
			/// <returns> The lhs vector (this) after equalizing them </returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			
			/// <summary>
			/// An overloaded equality for ConstIterators
			/// </summary>
			/// <param name = "other"> The ConstIterator to equate this ConstIterator to </param>
			/// <remarks> Equality operator uses r-values instead of l-values </remarks>
			/// <returns> The lhs list (this) after equalizing them </returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;

			/// <summary>
			/// The default destructor, destroying the ConstIterator
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Determines if two ConstIterator's are the same (owner and index)
			/// </summary>
			/// <param name = "other"> The ConstIterator to compare this too </param>
			/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
			/// <returns> A boolean to indicate if two ConstIterator's are equivalent </returns>
			bool operator==(const ConstIterator& other) const;
			
			/// <summary>
			/// Determines if two ConstIterator's are not the same (owner and index)
			/// </summary>
			/// <param name = "other"> The ConstIterator to compare this too </param>
			/// <returns> A boolean to indicate if two ConstIterator's are not the same </returns>
			bool operator!=(const ConstIterator& other) const;
			
			/// <summary>
			/// Increments a Iterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the prefix increment case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered ConstIterator </returns>
			ConstIterator& operator++();
			
			/// <summary>
			/// Increments a ConstIterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the postfix increment case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old ConstIterator </returns>
			ConstIterator operator++(int);
			
			/// <summary>
			/// Decrements a Iterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the prefix decrement case </remarks>
			/// <exception cref="std::runtime_error"> If the owning list is null </exception>
			/// <returns> The altered ConstIterator </returns>
			ConstIterator& operator--();
			
			/// <summary>
			/// Decrements a ConstIterator by altering the index to look at the next element
			/// </summary>
			/// <remarks> Specifically handles the postfix decrement case </remarks>
			/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
			/// <returns> A copy of the old ConstIterator </returns>
			ConstIterator operator--(int);
			
			/// <summary>
			/// The overloaded dereference operator which retrieves the ConstIterator's index's data
			/// </summary>
			/// <exception cref="std::runtime_error"> If the owner is null </exception>
			/// <remarks> Does not alter the vector in the iterator </remarks>
			/// <returns> The data that the ConstIterator's index points at </returns>
			const T& operator*() const;

			/// <summary>
			/// The overloaded dereference operator which retrieves the ConstIterator's node's data
			/// </summary>
			/// <exception cref="std::runtime_error"> If the node is null </exception>
			/// <remarks> Does not alter the vector in the iterator </remarks>
			/// <returns> A pointer to the data that the Iterator's node points at </returns>
			const T* operator->() const;

		private:
			/// <summary>
			/// Private constructor that's used to assign vectors and indexes from only within the Vector class
			/// <param name = "owner"> The vector that will own this ConstIterator </param>
			/// <param name = "new_index"> The index in the vector this ConstIterator has a reference to </param>
			/// </summary>
			ConstIterator(const Vector& owner, const size_t new_index);

			const Vector* owner = nullptr;
			size_t index = 0_z;
		};

		/// <summary>
		/// Constructor for object initialization
		/// <param name = "new_capacity"> The amount of space to initially allocate to the vector </param>
		/// </summary>
		/// <remarks> Calls reserve if the paramater is greater than 0 </remarks>
		Vector(const size_t new_capacity = 0_z);
		
		/// <summary>
		/// Constructor for object initialization
		/// <param name = "init_list"> The amount of space to initially allocate to the vector </param>
		/// </summary>
		/// <remarks> Calls reserve if the paramater is greater than 0 </remarks>
		Vector(std::initializer_list<T> init_list);
		
		/// <summary>
		/// Copy constructor which creates a new vector based on the existing other vector
		/// </summary>
		/// <param name="other"> The original vector to copy </param>
		Vector(const Vector& other);
		
		/// <summary>
		/// Copy constructor which creates a new vector based on the existing other vector
		/// </summary>
		/// <param name="other"> The original vector to copy</param>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		Vector(Vector&& other) noexcept;
		
		/// <summary>
		/// Sets this vector equal to the other vector
		/// </summary>
		/// <param name = "other"> The vector to equate this vector to </param>
		/// <remarks> Clears and shrinks "this" vector before copying </remarks>
		/// <returns> The lhs vector (this) after equalizing them </returns>
		Vector& operator=(const Vector& other);
		
		/// <summary>
		/// An overloaded equality operator for vectors
		/// </summary>
		/// <param name = "other"> The vector to equate this vector to </param>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs vector (this) after equalizing them </returns>
		Vector& operator=(Vector&& other) noexcept;
		
		/// <summary>
		/// Destructor for the vector, clears the vector
		/// </summary>
		~Vector();

		/// <summary>
		/// Queries the vector and retrieves the data member at the parameter's search_index
		/// </summary>
		/// <param name = "search_index"> The index to query </param>
		/// <exception cref="std::runtime_error"> When the parameter is out of range </exception>
		/// <returns> A mutable reference to the object's data member at the search_index </returns>
		T& operator[](const size_t search_index);
		
		/// <summary>
		/// Queries the vector and retrieves the data member at the parameter's search_index
		/// </summary>
		/// <param name = "search_index"> The index to query </param>
		/// <remarks> This is the const version of the other operator[] method </remarks>
		/// <exception cref="std::runtime_error"> When the parameter is out of range </exception>
		/// <returns> A mutable reference to the object's data member at the search_index </returns>
		T& operator[](const size_t search_index) const;
		
		/// <summary>
		/// Queries the vector and retrieves the data member at the parameter's search_index
		/// </summary>
		/// <param name = "search_index"> The index to query </param>
		/// <exception cref="std::runtime_error"> When the parameter is out of range </exception>
		/// <returns> A mutable reference to the object's data member at the search_index </returns>
		T& at(const size_t search_index);
		
		/// <summary>
		/// Queries the vector and retrieves the data member at the parameter's search_index
		/// </summary>
		/// <param name = "search_index"> The index to query </param>
		/// <remarks> This is the const version of the other at method </remarks>
		/// <exception cref="std::runtime_error"> When the parameter is out of range </exception>
		/// <returns> A mutable reference to the object's data member at the search_index </returns>
		T& at(const size_t search_index) const;

		/// <summary>
		/// Removes the last data member of the vector
		/// </summary>
		void PopBack();
			 
		/// <summary>
		/// Queries the vector and determines empty status
		/// </summary>
		/// <returns> True/False determining if the list has objects in it </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Queries the vector and retrieves the first data member of the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> When the vector is empty </exception>
		/// <returns> A mutable reference to the front object's data member </returns>
		T& Front();
		
		/// <summary>
		/// Queries the vector and retrieves the first data member of the vector
		/// </summary>
		/// <remarks> This is the const version of the other Front method </remarks>
		/// <exception cref="std::runtime_error"> When the vector is empty </exception>
		/// <returns> A mutable reference to the front object's data member </returns>
		const T& Front() const;
		
		/// <summary>
		/// Queries the vector and retrieves the last data member in the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> When the vector is empty </exception>
		/// <returns> A mutable reference to the last data member in the vector </returns>
		T& Back();
		
		/// <summary>
		/// Queries the vector and retrieves the last data member in the vector
		/// </summary>
		/// <remarks> This is the const version of the other Back method </remarks>
		/// <exception cref="std::runtime_error"> When the vector is empty </exception>
		/// <returns> A non-mutable reference to the last data member in the vector </returns>
		const T& Back() const;

		/// <summary>
		/// Queries the vector and determines the amount of objects in the vector
		/// </summary>
		/// <returns> Amount of elements in the vector </returns>
		size_t Size() const;
		
		/// <summary>
		/// Queries the vector and determines the amount of space available in the vector
		/// </summary>
		/// <returns> Amount of space available in the vector </returns>
		size_t Capacity() const;

		/// <summary>
		/// Creates an Iterator from this vector and the first element of the vector
		/// </summary>
		/// <returns> An Iterator referencing to the front of the vector </returns>
		Iterator begin();
		
		/// <summary>
		/// Creates an Iterator from this vector and PAST the last element of the vector
		/// </summary>
		/// <returns> An Iterator referencing beyond the back/end of the vector </returns>
		Iterator end();
		
		/// <summary>
		/// Creates a ConstIterator from this vector and the first element of the vector
		/// </summary>
		/// <returns> A ConstIterator referencing to the front of the vector </returns>
		ConstIterator begin() const;
		
		/// <summary>
		/// Creates a ConstIterator from this vector and the first element of the vector
		/// </summary>
		/// <remarks> In a non-const vector, you can still get a ConstIterator out of a mutable vector </remarks>
		/// <returns> A ConstIterator referencing to the front of the vector </returns>
		ConstIterator cbegin() const;
		
		/// <summary>
		/// Creates a ConstIterator from this vector and PAST the last element of the vector
		/// </summary>
		/// <returns> A ConstIterator referencing beyond the back/end of the vector </returns>
		ConstIterator end() const;
		
		/// <summary>
		/// Creates a ConstIterator from this vector and PAST the last element of the vector
		/// </summary>
		/// <remarks> In a non-const vector, you can still get a ConstIterator out of a mutable vector </remarks>
		/// <returns> A ConstIterator referencing beyond the back/end of the vector </returns>
		ConstIterator cend() const;

		/// <summary>
		/// Adds a new element to the end of the vector
		/// </summary>
		/// <param name="other_data">The data object that will associated with the index </param>
		/// <returns> An iterator that contains this index and the current vector </returns>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(const T& value);

		/// <summary>
		/// Adds a new element to the end of the vector
		/// </summary>
		/// <param name="other_data">The data object that will associated with the index </param>
		/// <remarks> Explicitly uses r-values instead of l-values </remarks>
		/// <returns> An iterator that contains this index and the current vector </returns>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(T&& value);

		/// <summary>
		/// Reallocates memory for the new array with the provided capacity as space
		/// </summary>
		/// <param name="new_capacity">The amount of capacity to reserve on this vector </param>
		/// <remarks> Increases capacity, NEVER decreases it </remarks>
		/// <remarks> Does not impact size in any way </remarks>
		void Reserve(const size_t new_capacity);

		/// <summary>
		/// Reduces the size of the vector to the parameter's size
		/// </summary>
		/// <param name="new_size">The amount of space to set size on this vector </param>
		/// <remarks> Guarantees capacity is greater than or equal to the size </remarks>
		void Resize(const size_t new_size);

		/// <summary>
		/// Finds the first index with the given data element in this vector
		/// </summary>
		/// <param name="value"> The vector's data which is to be searched for </param>
		/// <remarks> The returned Iterator will point past the end of the list if the node isn't found </remarks>
		/// <returns> An Iterator that contains a reference to the vector and the index of the data found </returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value);

		/// <summary>
		/// Finds the first index with the given data element in this vector
		/// </summary>
		/// <param name="value"> The vector's data which is to be searched for </param>
		/// <remarks> The returned ConstIterator will point past the end of the list if the node isn't found </remarks>
		/// <returns> An ConstIterator that contains a reference to the vector and the index of the data found </returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value) const;

		/// <summary>
		/// Removes all elements from the vector
		/// </summary>
		void Clear();

		/// <summary>
		/// Shrinks the data down if there is excessive capacity
		/// <remarks> If the size of the vector is already 0, frees the array </remarks>
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Removes a data member in the vector given some data value (not position index)
		/// </summary>
		/// <param name="value"> The data to search for, and remove </param>
		/// <remarks> Calls find and then immediately calls the iterator overload for Remove </remarks>
		/// <returns> A boolean indicating whether the data element was removed or not </returns>
		bool Remove(const T& value);
		
		/// <summary>
		/// Removes a data member in the vector given some data value (not position index)
		/// </summary>
		/// <param name="it"> The iterator containing the current vector and the index position to remove </param>
		/// <exception cref="std::runtime_error"> When the Iterator it has the incorrect owner </exception>
		/// <returns> A boolean indicating whether the data element was removed or not </returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes a range of data members in the vector given multiple iterators (with positions)
		/// </summary>
		/// <param name="first_it"> The iterator containing the current vector and the first index position to remove </param>
		/// <param name="last_it"> The iterator containing the current vector and the index of the vector to STOP at </param>
		/// <remarks> The last iterator's member does not get removed, ie this is inclusive,exclusive </remarks>
		/// <exception cref="std::runtime_error"> When either Iterator has the incorrect owner </exception>
		/// <returns> A boolean indicating whether the data element was removed or not </returns>
		bool Remove(const Iterator& first_it, const Iterator& last_it);
			
	private:
		/// <summary>
		/// A helper method that the copy constructor and equality overload uses to create a vector
		/// </summary>
		/// <param name="other"> The vector used to construct the new vector</param>
		/// <remarks> Reserves new space for the vector based on the other vector </remarks>
		/// <returns> The new, duplicated vector </returns>
		Vector& DeepCopyVector(const Vector& other);

		T* data = nullptr;
		size_t size = 0_z;
		size_t capacity = 0_z;
	};
}

#include "Vector.inl"