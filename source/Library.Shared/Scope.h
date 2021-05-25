#pragma once

#include "RTTI.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "Factory.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	/// <summary>
	/// A class which defines a non-templated scope which is simply a string, datum pair which 
	/// can be used to refer to tables of data
	/// parent refers to this scope's parent
	/// map refers an unordered map of all of elements that's appended to the scope
	/// order refers an ordered vector ofall of elements that's appended to the scope
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		using ScopePairType = std::pair<const std::string, Datum>;

		/// <summary>
		/// Constructor for object initialization
		/// <param name = "initial_capacity"> The amount of potential space in the vector </param>
		/// </summary>
		explicit Scope(const size_t initial_capacity = 0);
		
		/// <summary>
		/// Copy constructor which creates a new scope based on the existing other scope
		/// </summary>
		/// <param name="other"> The original scope to copy </param>
		Scope(const Scope& other);
		
		/// <summary>
		/// Copy constructor which creates a new scope based on the existing other scope
		/// </summary>
		/// <param name="other"> The original scope to copy </param>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		Scope(Scope&& other) noexcept;
		
		/// <summary>
		/// Default equality operator for scopes
		/// </summary>
		/// <param name = "other"> The scope to equate this scope to </param>
		/// <returns> The lhs scope (this) after equalizing them </returns>
		Scope& operator=(const Scope& other);
		
		/// <summary>
		/// Default equality operator for scopes
		/// </summary>
		/// <param name = "other"> The scope to equate this scope to </param>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs scope (this) after equalizing them </returns>
		Scope& operator=(Scope&& other) noexcept;
		
		/// <summary>
		/// Destructor for scope, clears the scopes
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Queries the vectors size
		/// </summary>
		/// <returns> The amount of elements present in the order vector </returns>
		size_t Size() const { return order.Size(); }

		/// <summary>
		/// Determines if an element is in the current scope or not based on the key parameter
		/// </summary>
		/// <param name="entry"> The key to search for</param>
		/// <returns> The datum pointer at that specific key </returns>
		Datum* Find(const std::string& entry);
		
		/// <summary>
		/// Determines if an element is in the current scope or not based on the key parameter
		/// </summary>
		/// <param name="entry"> The key to search for</param>
		/// <remarks> Specifically the const version of the non-const Find() </remarks>
		/// <returns> The datum pointer at that specific key </returns>
		const Datum* Find(const std::string& entry) const;

		/// <summary>
		/// Determines if an element is in the current scope or any of its parents based on the key parameter
		/// </summary>
		/// <param name="entry"> The key to search for</param>
		/// <returns> A tuple that contains the datum pointer at the key and a pointer to the scope it was found in </returns>
		std::tuple<Datum*, Scope*> Search(const std::string& entry);
		
		/// <summary>
		/// Determines if an element is in the current scope or any of its parents based on the key parameter
		/// </summary>
		/// <param name="entry"> The key to search for</param>
		/// <remarks> Specifically the const version of the non-const Search() </remarks>
		/// <returns> A tuple that contains the datum pointer at the key and a pointer to the scope it was found in </returns>
		const std::tuple<Datum*, Scope*> Search(const std::string& entry) const;

		/// <summary>
		/// Adds an element to the hashmap. If that element doesn't exist, also add it to the order vector
		/// Creates a datum that has an unknown type
		/// </summary>
		/// <param name="entry"> The key to append </param>
		/// <exception cref="std::invalid_argument"> If the key is the empty string </exception>
		/// <returns> A Datum with type unknown (the value in the pair) </returns>
		Datum& Append(const std::string& entry);

		/// <summary>
		/// Adds an element to the hashmap. If that element doesn't exist, also add it to the order vector
		/// Creates a datum that has an unknown type
		/// </summary>
		/// <param name="entry"> The key to append </param>
		/// <param name="entry_created"> An out parameter to determine if an entry was created </param>
		/// <exception cref="std::invalid_argument"> If the key is the empty string </exception>
		/// <returns> A Datum with type unknown (the value in the pair) </returns>
		Datum& Append(const std::string& entry, bool& entry_created);
		
		/// <summary>
		/// Adds an element to the hashmap. If that element doesn't exist, also add it to the order vector
		/// Creates a datum that has a type Table
		/// Childs a scope to this scope (must be created here)
		/// </summary>
		/// <param name="entry"> The key to append </param>
		/// <exception cref="std::invalid_argument"> If the key is the empty string </exception>
		/// <returns> A Scope with type Table (the value in the pair) </returns>
		Scope& AppendScope(const std::string& entry);

		/// <summary>
		/// Adds an element to the hashmap. If that element doesn't exist, also add it to the order vector
		/// Creates a datum that has a type Table
		/// Childs a scope to this scope (parameterized)
		/// </summary>
		/// <param name="child"> The child to adopt </param>
		/// <param name="entry"> The key to append </param>
		/// <exception cref="std::invalid_argument"> If the child is trying to adopt itself (self adoption) </exception>
		/// <exception cref="std::invalid_argument"> If the child is also the parent (ancestor adoption) </exception>
		/// <exception cref="std::invalid_argument"> If the key is the empty string </exception>
		void Adopt(Scope& child, const std::string& entry);

		/// <summary>
		/// Queries the current scope and retrieves the parent
		/// </summary>
		/// <returns> The parent pointer </returns>
		Scope* GetParent() const { return parent; }

		/// <summary>
		/// Wrapper for Append() for convenient syntax
		/// </summary>
		/// <param name="entry"> The key to append </param>
		/// <returns> A Datum with type unknown (the value in the pair) </returns>
		Datum& operator[](const std::string& entry) { return Append(entry); }
		
		/// <summary>
		/// Queries the order vector and returns the value associated with the key at that index
		/// </summary>
		/// <param name="index"> The index to query into the vector </param>
		/// <returns> The value at the index of the order vector </returns>
		Datum& operator[](const uint32_t index) { return order[index]->second; }
		
		/// <summary>
		/// Queries the order vector and returns the value associated with the key at that index
		/// </summary>
		/// <param name="index"> The index to query into the vector </param>
		/// <remarks> Specifically the const version of the non-const operator[] </remarks>
		/// <returns> The value at the index of the order vector </returns>
		const Datum& operator[](const uint32_t index) const { return order[index]->second; }

		/// <summary>
		/// Determines if two Scopes are identical; wraps the overridden Equals method
		/// </summary>
		/// <param name = "other"> The Scope to compare this too </param>
		/// <returns> A boolean to indicate if two Scopes are equivalent </returns>
		bool operator==(const Scope& other) const { return Equals(&other); }
		
		/// <summary>
		/// Determines if two Scopes are not identical; wraps the overridden Equals method
		/// </summary>
		/// <param name = "other"> The Scope to compare this too </param>
		/// <remarks> Used less frequently, call the inverse of the == operator </remarks>
		/// <returns> A boolean to indicate if two Scopes are equivalent </returns>
		bool operator!=(const Scope& other) const { return !(operator==(other)); }

		/// <summary>
		/// Finds the parameterized scope within the current scope by going down to its children scopes and searching 
		/// </summary>
		/// <param name="other"> The scope to find </param>
		/// <returns> A tuple which contains the datum (of type Table) which refers to the other scope, and the index of that datum </returns>
		std::tuple<Datum*, size_t> FindContainedScope(const Scope& other) const;

		/// <summary>
		/// Deletes all the nested scopes within this scopes
		/// <remarks> Both the map and vector will clear its members on destruction, no need to repeat here </remarks>
		/// </summary>
		void Clear();

		/// <summary>
		/// Determines if two Scopes are equal
		/// </summary>
		/// <param name = "other"> The Scope to compare this too </param>
		/// <remarks> Specifically </remarks>
		/// <returns> A boolean to indicate if two Scopes are equivalent </returns>
		bool Equals(const RTTI* rhs) const override; // Order matters

		/// <summary>
		/// Removes the scope at the current index and removes the reference to the parent
		/// <remarks> This does not reparent this scope, only removes it from its parent </remarks>
		/// </summary>
		void Orphan();

		/// <summary>
		/// Determines if "this' scope is an ancestor of the other scope
		/// </summary>
		/// <param name="other"> The potential descendant of "this" scope </param>
		/// <remarks> Simply calls IsDescendant on other </remarks>
		/// <returns> Determines ancestry between "this" and the other scope </returns>
		bool IsAncestor(const Scope& other) const;
		
		/// <summary>
		/// Determines if "this' scope is a descendant of the other scope
		/// </summary>
		/// <param name="other"> The potential ancestor of "this" scope </param>
		/// <returns> Determines if "this" is a distant child of the other scope </returns>
		bool IsDescendant(const Scope& other) const;

		/// <summary>
		/// Clones this current scope on the heap
		/// </summary>
		/// <returns> A pointer to the new heap allocated scope, meant to be deleted by the caller </returns>
		virtual gsl::owner<Scope*> Clone() const { return new Scope(*this); }

	protected:
		Scope* parent = nullptr;
		HashMap<std::string, Datum> map;
		Vector<HashMap<const std::string, Datum>::PairType*> order;

		/// <summary>
		/// Handles the logic for moving the parameter scope into this one
		/// </summary>
		/// <param name="other"> The scope to move </param>
		void MoveScope(Scope&& other);

		/// <summary>
		/// Handles the logic copying the parameter scope into this one
		/// </summary>
		/// <param name="other"> The scope to copy </param>
		void DeepCopyScope(const Scope& other);

		using NestedScopeFunction = std::function<bool(const Scope&, Datum&, size_t)>;
		
		/// <summary>
		/// Wraps a commonly-used mechanism to iterate through nested scopes
		/// </summary>
		/// <param name="func"> The function to call on each nested scope </param>
		void ForEachNestedScopeIn(NestedScopeFunction func) const;
	};

	ConcreteFactory(Scope, Scope)
}