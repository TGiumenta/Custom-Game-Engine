#pragma once

#include "DefaultEquality.h"

namespace FieaGameEngine
{ 
	/// <summary>
	/// A class which defines a templated LinkedList
	/// Each "node" is made up of templated data and a next pointer which is a reference to the next list's element
	/// front refers to the node at the front of the list
	/// back refers to the node at the back of the list
	/// size refers to the size of the list
	/// </summary>
	template <typename T>
	class SList final
	{
		private:
			struct Node final
			{
				/// <summary>
				/// Construct a new node with templated data and creating space for the next node's reference
				/// </summary>
				/// <param name="new_data"> The templated data associated with this node </param>
				/// <param name="node_next"> The next node's reference in the list </param>
				explicit Node(const T& new_data, Node* node_next = nullptr);

				/// <summary>
				/// Construct a new node with templated data and creating space for the next node's reference
				/// </summary>
				/// <param name="new_data"> The templated data associated with this node </param>
				/// <param name="node_next"> The next node's reference in the list </param>
				/// <remarks> Explicilty adds templated r-value data </remarks>
				Node(T&& data, Node* next = nullptr);

				/// <summary>
				/// Destroys this node 
				/// </summary>
				~Node() {};

				T data;
				Node* next;
			};

		public:
			using value_type = T;
			/// <summary>
			/// A templated class which defines a non-inheritable Iterator
			/// Stores a reference to the owning list and the node this iterator is referring to
			/// This will be mainly used to traverse the list 
			/// </summary>
			class Iterator final
			{
				friend SList;
				friend class ConstIterator;

			public:
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using value_type = T;
				using reference = T;
				using pointer = T*;
				using iterator_category = std::forward_iterator_tag;

				/// <summary>
				/// The default constructor with a const list and a node pointer 
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
				/// <returns> The lhs list (this) after equalizing them </returns>
				Iterator& operator=(const Iterator& other) = default;
				
				/// <summary>
				/// The default equality for Iterators
				/// </summary>
				/// <param name = "other"> The Iterator to equate this Iterator to </param>
				/// <remarks> Equality operator uses r-values instead of l-values </remarks>
				/// <returns> The lhs list (this) after equalizing them </returns>
				Iterator& operator=(Iterator&& other) noexcept = default;
				
				/// <summary>
				/// The default destructor, destroying the Iterator
				/// </summary>
				~Iterator() = default;

				/// <summary>
				/// Determines if two Iterator's are the same (owner and node)
				/// </summary>
				/// <param name = "other"> The Iterator to compare this too </param>
				/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
				/// <returns> A boolean to indicate if two Iterator's are equivalent </returns>
				bool operator==(const Iterator& other) const;

				/// <summary>
				/// Determines if two Iterator's are not the same (owner and node)
				/// </summary>
				/// <param name = "other"> The Iterator to compare this too </param>
				/// <returns> A boolean to indicate if two Iterator's are not the same </returns>
				bool operator!=(const Iterator& other) const;

				/// <summary>
				/// Increments a Iterator by altering the node pointer to look at its next
				/// </summary>
				/// <remarks> Specifically handles the prefix increment case </remarks>
				/// <exception cref="std::runtime_error"> If the owning list is null </exception>
				/// <returns> The altered Iterator </returns>
				Iterator& operator++();

				/// <summary>
				/// Increments a Iterator by altering the node pointer to look at its next
				/// </summary>
				/// <remarks> Specifically handles the postfix increment case </remarks>
				/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
				/// <returns> A copy of the old Iterator </returns>
				Iterator operator++(int);

				/// <summary>
				/// The overloaded dereference operator which retrieves the Iterator's node's data
				/// </summary>
				/// <exception cref="std::runtime_error"> If the node is null (or = to end()) </exception>
				/// <remarks> Does not alter the list in the iterator </remarks>
				/// <returns> The data that the Iterator's node points at </returns>
				T& operator*() const;

			private:
				/// <summary>
				/// Private constructor that's used to assign lists and access nodes from only within the SList class
				/// <param name = "owner"> The list that will own this Iterator </param>
				/// <param name = "node"> The node this Iterator is pointing at </param>
				/// </summary>
				Iterator(const SList& owner, Node* node);

				const SList* owner = nullptr;
				Node* current = nullptr;		// Use this to mutate the list
			};

			using value_type = T;
			/// <summary>
			/// A templated class which defines a non-inheritable ConstIterator
			/// Stores a reference the owning list and the node this iterator is referring to
			/// The difference between this and a regular iterator, is that its node is const and cannot be mutated
			/// This will be mainly used to traverse the list 
			/// </summary>
			class ConstIterator final
			{
				friend SList;

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
				/// <param name="other"> The original iterator to copy into a ConstIterator </param>
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
				/// <returns> The lhs list (this) after equalizing them </returns>
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
				/// Determines if two ConstIterator's are the same (owner and node)
				/// </summary>
				/// <param name = "other"> The ConstIterator to compare this too </param>
				/// <remarks> Used less frequently, call the inverse of the != operator </remarks>
				/// <returns> A boolean to indicate if two ConstIterator's are equivalent </returns>
				bool operator==(const ConstIterator& other) const;
				
				/// <summary>
				/// Determines if two ConstIterator's are not the same (owner and node)
				/// </summary>
				/// <param name = "other"> The ConstIterator to compare this too </param>
				/// <returns> A boolean to indicate if two ConstIterator's are not the same </returns>
				bool operator!=(const ConstIterator& other) const;

				/// <summary>
				/// Increments a ConstIterator by altering the node pointer to look at its next
				/// </summary>
				/// <remarks> Specifically handles the prefix increment case </remarks>
				/// <exception cref="std::runtime_error"> If the owning list is null </exception>
				/// <returns> The altered ConstIterator </returns>
				ConstIterator& operator++();

				/// <summary>
				/// Increments a ConstIterator by altering the node pointer to look at its next
				/// </summary>
				/// <remarks> Specifically handles the postfix increment case </remarks>
				/// <remarks> Fake parameter to this operator, it doesn't exist. Must be an int </remarks>
				/// <returns> A copy of the old ConstIterator </returns>
				ConstIterator operator++(int);

				/// <summary>
				/// The overloaded dereference operator which retrieves the ConstIterator's node's data
				/// </summary>
				/// <exception cref="std::runtime_error"> If the node is null (or = to end()) </exception>
				/// <returns> The data that the ConstIterator's node points at </returns>
				const T& operator*() const;

			private:
				/// <summary>
				/// Private constructor that's used to assign lists and access nodes from only within the SList class
				/// <param name = "owner"> The list that will own this ConstIterator </param>
				/// <param name = "node"> The node this ConstIterator is pointing at </param>
				/// </summary>
				ConstIterator(const SList& owner, Node* node);

				const SList* owner = nullptr;
				const Node* current = nullptr;	// Use this to NOT mutate the list
			};

			/// <summary>
			/// Default constructor for object initialization
			/// </summary>
			SList() = default;

			/// <summary>
			/// Copy constructor which creates a new list based on the existing other list
			/// </summary>
			/// <param name="other"> The original list to copy </param>
			SList(const SList& other);

			/// <summary>
			/// Move constructor which creates a new list based on moving the existing other list
			/// </summary>
			/// <param name="other"> The original list to copy</param>
			/// <remarks> Move constructor uses r-values instead of l-values </remarks>
			SList(SList&& other) noexcept;

			/// <summary>
			/// Sets this list equal to the other list
			/// </summary>
			/// <param name = "other"> The list to equate this list to </param>
			/// <remarks> Clears "this" list before copying </remarks>
			/// <returns> The lhs list (this) after equalizing them </returns>
			SList& operator=(const SList& other);

			/// <summary>
			/// Move assignment equality operator for lists
			/// </summary>
			/// <param name = "other"> The list to equate this list to </param>
			/// <remarks> Assignment operator uses r-values instead of l-values </remarks>
			/// <returns> The lhs list (this) after moving the "other" list </returns>
			SList& operator=(SList&& other) noexcept;

			/// <summary>
			/// Destructor for the list, pops all nodes to completion
			/// </summary>
			~SList();

			/// <summary>
			/// Adds a new node to the front of the list
			/// </summary>
			/// <param name="other_data">The data object that will associated with the node </param>
			/// <returns> An iterator that contains this node and the current list </returns>
			Iterator PushFront(const T& other_data);

			/// <summary>
			/// Adds a new node to the front of the list
			/// </summary>
			/// <param name="other_data">The data object that will associated with the node </param>
			/// <remarks> Explicitly uses r-values instead of l-values </remarks>
			/// <returns> An iterator that contains this node and the current list </returns>
			Iterator PushFront(T&& other_data);

			/// <summary>
			/// Removes the front node from the list
			/// </summary>
			/// <remarks> Also sets the new front of the list </remarks>
			void PopFront();

			/// <summary>
			/// Adds a new node to the back of the list
			/// </summary>
			/// <param name="other_data">The data object that will associated with the node </param>
			/// <returns> An iterator that contains this node and the current list </returns>
			Iterator PushBack(const T& other_data);

			/// <summary>
			/// Adds a new node to the back of the list
			/// </summary>
			/// <param name="other_data">The data object that will associated with the node </param>
			/// <remarks> Explicitly uses r-values instead of l-values </remarks>
			/// <returns> An iterator that contains this node and the current list </returns>
			Iterator PushBack(T&& other_data);

			/// <summary>
			/// Queries the list and determines empty status
			/// </summary>
			/// <returns> True/False determining if the list has objects in it </returns>
			bool IsEmpty() const;

			/// <summary>
			/// Queries the list and retrieves the front's data member
			/// </summary>
			/// <exception cref="std::runtime_error"> When front is null (list is empty) </exception>
			/// <returns> A mutable reference to the front object's data member </returns>
			T& Front();

			/// <summary>
			/// Queries the list and retrieves the front's data member
			/// </summary>
			/// <remarks> This is the const version of the other Front method </remarks>
			/// <exception cref="std::runtime_error"> When front is null (list is empty) </exception>
			/// <returns> A non-mutable reference to the front object's data member </returns>
			const T& Front() const;

			/// <summary>
			/// Queries the list and retrieves the tails's data member
			/// </summary>
			/// <exception cref="std::runtime_error"> When back is null (list is empty) </exception>
			/// <returns> A mutuable reference to the back object's data member </returns>
			T& Back();
		
			/// <summary>
			/// Queries the list and retrieves the tails's data member
			/// </summary>
			/// <remarks> This is the const version of the other Back method </remarks>
			/// <exception cref="std::runtime_error"> When back is null (list is empty) </exception>
			/// <returns> A non-mutable reference to the back object's data member </returns>
			const T& Back() const;

			/// <summary>
			/// Queries the list and determines the amount of objects in the list
			/// </summary>
			/// <returns> Amount of elements in the list </returns>
			size_t Size() const;

			/// <summary>
			/// Removes all elements from the list
			/// </summary>
			void Clear();

			/// <summary>
			/// Removes the back node from the list
			/// </summary>
			/// <remarks> Also sets the new back of the list </remarks>
			void PopBack();

			/// <summary>
			/// Creates an Iterator from this list and the front node
			/// </summary>
			/// <returns> An Iterator referencing to the front of the list </returns>
			Iterator begin();

			/// <summary>
			/// Creates an Iterator from this list and PAST the back node
			/// </summary>
			/// <returns> An Iterator referencing beyond the back/end of the list </returns>
			Iterator end();

			/// <summary>
			/// Creates a ConstIterator from this list and the front node
			/// </summary>
			/// <returns> A ConstIterator referencing to the front of the list </returns>
			ConstIterator begin() const;
			
			/// <summary>
			/// Creates a ConstIterator from this list and the front node
			/// </summary>
			/// <remarks> In a non const list, you can still get a ConstIterator out of a mutable list </remarks>
			/// <returns> A ConstIterator referencing to the front of the list </returns>
			ConstIterator cbegin() const;

			/// <summary>
			/// Creates a ConstIterator from this list and PAST the back node
			/// </summary>
			/// <returns> A ConstIterator referencing past the end of the list </returns>
			ConstIterator end() const;
			
			/// <summary>
			/// Creates a ConstIterator from this list and PAST the back node
			/// </summary>
			/// <remarks> In a non const list, you can still get a const iterator out of a mutable list </remarks>
			/// <returns> A ConstIterator referencing past the end of the list </returns>
			ConstIterator cend() const;

			/// <summary>
			/// Inserts data in a specific position in the list, after the node referenced by the iterator parameter
			/// </summary>
			/// <param name="other_data"> The data associated that must be attached to the node to insert </param>
			/// <param name="it"></param>
			/// <exception cref="std::runtime_error"> If the owning lists are not equal </exception>
			/// <returns> An Iterator point to the node that was inserted </returns>
			Iterator InsertAfter(const T& other_data, const Iterator& it);
			
			/// <summary>
			/// Finds the first node with the given data in this list
			/// </summary>
			/// <param name="value"> The node's data which is to be searched for </param>
			/// <remarks> The returned Iterator will point past the end of the list if the node isn't found </remarks>
			/// <returns> An Iterator that contains a reference to the list and the node found </returns>
			template <typename EqualityFunctor = DefaultEquality<T>>
			Iterator Find(const T& value);

			/// <summary>
			/// Finds the first node with the given data in this list
			/// </summary>
			/// <param name="value"> The node's data which is to be searched for </param>
			/// <remarks> The returned area will point past the end of the list if the node isn't found </remarks>
			/// <remarks> Explicitly finds an ConstIterator from a const list </remarks>
			/// <returns> An Iterator that contains a reference to the list and the node found </returns>
			template <typename EqualityFunctor = DefaultEquality<T>>
			ConstIterator Find(const T& value) const;
			
			/// <summary>
			/// Removes a node in the list given data
			/// </summary>
			/// <param name="value"> The data to search for, and remove </param>
			/// <returns> A boolean indicating whether the node was removed or not </returns>
			template <typename EqualityFunctor = DefaultEquality<T>>
			bool Remove(const T& value);

			/// <summary>
			/// Removes a node in the list given an iterator which references the data/node
			/// </summary>
			/// <param name="value"> Contains the node in which to delete </param>
			/// <remarks> No find is required here, only logic to remove the element from the list </remarks>
			/// <exception cref="std::runtime_error"> If the iterator's owner is invalid </exception>
			/// <returns> A boolean indicating whether the node was removed or not </returns>
			bool Remove(const Iterator& it);

		private:
			/// <summary>
			/// A helper method that the copy constructor and equality overload uses to create a list
			/// </summary>
			/// <param name="other"> The list used to construct the new list</param>
			/// <returns> The new, duplicated list </returns>
			SList<T>& DeepCopyList(const SList& other);

			Node* front = nullptr;
			Node* back = nullptr;
			size_t size = 0;
	};
}

#include "SList.inl"