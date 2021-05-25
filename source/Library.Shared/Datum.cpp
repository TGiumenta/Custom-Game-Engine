#include "pch.h"
#include "Datum.h"
#include "RTTI.h"
#include <stdexcept>

namespace FieaGameEngine
{
#pragma region RuleOf6
	Datum::Datum(const DatumTypes new_type) :
		type(new_type)
	{}

	Datum::Datum(const Datum& other) :
		size(other.size),
		type(other.type),
		is_external(other.is_external)
	{
		if (other.is_external)
		{
			data = other.data;
			capacity = other.capacity;
		}
		else
		{
			DeepCopyDatum(other);
		}
	}

	Datum::Datum(Datum&& other) noexcept 
	{
		MoveDatum(other);
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			if ((is_external == false) && (capacity > 0_z))
			{
				Clear();
				capacity = 0_z;
				free(data.vp);
			}

			size = other.size;
			type = other.type;
			is_external = other.is_external;

			// = External
			if (other.is_external)
			{
				data = other.data;
				capacity = other.capacity;
			}
			// = Internal
			else
			{
				capacity = 0_z;
				data.vp = nullptr;
				DeepCopyDatum(other);
			}
		}

		return *this;
	}

	Datum& Datum::operator=(const int& other)
	{
		SetType(Datum::DatumTypes::Integer);
		Resize(1_z);
		Set<int>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(const float& other)
	{
		SetType(Datum::DatumTypes::Float);
		Resize(1_z);
		Set<float>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(const std::string& other)
	{
		SetType(Datum::DatumTypes::String);
		Resize(1_z);
		Set<std::string>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& other)
	{
		SetType(Datum::DatumTypes::Vector);
		Resize(1_z);
		Set<glm::vec4>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& other)
	{
		SetType(Datum::DatumTypes::Matrix);
		Resize(1_z);
		Set<glm::mat4>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(RTTI* other)
	{
		SetType(Datum::DatumTypes::Pointer);
		Resize(1_z);
		Set<RTTI*>(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(Scope& other)
	{
		SetType(Datum::DatumTypes::Table);
		Resize(1_z);
		SetScope(other, 0_z);
		return *this;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveDatum(other);
		}

		return *this;
	}

	Datum::~Datum()
	{
		if (!is_external)
		{
			if (capacity > 0_z)
			{
				Clear();
				free(data.vp);
			}
		}
	}

#pragma endregion RuleOf6

	void Datum::SetType(const DatumTypes& new_type)
	{
		if (type != DatumTypes::Unknown && new_type != type)
		{
			throw std::runtime_error("You can't change this type once set.");
		}

		type = new_type;
	}

#pragma region CreateFunctions

	inline void Datum::CreateIntegers(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.i + i)int(0);
		}
	}

	inline void Datum::CreateFloats(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.f + i)float(0.0f);
		}
	}

	inline void Datum::CreateStrings(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.s + i)std::string();
		}
	}

	inline void Datum::CreateVectors(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.v + i)glm::vec4(0);
		}
	}

	inline void Datum::CreateMatrices(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.m + i)glm::mat4(0);
		}
	}

	inline void Datum::CreatePointers(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.r + i)RTTI*(nullptr);
		}
	}

	inline void Datum::CreateTables(const size_t& start, const size_t& amount)
	{
		for (size_t i = start; i < amount; ++i)
		{
			new (data.sc + i)Scope*(nullptr);
		}
	}

#pragma endregion CreateFunctions

	void Datum::Resize(const size_t& new_size)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot resize on external data.");
		}

		if (type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Can't reserve on an unknown type.");
		}

		if (new_size < size)
		{
			if (type == Datum::DatumTypes::String)
			{
				using namespace std;
				for (size_t i = new_size; i < size; ++i)
				{
					data.s[i].~string();
				}
			}
		}
		else
		{
			Reserve(new_size);

			CreateDefaultFunctions func = CreateFunctions[static_cast<int>(type)];
			assert(func != nullptr);
			(this->*func)(size, new_size);
		}

		size = new_size;
	}

	void Datum::Clear()
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot clear external data.");
		}

		if (type == DatumTypes::String)
		{
			using namespace std;

			for (size_t i = 0_z; i < size; ++i)
			{
				data.s[i].~string();
			}

		}
		size = 0_z;
	}

#pragma region CompareFunctions

	bool Datum::CompareMemory(const Datum& other) const
	{
		const size_t amount_of_space = size_map[static_cast<int>(type)];
		const size_t bytes_to_compare = amount_of_space * other.size;

		return (memcmp(data.b, other.data.b, bytes_to_compare) == 0);
	}

	bool Datum::CompareStrings(const Datum& other) const
	{
		bool is_equal = true;
		for (size_t index = 0; index < other.size; ++index)
		{
			if (data.s[index] != other.data.s[index])
			{
				is_equal = false;
				break;
			}
		}

		return is_equal;
	}

	bool Datum::ComparePointers(const Datum& other) const
	{
		for (size_t index = 0; index < other.size; ++index)
		{
			RTTI* lhsRtti = data.r[index];
			RTTI* rhsRtti = other.data.r[index];

			if ((lhsRtti != rhsRtti) && (lhsRtti != nullptr) && !(lhsRtti->Equals(rhsRtti)))
			{
				return false;
			}
		}

		return true;
	}

#pragma endregion CompareFunctions

	bool Datum::operator==(const Datum& other) const
	{
		bool found = false;

		if (size == other.size && type == other.type)
		{
			if (type == Datum::DatumTypes::Unknown)
			{
				return true;
			}

			CompareDefaultFunctions func = CompareFunctions[static_cast<int>(type)];
			assert(func != nullptr);
			found = (this->*func)(other);
		}

		return found;
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !(operator==(other));
	}

	Scope& Datum::operator[](const size_t& index)
	{
		return Get<Scope>(index);
	}

	const Scope& Datum::operator[](const size_t& index) const
	{
		return Get<Scope>(index);
	}

	void Datum::Reserve(const size_t& new_capacity)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot reserve external data.");
		}

		if (type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Can't reserve on an unknown type.");
		}

		if (new_capacity > capacity)
		{
			const size_t element_size = size_map[static_cast<int>(type)];

			void* new_data = realloc(data.vp, new_capacity * element_size);
			assert(new_data != nullptr);
			data.vp = new_data;

			capacity = new_capacity;
		}
	}

#pragma region FromStringFunctions

	template <>
	inline int32_t Datum::FromString(const std::string& value)
	{
		return stoi(value);
	}

	template <>
	inline float Datum::FromString(const std::string& value)
	{
		return stof(value);
	}

	template <>
	inline std::string Datum::FromString(const std::string& value)
	{
		return value;
	}

	template <>
	inline glm::vec4 Datum::FromString(const std::string& value)
	{
		glm::vec4 vector{};
		sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &vector.x, &vector.y, &vector.z, &vector.w);

		return vector;
	}

	template <>
	inline glm::mat4 Datum::FromString(const std::string& value)
	{
		glm::mat4 matrix{};

		sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
			&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
			&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
			&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);

		return matrix;
	}

#pragma endregion FromStringFunctions

#pragma region SetDeserializeFunctions

	inline void Datum::SetDeserializeInteger(const std::string& value, const size_t& index)
	{
		Set<int>(FromString<int>(value), index);
	}

	inline void Datum::SetDeserializeFloat(const std::string& value, const size_t& index)
	{
		Set<float>(FromString<float>(value), index);
	}

	inline void Datum::SetDeserializeString(const std::string& value, const size_t& index)
	{
		Set<std::string>(FromString<std::string>(value), index);
	}

	inline void Datum::SetDeserializeVector(const std::string& value, const size_t& index)
	{
		Set<glm::vec4>(FromString<glm::vec4>(value), index);
	}

	inline void Datum::SetDeserializeMatrix(const std::string& value, const size_t& index)
	{
		Set<glm::mat4>(FromString<glm::mat4>(value), index);
	}

	inline void Datum::SetDeserializePointer(const std::string& /*value*/, const size_t& /*index*/)
	{
		throw std::runtime_error("Trying to destring a pointer.");
	}

#pragma endregion SetDeserializeFunctions

#pragma region PushBackFromStringFunctions

	inline void Datum::PushBackDeserializeInteger(const std::string& value)
	{
		PushBack(FromString<int>(value));
	}

	inline void Datum::PushBackDeserializeFloat(const std::string& value)
	{
		PushBack(FromString<float>(value));
	}

	inline void Datum::PushBackDeserializeString(const std::string& value)
	{
		PushBack(FromString<std::string>(value));
	}

	inline void Datum::PushBackDeserializeVector(const std::string& value)
	{
		PushBack(FromString<glm::vec4>(value));
	}

	inline void Datum::PushBackDeserializeMatrix(const std::string& value)
	{
		PushBack(FromString<glm::mat4>(value));
	}

	inline void Datum::PushBackDeserializePointer(const std::string& /*value*/)
	{
		throw std::runtime_error("Trying to destring a pointer.");
	}

#pragma endregion PushBackFromStringFunctions

	void Datum::SetFromString(const std::string& value, const size_t& index)
	{
		DestringDefaultFunctions func = SetDeserializeFunctions[static_cast<int>(type)];
		assert(func != nullptr);
		(this->*func)(value, index);
	}

	void Datum::PushBackFromString(const std::string& value)
	{
		PushBackDefaultFunctions func = PushBackDeserializeFunctions[static_cast<int>(type)];
		assert(func != nullptr);
		(this->*func)(value);
	}

#pragma region SerializeFunctions

	std::string Datum::SerializeInteger(const size_t& index) const
	{
		return std::to_string(Get<int>(index));
	}

	std::string Datum::SerializeFloat(const size_t& index) const
	{
		return std::to_string(Get<float>(index));
	}

	std::string Datum::SerializeString(const size_t& index) const
	{
		return Get<std::string>(index);
	}

	std::string Datum::SerializeVector(const size_t& index) const
	{
		return glm::to_string(Get<glm::vec4>(index));
	}

	std::string Datum::SerializeMatrix(const size_t& index) const
	{
		return glm::to_string(Get<glm::mat4>(index));
	}

	std::string Datum::SerializePointer(const size_t& index) const
	{
		using namespace std::string_literals;

		RTTI* rtti = Get<RTTI*>(index);
		if (rtti == nullptr)
		{
			return "nullptr"s;
		}

		return rtti->ToString();
	}

#pragma endregion SerializeFunctions

	std::string Datum::ToString(const size_t& index) const
	{
		if (type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Can't ToString on an unknown type.");
		}

		StringifyDefaultFunctions func = SerializeFunctions[static_cast<int>(type)];
		assert(func != nullptr);
		return (this->*func)(index);
	}

	void Datum::PopBack()
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot remove on external data.");
		}

		if (type == Datum::DatumTypes::String)
		{
			using namespace std;
			data.s[size - 1].~string();
		}

		--size;
	}

	bool Datum::RemoveAt(const size_t& found_index)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot remove on external data.");
		}

		bool found = false;
		if (found_index < size)
		{
			if (type == DatumTypes::String)
			{
				using namespace std;
				data.s[found_index].~string();
			}

			const size_t bytes_to_move = ((size - 1_z) - found_index) * size_map[static_cast<int>(type)];
			if (bytes_to_move > 0)
			{
				std::memmove(&data.b[found_index*bytes_to_move], &data.b[(found_index + 1)*bytes_to_move], bytes_to_move);
			}

			--size;

			found = true;
		}

		return found;
	}

	Datum& Datum::ShallowCopyDatum(const Datum& other)
	{
		data = other.data;
		type = other.type;
		size = other.size;
		capacity = other.capacity;
		is_external = other.is_external;

		return *this;
	}

	Datum& Datum::MoveDatum(Datum& other)
	{
		ShallowCopyDatum(other);

		other.data.vp = nullptr;
		other.type = Datum::DatumTypes::Unknown;
		other.size = 0_z;
		other.capacity = 0_z;
		other.is_external = false;

		return *this;
	}

	Datum& Datum::DeepCopyDatum(const Datum& other)
	{
		if (other.capacity > 0_z)
		{
			Reserve(other.size);

			if (type == Datum::DatumTypes::String)
			{
				for (size_t index = 0_z; index < other.size; ++index)
				{
					new(data.s + index)std::string(other.data.s[index]);
				}
			}
			else
			{
				const size_t bytes_to_copy = other.size * size_map[static_cast<int>(type)];
				if (bytes_to_copy > 0_z)
				{
					std::memcpy(&data.b[0_z], &other.data.b[0_z], bytes_to_copy);
				}
			}
		}

		return *this;
	}
}