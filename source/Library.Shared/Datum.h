#pragma once
#include <glm/glm.hpp>
#include <functional>
#include "SizeLiteral.h"
#include "DefaultIncrement.h"
#include <map>

namespace FieaGameEngine
{
	class RTTI;
	class Scope;

	/// <summary>
	/// A class which defines a non-templated, final Datum. 
	/// Stores a primitive/complex type, a reference to the array that's used, the size of the Datum, capacity, if the data that's stored is external
	/// Type refers to the primitive or complex type
	/// Size refers to the amount of elements in the vector
	/// Capacity refers to the amount of space that's available in the vector
	/// IsExternal refers to if the information in this Datum is defined elsewhere (ie RTTI derived classes)
	/// </summary>
	class Datum final
	{
	public:

		friend Scope;

		/// <summary>
		/// An enum which defines each possible type that could be stored within datum
		/// </summary>
		enum class DatumTypes
		{
			Integer,
			Float,
			String,
			Vector,
			Matrix,
			Pointer,
			Table,
			Unknown,
			End = Unknown
		};

#pragma region RuleOf6

		/// <summary>
		/// Constructor for object initialization
		/// <param name = "new_type"> The type of information this datum will store </param>
		/// </summary>
		explicit Datum(const DatumTypes new_type = DatumTypes::Unknown);

		/// <summary>
		/// Copy constructor which creates a new datum based on the existing other datum
		/// </summary>
		/// <param name="other"> The original datum to copy </param>
		/// <remarks> If the the datum is external, perform a shallow copy. Otherwise, perform a deep copy </remarks>
		Datum(const Datum& other);
		
		/// <summary>
		/// Copy constructor which creates a new datum based on the existing other datum
		/// </summary>
		/// <param name="other"> The original datum to copy </param>
		/// <remarks> Copy constructor uses r-values instead of l-values </remarks>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Sets this datum equal to the other datum
		/// </summary>
		/// <param name = "other"> The datum to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const Datum& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar int
		/// </summary>
		/// <param name = "other"> The scalar int to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const int& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar float
		/// </summary>
		/// <param name = "other"> The scalar float to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <remarks> Performs only a shallow copy if the data is marked external, and a deep copy otherwise </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const float& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar string
		/// </summary>
		/// <param name = "other"> The scalar string to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const std::string& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar vector
		/// </summary>
		/// <param name = "other"> The scalar vector to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const glm::vec4& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar matrix
		/// </summary>
		/// <param name = "other"> The scalar matrix to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(const glm::mat4& other);
		
		/// <summary>
		/// Sets this datum equal to the scalar RTTI
		/// </summary>
		/// <param name = "other"> The scalar RTTI to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(RTTI* other);

		/// <summary>
		/// Sets this datum equal to the scalar Scope
		/// </summary>
		/// <param name = "other"> The scalar Scope to equate this datum to </param>
		/// <remarks> Clears and shrinks "this" datum before copying </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(Scope& other);

		/// <summary>
		/// Sets this datum equal to the other datum
		/// </summary>
		/// <param name = "other"> The scalar int to equate this datum to </param>
		/// <remarks> Equality operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs datum (this) after equalizing them </returns>
		Datum& operator=(Datum&& other) noexcept;

		/// <summary>
		/// Destroys internal datums, calls clear
		/// </summary>
		~Datum();

#pragma endregion RuleOf6

		/// <summary>
		/// Queries the datum for its type
		/// </summary>
		/// <returns> The type associated with this datum </returns>
		const DatumTypes Type() const { return type; }

		/// <summary>
		/// Sets the type of the datum if it has not been set already set
		/// </summary>
		/// <param name="new_type"> The type to set this datum to </param>
		void SetType(const DatumTypes& new_type);

		/// <summary>
		/// Queries the datum and determines the amount of objects in the datum
		/// </summary>
		/// <returns> Amount of elements in the datum </returns>
		size_t Size() const { return size; }

		/// <summary>
		/// Queries the datum and determines the amount of space available in the datum
		/// </summary>
		/// <returns> Amount of space available in the datum </returns>
		size_t Capacity() const { return capacity; }

		/// <summary>
		/// Reduces the size of the datum to the parameter's size
		/// </summary>
		/// <param name="new_size">The amount of space to set size on this datum </param>
		/// <remarks> Guarantees capacity is greater than or equal to the size </remarks>
		void Resize(const size_t& new_size);

		/// <summary>
		/// Removes all elements from the datum
		/// </summary>
		/// <remarks> Manually calls destructor on strings, otherwise simply sets size to 0 </remarks>
		void Clear();

		/// <summary>
		/// A templated method which sets a datum to be externally stored
		/// </summary>
		/// <param name="new_data"> An array of data that will become the data stored at this datum </param>
		/// <param name="new_size"> The size/capacity to set this datum to </param>
		/// <remarks> This method simply type checks and then sets the data to the passed in array </remarks>
		template <typename T>
		void SetStorage(T* new_data, const size_t& new_size);

		/// <summary>
		/// Compares two datums to one another. Refers to a method defined in a lookup table for different types
		/// </summary>
		/// <param name="other"> The datum to compare "this" datum to </param>
		/// <returns> Whether or not the datums are equivalent </returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// Compares two datums to one another. Refers to a method defined in a lookup table for different types
		/// </summary>
		/// <param name="other"> The datum to compare "this" datum to </param>
		/// <returns> Whether or not the datums are NOT equivalent </returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// A templated overload of the equlality which compares this datum to a scalar templated value
		/// </summary>
		/// <param name="value"> The scalar value to compare "this" datum to, (ie 10, 24.4f, "Hello", etc) </param>
		/// <returns> Whether or not the datum is equivalent to the scalar </returns>
		template <typename T>
		bool operator==(const T& value) const;

		/// <summary>
		/// A templated overload of the equlality which compares this datum to a scalar templated value
		/// </summary>
		/// <param name="value"> The scalar value to compare "this" datum to, (ie 10, 24.4f, "Hello", etc) </param>
		/// <returns> Whether or not the datum is NOT equivalent to the scalar </returns>
		template <typename T>
		bool operator!=(const T& value) const;

		/// <summary>
		/// Allows simpler access to nested scopes
		/// </summary>
		/// <param name="index"> The index of the datum at which to get data </param>
		/// <returns> The data at the given index </returns>
		Scope& operator[](const size_t& index);

		/// <summary>
		/// Allows simpler access to nested scopes
		/// </summary>
		/// <param name="index"> The index of the datum at which to get data </param>
		/// <returns> The data at the given index </returns>
		const Scope& operator[](const size_t& index) const;

		/// <summary>
		/// Reallocates memory for the new array with the provided capacity as space
		/// </summary>
		/// <param name="new_capacity">The amount of capacity to reserve on this datum </param>
		/// <remarks> Increases capacity, NEVER decreases it </remarks>
		/// <remarks> Does not impact size in any way </remarks>
		void Reserve(const size_t& new_capacity);

		/// <summary>
		/// Sets a value of the datum at the paramaterized index
		/// </summary>
		/// <param name="value"> The data to set </param>
		/// <param name="index"> The index of the datum at which to set the data </param>
		template<typename T>
		void Set(const T& value, const size_t& index = 0);

		/// <summary>
		/// Gets a value of the datum at the paramaterized index
		/// </summary>
		/// <param name="index"> The index of the datum at which to get data </param>
		/// <returns> The data at the given index </returns>
		template<typename T>
		T& Get(const size_t& index = 0);

		/// <summary>
		/// Gets a value of the datum at the paramaterized index
		/// </summary>
		/// <param name="index"> The index of the datum at which to get data </param>
		/// <remarks> Specifically refers to the Get on const datums </remarks>
		/// <returns> The data at the given index </returns>
		template<typename T>
		const T& Get(const size_t& index = 0) const;

		/// <summary>
		/// Converts a value in string format to that actual value
		/// </summary>
		/// <returns> The actual value based on the parameter </returns>
		template <typename T>
		T FromString(const std::string& value);

		/// <summary>
		/// Calls set on a string, which requires parsing
		/// </summary>
		/// <param name="value"> The value which needs to be parsed. When parsed, sends a value to set </param>
		/// <param name="index"> The index of the datum at which to set the data </param>
		void SetFromString(const std::string& value, const size_t& index = 0);

		/// <summary>
		/// Calls the FromString template and pushes back into the Datum thereafter
		/// </summary>
		/// <param name="value"> The string to deserialize and push into the Datum </param>
		void PushBackFromString(const std::string& value);

		/// <summary>
		/// Converts a data value to a string at the given index
		/// </summary>
		/// <param name="index"> The index of the datum at which to get the data to convert to a string </param>
		/// <returns> A string converted from the data found at the index </returns>
		std::string ToString(const size_t& index = 0) const;

		/// <summary>
		/// Adds a new element to the end of the datum
		/// </summary>
		/// <param name="value">The data object that will associated with the index </param>
		template <typename T, typename IncrementFunctor = DefaultIncrement>
		void PushBack(const T& value);

		/// <summary>
		/// Adds a new element to the end of the datum
		/// </summary>
		/// <param name="value">The data object that will associated with the index </param>
		/// <remarks> Specifically handles the r-value version of strings </remarks>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(std::string&& value);

		/// <summary>
		/// Removes the last data member of the datum
		/// </summary>
		void PopBack();

		/// <summary>
		/// Queries the datum and retrieves the first data member in the datum
		/// </summary>
		/// <returns> A mutable reference to the first data member in the datum </returns>
		template<typename T>
		T& Front();

		/// <summary>
		/// Queries the datum and retrieves the first data member in the datum
		/// </summary>
		/// <remarks> This is the const version of the other Front method </remarks>
		/// <returns> A non-mutable reference to the first data member in the datum </returns>
		template<typename T>
		const T& Front() const;

		/// <summary>
		/// Queries the datum and retrieves the last data member in the datum
		/// </summary>
		/// <returns> A mutable reference to the last data member in the datum </returns>
		template<typename T>
		T& Back();

		/// <summary>
		/// Queries the datum and retrieves the last data member in the datum
		/// </summary>
		/// <remarks> This is the const version of the other Back method </remarks>
		/// <returns> A non-mutable reference to the last data member in the datum </returns>
		template<typename T>
		const T& Back() const;

		/// <summary>
		/// A templated version of remove which queries IndexOf, and RemoveAt that index
		/// </summary>
		/// <returns> A booelan which determines whether the element was removed </returns>
		template<typename T>
		bool Remove(const T& value);

		/// <summary>
		/// Removes whatever data is stored at the given index
		/// </summary>
		/// <param name="found_index"> The index to reference in the datum to remove </param>
		/// <returns> A booelan which determines whether the element was removed </returns>
		bool RemoveAt(const size_t& found_index);

		/// <summary>
		/// Finds the index of the value passed in. Returns size if the data was not found
		/// </summary>
		/// <param name="value"> The data to search for </param>
		/// <returns> The index of the data, size if not found </returns>
		template<typename T>
		inline const size_t IndexOf(const T& value) const;

		/// <summary>
		/// Queries the current Datum and determines if its external or not
		/// </summary>
		/// <returns> An indicator determining if the Datum is external or not </returns>
		bool IsExternal();

		/// <summary>
		/// A mapping of names which converts the DatumType into a string for easy lookup
		/// </summary>
		inline static const std::map<DatumTypes, std::string> type_to_name_map =
		{
			{DatumTypes::Integer, "Integer"},
			{DatumTypes::Float, "Float"},
			{DatumTypes::String, "String"},
			{DatumTypes::Vector, "Vector4"},
			{DatumTypes::Matrix, "Matrix4"},
			{DatumTypes::Pointer, "Pointer"},
			{DatumTypes::Table, "Table"},
			{DatumTypes::Unknown, "Unknown"}
		};

		inline static const std::map<std::string, DatumTypes> name_to_type_map =
		{
			{"Integer", DatumTypes::Integer},
			{"Float", DatumTypes::Float},
			{"String", DatumTypes::String},
			{"Vector4", DatumTypes::Vector},
			{"Matrix4", DatumTypes::Matrix},
			{"Pointer", DatumTypes::Pointer},
			{"Table", DatumTypes::Table},
			{"Unknown", DatumTypes::Unknown}
		};

	private:
		union DatumValue
		{
			int* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			RTTI** r;
			Scope** sc;

			char* b;				// Used for byte moving
			void* vp = nullptr;		// Shared Data
		};

		template <typename IncrementFunctor = DefaultIncrement>
		void PotentiallyReserve(const Datum::DatumTypes& other_type);

		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(Scope& value);

		void SetScope(Scope& value, const size_t& index);

		void GetInit(const Datum::DatumTypes& other_type, const size_t& index) const;
		void SetInit(const Datum::DatumTypes& other_type, const size_t& index) const;

		Datum& ShallowCopyDatum(const Datum& other);
		Datum& MoveDatum(Datum& other);
		Datum& DeepCopyDatum(const Datum& other);

		inline static const size_t size_map[static_cast<int>(DatumTypes::End) + 1] =
		{
			sizeof(int),
			sizeof(float),
			sizeof(std::string),
			sizeof(glm::vec4),
			sizeof(glm::mat4),
			sizeof(RTTI*),
			sizeof(Scope*),
			0						// Unknown
		};

		DatumTypes type{ DatumTypes::Unknown }; // Discriminator. This is what I will use to set my type
		DatumValue data{ nullptr };
		size_t size = 0_z;
		size_t capacity = 0_z;
		bool is_external = false;

		friend class Attributed;
		/// <summary>
		/// A templated method which sets a datum to be externally stored
		/// </summary>
		/// <param name="new_data"> An array of data that will become the data stored at this datum </param>
		/// <param name="other_type"> The type of the data to ensure type checking </param>
		/// <param name="new_size"> The size/capacity to set this datum to </param>
		/// <remarks> Explicitly sets new_data equal to the .vp scope of Data, the type no longer matters </remarks>
		void SetStorage(void* new_data, const Datum::DatumTypes& other_type, const size_t& new_size);

// Function Pointer Groupings

#pragma region CreateFunctions

		inline void CreateIntegers(const size_t& startIndex, const size_t& amount);
		inline void CreateFloats(const size_t& startIndex, const size_t& amount);
		inline void CreateStrings(const size_t& startIndex, const size_t& amount);
		inline void CreateVectors(const size_t& startIndex, const size_t& amount);
		inline void CreateMatrices(const size_t& startIndex, const size_t& amount);
		inline void CreatePointers(const size_t& startIndex, const size_t& amount);
		inline void CreateTables(const size_t& startIndex, const size_t& amount);

		using CreateDefaultFunctions = void(Datum::*)(const size_t&, const size_t&);
		inline static const Datum::CreateDefaultFunctions CreateFunctions[static_cast<int>(DatumTypes::End) + 1] =
		{
			&Datum::CreateIntegers,
			&Datum::CreateFloats,
			&Datum::CreateStrings,
			&Datum::CreateVectors,
			&Datum::CreateMatrices,
			&Datum::CreatePointers,
			&Datum::CreateTables
		};

#pragma endregion CreateFunctions

#pragma region CompareFunctions

		inline bool CompareMemory(const Datum& other) const;
		inline bool CompareStrings(const Datum& other) const;
		inline bool ComparePointers(const Datum& other) const;

		using CompareDefaultFunctions = bool(Datum::*)(const Datum&) const;
		inline static const Datum::CompareDefaultFunctions CompareFunctions[static_cast<int>(DatumTypes::End) + 1] =
		{
			&Datum::CompareMemory,
			&Datum::CompareMemory,
			&Datum::CompareStrings,
			&Datum::CompareMemory,
			&Datum::CompareMemory,
			&Datum::ComparePointers,
			&Datum::ComparePointers
		};

#pragma endregion CompareFunctions

#pragma region SerializeFunctions 

		inline std::string SerializeInteger(const size_t& index) const;
		inline std::string SerializeFloat(const size_t& index) const ;
		inline std::string SerializeString(const size_t& index) const;
		inline std::string SerializeVector(const size_t& index) const;
		inline std::string SerializeMatrix(const size_t& index) const;
		inline std::string SerializePointer(const size_t& index) const;

		using StringifyDefaultFunctions = std::string(Datum::*)(const size_t&) const;
		inline static const Datum::StringifyDefaultFunctions SerializeFunctions[static_cast<int>(DatumTypes::End) + 1] =
		{
			&Datum::SerializeInteger,
			&Datum::SerializeFloat,
			&Datum::SerializeString,
			&Datum::SerializeVector,
			&Datum::SerializeMatrix,
			&Datum::SerializePointer,
			&Datum::SerializePointer
		};

#pragma endregion SerializeFunctions

#pragma region SetDeserializeFunctions

		inline void SetDeserializeInteger(const std::string& value, const size_t& index);
		inline void SetDeserializeFloat(const std::string& value, const size_t& index);
		inline void SetDeserializeString(const std::string& value, const size_t& index);
		inline void SetDeserializeVector(const std::string& value, const size_t& index);
		inline void SetDeserializeMatrix(const std::string& value, const size_t& index);
		inline void SetDeserializePointer(const std::string& value, const size_t& index);

		using DestringDefaultFunctions = void(Datum::*)(const std::string&, const size_t&);
		inline static const Datum::DestringDefaultFunctions SetDeserializeFunctions[static_cast<int>(DatumTypes::End) + 1] =
		{
			&Datum::SetDeserializeInteger,
			&Datum::SetDeserializeFloat,
			&Datum::SetDeserializeString,
			&Datum::SetDeserializeVector,
			&Datum::SetDeserializeMatrix,
			&Datum::SetDeserializePointer,
			&Datum::SetDeserializePointer
		};

#pragma endregion SetDeserializeFunctions

#pragma region PushBackDeserializeFunctions

		inline void PushBackDeserializeInteger(const std::string& value);
		inline void PushBackDeserializeFloat(const std::string& value);
		inline void PushBackDeserializeString(const std::string& value);
		inline void PushBackDeserializeVector(const std::string& value);
		inline void PushBackDeserializeMatrix(const std::string& value);
		inline void PushBackDeserializePointer(const std::string& value);

		using PushBackDefaultFunctions = void(Datum::*)(const std::string&);
		inline static const Datum::PushBackDefaultFunctions PushBackDeserializeFunctions[static_cast<int>(DatumTypes::End) + 1] =
		{
			&Datum::PushBackDeserializeInteger,
			&Datum::PushBackDeserializeFloat,
			&Datum::PushBackDeserializeString,
			&Datum::PushBackDeserializeVector,
			&Datum::PushBackDeserializeMatrix,
			&Datum::PushBackDeserializePointer,
			&Datum::PushBackDeserializePointer
		};

#pragma endregion PushBackDeserializeFunctions
	
	}; // END OF DATUM CLASS

// Template Method Specializations

#pragma region EqualityScalar

	template<> bool Datum::operator==(const int& value) const;
	template<> bool Datum::operator!=(const int& value) const;
	template<> bool Datum::operator==(const float& value) const;
	template<> bool Datum::operator!=(const float& value) const;
	template<> bool Datum::operator==(const std::string& value) const;
	template<> bool Datum::operator!=(const std::string& value) const;
	template<> bool Datum::operator==(const glm::vec4& value) const;
	template<> bool Datum::operator!=(const glm::vec4& value) const;
	template<> bool Datum::operator==(const glm::mat4& value) const;
	template<> bool Datum::operator!=(const glm::mat4& value) const;
	template<> bool Datum::operator==(RTTI* const& value) const;
	template<> bool Datum::operator!=(RTTI* const& value) const;
	template<> bool Datum::operator==(const Scope& value) const;
	template<> bool Datum::operator!=(const Scope& value) const;

#pragma endregion EqualityScalar

#pragma region Set

	template<> void Datum::Set(const int& value, const size_t& index);
	template<> void Datum::Set(const float& value, const size_t& index);
	template<> void Datum::Set(const std::string& value, const size_t& index);
	template<> void Datum::Set(const glm::vec4& value, const size_t& index);
	template<> void Datum::Set(const glm::mat4& value, const size_t& index);
	template<> void Datum::Set(RTTI* const& value, const size_t& index);

#pragma endregion Set

#pragma region Get

	template<> int& Datum::Get(const size_t& index);
	template<> const int& Datum::Get(const size_t& index) const;
	template<> float& Datum::Get(const size_t& index);
	template<> const float& Datum::Get(const size_t& index) const;
	template<> std::string& Datum::Get(const size_t& index);
	template<> const std::string& Datum::Get(const size_t& index) const;
	template<> glm::vec4& Datum::Get(const size_t& index);
	template<> const glm::vec4& Datum::Get(const size_t& index) const;
	template<> glm::mat4& Datum::Get(const size_t& index);
	template<> const glm::mat4& Datum::Get(const size_t& index) const;
	template<> RTTI*& Datum::Get(const size_t& index);
	template<> RTTI* const& Datum::Get(const size_t& index) const;

	template<> Scope& Datum::Get(const size_t& index);
	template<> const Scope& Datum::Get(const size_t& index) const;

#pragma endregion Get

#pragma region Front

	template<> int& Datum::Front();
	template<> const int& Datum::Front() const;
	template<> float& Datum::Front();
	template<> const float& Datum::Front() const;
	template<> std::string& Datum::Front();
	template<> const std::string& Datum::Front() const;
	template<> glm::vec4& Datum::Front();
	template<> const glm::vec4& Datum::Front() const;
	template<> glm::mat4& Datum::Front();
	template<> const glm::mat4& Datum::Front() const;
	template<> RTTI*& Datum::Front();
	template<> RTTI* const& Datum::Front() const;
	template<> Scope& Datum::Front();
	template<> const Scope& Datum::Front() const;

#pragma endregion Front

#pragma region PushBack

	template<> void Datum::PushBack(const int& value);
	template<> void Datum::PushBack(const float& value);
	template<> void Datum::PushBack(const std::string& value);
	template<> void Datum::PushBack(const glm::vec4& value);
	template<> void Datum::PushBack(const glm::mat4& value);
	template<> void Datum::PushBack(RTTI* const& value);

#pragma endregion PushBack

#pragma region Back

	template<> int& Datum::Back();
	template<> const int& Datum::Back() const;
	template<> float& Datum::Back();
	template<> const float& Datum::Back() const;
	template<> std::string& Datum::Back();
	template<> const std::string& Datum::Back() const;
	template<> glm::vec4& Datum::Back();
	template<> const glm::vec4& Datum::Back() const;
	template<> glm::mat4& Datum::Back();
	template<> const glm::mat4& Datum::Back() const;
	template<> RTTI*& Datum::Back();
	template<> const RTTI* const& Datum::Back() const;
	template<> Scope& Datum::Back();
	template<> const Scope& Datum::Back() const;

#pragma endregion Back

#pragma region SetStorage

	template<> void Datum::SetStorage(int* new_data, const size_t& new_size);
	template<> void Datum::SetStorage(float* new_data, const size_t& new_size);
	template<> void Datum::SetStorage(std::string* new_data, const size_t& new_size);
	template<> void Datum::SetStorage(glm::vec4* new_data, const size_t& new_size);
	template<> void Datum::SetStorage(glm::mat4* new_data, const size_t& new_size);
	template<> void Datum::SetStorage(RTTI** new_data, const size_t& new_size);

#pragma endregion SetStorage

#pragma region Remove

	template<> bool Datum::Remove(const int& value);
	template<> bool Datum::Remove(const float& value);
	template<> bool Datum::Remove(const std::string& value);
	template<> bool Datum::Remove(const glm::vec4& value);
	template<> bool Datum::Remove(const glm::mat4& value);
	template<> bool Datum::Remove(RTTI* const& value);

#pragma endregion Remove

#pragma region IndexOf

	template<> const size_t Datum::IndexOf(const int& value) const;
	template<> const size_t Datum::IndexOf(const float& value) const;
	template<> const size_t Datum::IndexOf(const std::string& value) const;
	template<> const size_t Datum::IndexOf(const glm::vec4& value) const;
	template<> const size_t Datum::IndexOf(const glm::mat4& value) const;
	template<> const size_t Datum::IndexOf(RTTI* const& value) const;

#pragma endregion IndexOf
}

#include "Datum.inl"