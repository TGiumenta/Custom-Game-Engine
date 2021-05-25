#pragma once

#include "RTTI.h"
#include "HashMap.h"
#include "Datum.h"

namespace FieaGameEngine
{
	class Signature final
	{
	public:
		/// <summary>
		/// The default constructor for object initialization
		/// </summary>
		Signature() = default;

		/// <summary>
		/// Constructor overload to handle a prescribed attribute
		/// </summary>
		/// <param name="new_name"> Name of the prescribed attribute </param>
		/// <param name="new_type"> Type of the prescribed attribute </param>
		/// <param name="new_size"> Size of the prescribed attribute </param>
		/// <param name="new_offset"> Offset of the prescribed attribute </param>
		Signature(std::string new_name, Datum::DatumTypes new_type, size_t new_size, size_t new_offset);
		
		/// <summary>
		/// Copy constructor which creates a new signature based on the existing signature
		/// </summary>
		/// <param name="other"> The signature to copy </param>
		Signature(const Signature& other) = default;

		/// <summary>
		/// Copy constructor which creates a new signature based on the existing signature
		/// </summary>
		/// <param name="other"> The signature to copy </param>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		Signature(Signature&& other) noexcept = default;
		
		/// <summary>
		/// Equality operator for signatures
		/// </summary>
		/// <param name = "other"> The signature to equate this signature to </param>
		/// <returns> The lhs signature object (this) after equalizing them </returns>
		Signature& operator=(const Signature& other) = default;
		
		/// <summary>
		/// Equality operator for signatures
		/// </summary>
		/// <param name = "other"> The signature to equate this signature to </param>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs signature object (this) after equalizing them </returns>
		Signature& operator=(Signature&& other) noexcept = default;
		
		/// <summary>
		/// Defaulted destructor
		/// </summary>
		~Signature() = default;

		std::string name;
		Datum::DatumTypes type;
		size_t size;
		size_t storage_offset;
	};

	class TypeManager final
	{
	public:
		struct TypeInfo
		{
			const Vector<Signature> signatures;
			RTTI::IdType parent_id_type;
		};

		/// <summary>
		/// The default constructor for object initialization
		/// <remarks> Deleted so that users cannot instantiate another typemanager </remarks> 
		/// </summary>
		TypeManager() = delete;
		
		/// <summary>
		/// Deleted copy constructor which creates a new typemanager based on the existing typemanager
		/// </summary>
		/// <param name="other"> The typemanager to copy </param>
		/// <remarks> Deleted so that users cannot copy another typemanager </remarks>
		TypeManager(const TypeManager& other) = delete;

		/// <summary>
		/// Deleted copy constructor which creates a new typemanager based on the existing typemanager
		/// </summary>
		/// <param name="other"> The typemanager to copy </param>
		/// <remarks> Deleted so that users cannot copy another typemanager </remarks>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		TypeManager(TypeManager&& other) noexcept = delete;
		
		/// <summary>
		/// Deleted equality operator for typemanagers
		/// </summary>
		/// <param name = "other"> The typemanager to equate this typemanager to </param>
		/// <remarks> Deleted so that users cannot copy into another typemanager </remarks>
		/// <returns> The lhs typemanager object (this) after equalizing them </returns>
		TypeManager& operator=(const TypeManager& other) = delete;
		
		/// <summary>
		/// Deleted equality operator for typemanagers
		/// </summary>
		/// <param name = "other"> The typemanager to equate this typemanager to </param>
		/// <remarks> Deleted so that users cannot copy into another typemanager </remarks>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs typemanager object (this) after equalizing them </returns>
		TypeManager& operator=(TypeManager&& other) noexcept = delete;
		
		/// <summary>
		/// Defaulted destructor, not responsible for deleting the map
		/// </summary>
		~TypeManager() = default;

		/// <summary>
		/// Queries the map's values at the given key
		/// </summary>
		/// <param name="type"> The key to query the map with </param>
		/// <returns> The map's value at the key </returns>
		static const Vector<Signature> GetSignaturesForType(const RTTI::IdType type);
		
		/// <summary>
		/// Retrieves the type's signature based on the given entry key
		/// </summary>
		/// <param name="type"> The key to query the map with </param>
		/// <param name="entry"> Used for comparing with the name associated with the signature </param>
		/// <returns> A pointer to the signature found (if any) </returns>
		static Signature* GetSignature(const RTTI::IdType type, const std::string& entry);
		
		/// <summary>
		/// Queries the map's containskey
		/// </summary>
		/// <param name="type"> The key to search for in the map</param>
		/// <returns> True/false depending on if the key was found in the hashmap </returns>
		static bool ContainsType(const RTTI::IdType type) { return map.ContainsKey(type); }
		
		/// <summary>
		/// Quereies the map's add
		/// <exception cref="std::runtime_error"> The type was already registered </exception> 
		/// <param name="type"> The key to add to the map </param>
		/// <param name="vector"> The value to add to the map </param>
		/// </summary>
		static void AddType(const RTTI::IdType type, const Vector<Signature>& signatures, RTTI::IdType parent_id =std::numeric_limits<size_t>::max());
		
		/// <summary>
		/// Queries the map's remove 
		/// <param name="type"> The key to remove from the map </param>
		/// </summary>
		static void RemoveType(const RTTI::IdType type) { map.Remove(type); }

		/// <summary>
		/// Queries the map's size
		/// </summary>
		static size_t Size() { return map.Size(); }

		/// <summary>
		/// Clears the map
		/// </summary>
		static void Clear() { map.Clear(); }

	private:
		//static HashMap<RTTI::IdType, const Vector<Signature>> map;
		static HashMap<RTTI::IdType, TypeInfo> map;
	};
}