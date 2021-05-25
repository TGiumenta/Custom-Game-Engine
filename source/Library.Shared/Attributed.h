#pragma once

#include "Scope.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract base class that inherits from public scope
	/// Binds the Datum&Scope structures (which can be created at run-time) at compile time to communicate with the scripting language
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
		/// <summary>
		/// Default constructor for class initialization
		/// <remarks> Deleted to prevent the user from creating a base object with no id type</remarks>
		/// </summary>
		Attributed() = delete;

		/// <summary>
		/// Copy constructor which creates a new attributed base object based on the existing other attributed object
		/// </summary>
		/// <remarks> Must redefine the data pointer for each datum </remarks>
		/// <param name="other"> The original attributed object to copy </param>
		Attributed(const Attributed& other);
		
		/// <summary>
		/// Copy constructor which creates a new attributed base object based on the existing other attributed object
		/// </summary>
		/// <param name="other"> The original attributed object to copy </param>
		/// <remarks> Must redefine the data pointer for each datum </remarks>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		Attributed(Attributed&& other) noexcept;
		
		/// <summary>
		/// Equality operator for attributed objects
		/// </summary>
		/// <param name = "other"> The attributed object to equate this attributed object to </param>
		/// <remarks> Must redefine the data pointer for each datum </remarks>
		/// <returns> The lhs attributed object (this) after equalizing them </returns>
		Attributed& operator=(const Attributed& other);
		
		/// <summary>
		/// Equality operator for attributed objects
		/// </summary>
		/// <param name = "other"> The attributed object to equate this attributed object to </param>
		/// <remarks> Must redefine the data pointer for each datum </remarks>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs attributed object (this) after equalizing them </returns>
		Attributed& operator=(Attributed&& other) noexcept;
		
		/// <summary>
		/// Pure virtual destructor which is explicitly meant to be overriden by the derived class
		/// </summary>
		virtual ~Attributed() = 0 {};

		/// <summary>
		/// Determines whether or not the entry is in the order vector, aka it is an attribute
		/// </summary>
		/// <param name="entry"> The string to search for </param>
		/// <returns> True/false depending on whether or not the string was found in the order vector </returns>
		bool IsAttribute(const std::string& entry) const;
		
		/// <summary>
		/// Determines whether or not the entry is in the attributes vector and is prescribed
		/// </summary>
		/// <param name="entry"> The string to search for </param>
		/// <returns> True/false depending on whether or not the typemanager has a signature for this string </returns>
		bool IsPrescribedAttribute(const std::string& entry) const;
		
		/// <summary>
		/// Determines whether or not the entry is in the attributes vector and is prescribed
		/// </summary>
		/// <param name="entry"> The string to search for </param>
		/// <remarks> Wraps IsAttribute and IsPrescribedAttribute </remarks>
		/// <returns> True/false depending on whether or not the entry is an attribute and NOT prescribed</returns>
		bool IsAuxiliaryAttribute(const std::string& entry) const;

		/// <summary>
		/// Simply appends an entry to the scope if the element is not prescribed
		/// </summary>
		/// <param name="entry"> The entry to append to the scope</param>
		/// <exception cref="std::invalid_argument"> The element is already prescribed </exception>
		/// <returns> The datum that comes back from the Append() call in Scope </returns>
		Datum& AppendAuxililaryAttribute(const std::string& entry);

		/// <summary>
		/// Queries the order vector
		/// </summary>
		/// <returns> The order vector in its entirety </returns>
		const Vector<HashMap<const std::string, Datum>::PairType*>& Attributes() const { return order; }
		
		/// <summary>
		/// Queries the attributes and filters out the prescribed attributes
		/// </summary>
		/// <remarks> Constructs a new vector, adding pointers to pairs as opposed to copies of pairs </remarks>
		/// <returns> A vector of attributes that are prescribed </returns>
		const Vector<ScopePairType*> PrescribedAttributes() const;
		
		/// <summary>
		/// Queries the attributes and filters out the auxiliary attributes
		/// </summary>
		/// <remarks> Constructs a new vector, adding pointers to pairs as opposed to copies of pairs </remarks>
		/// <returns> A vector of attributes that are auxiliary </returns>
		const Vector<ScopePairType*> AuxiliaryAttributes() const;

	protected:
		/// <summary>
		/// Creates an attributed and feeds the the child's id to the populate method
		/// </summary>
		/// <param name="child_id"> The children's RTTI ID </param>
		/// <remarks> Can't create an instance unless it's a derived class of attributed </remarks>
		explicit Attributed(const RTTI::IdType id_type);
	
	private:
		void Populate(const RTTI::IdType id_type);
		void PointerRewrite(const RTTI::IdType id_type);
	};
}

