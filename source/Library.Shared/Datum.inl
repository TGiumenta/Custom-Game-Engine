#include "Datum.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
	template<typename T>
	inline bool Datum::operator==(const T& /*value*/) const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline bool Datum::operator!=(const T& /*value*/) const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline void Datum::Set(const T& /*value*/, const size_t& /*index*/)
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline T& Datum::Get(const size_t& /*index*/)
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline const T& Datum::Get(const size_t& /*index*/) const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline void Datum::SetStorage(T* /*new_data*/, const size_t& /*new_size*/)
	{
		static_assert(false, "Data type not supported");
	}

	template <typename T, typename IncrementFunctor>
	inline void Datum::PushBack(const T& /*value*/)
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline T& Datum::Front()
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline const T& Datum::Front() const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline T& Datum::Back()
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline const T& Datum::Back() const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline const size_t Datum::IndexOf(const T& /*value*/) const
	{
		static_assert(false, "Data type not supported");
	}

	template<typename T>
	inline bool Datum::Remove(const T& /*value*/)
	{
		static_assert(false, "Data type not supported");
	}

#pragma region EqualityScalar

	template<>
	inline bool Datum::operator==(const int& value) const
	{
		return ((size >= 1) && (type == DatumTypes::Integer) && (Front<int>() == value));
	}

	template<>
	inline bool Datum::operator!=(const int& value) const
	{
		return !(operator==(value));
	}

	template<>
	inline bool Datum::operator==(const float& value) const
	{
		return ((size >= 1) && (type == DatumTypes::Float) && (Front<float>() == value));
	}

	template<>
	inline bool Datum::operator!=(const float& value) const
	{
		return !(operator==(value));
	}

	template<>
	inline bool Datum::operator==(const std::string& value) const
	{
		return ((size >= 1) && (type == DatumTypes::String) && (Front<std::string>() == value));
	}

	template<>
	inline bool Datum::operator!=(const std::string& value) const
	{
		return !(operator==(value));
	}

	template<>
	inline bool Datum::operator==(const glm::vec4& value) const
	{
		return ((size >= 1) && (type == DatumTypes::Vector) && (Front<glm::vec4>() == value));
	}

	template<>
	inline bool Datum::operator!=(const glm::vec4& value) const
	{
		return !(operator==(value));
	}

	template<>
	inline bool Datum::operator==(const glm::mat4& value) const
	{
		return ((size >= 1) && (type == DatumTypes::Matrix) && (Front<glm::mat4>() == value));
	}

	template<>
	inline bool Datum::operator!=(const glm::mat4& value) const
	{
		return !(operator==(value));
	}

	template<>
	inline bool Datum::operator==(RTTI* const& value) const
	{
		return ((size >= 1) && (type == DatumTypes::Pointer) && (Front<RTTI*>() == value));
	}

	template<>
	inline bool Datum::operator!=(RTTI* const& value) const
	{
		return !(operator==(value));
	}

#pragma endregion EqualityScalar

#pragma region Set

	template<>
	inline void Datum::Set(const int& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Integer, index);
		data.i[index] = value;
	}

	template<>
	inline void Datum::Set(const float& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Float, index);
		data.f[index] = value;
	}

	template<>
	inline void Datum::Set(const std::string& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::String, index);
		data.s[index] = value;
	}

	template<>
	inline void Datum::Set(const glm::vec4& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Vector, index);
		data.v[index] = value;
	}

	template<>
	inline void Datum::Set(const glm::mat4& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Matrix, index);
		data.m[index] = value;
	}

	template<>
	inline void Datum::Set(RTTI* const& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Pointer, index);
		data.r[index] = value;
	}

	inline void Datum::SetScope(Scope& value, const size_t& index)
	{
		SetInit(Datum::DatumTypes::Table, index);
		data.sc[index] = &value;
	}

	inline void Datum::SetInit(const Datum::DatumTypes& other_type, const size_t& index) const
	{
		if (other_type != type)
		{
			throw std::runtime_error("The Datum type is incorrect.");
		}

		if (index >= size)
		{
			throw std::runtime_error("Index out of range.");
		}
	}

#pragma endregion Set

#pragma region Get

	template<>
	inline int& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Integer, index);
		return data.i[index];
	}

	template<>
	inline const int& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Integer, index);
		return data.i[index];
	}

	template<>
	inline float& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Float, index);
		return data.f[index];
	}

	template<>
	inline const float& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Float, index);
		return data.f[index];
	}

	template<>
	inline std::string& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::String, index);
		return data.s[index];
	}

	template<>
	inline const std::string& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::String, index);
		return data.s[index];
	}

	template<>
	inline glm::vec4& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Vector, index);
		return data.v[index];
	}

	template<>
	inline const glm::vec4& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Vector, index);
		return data.v[index];
	}

	template<>
	inline glm::mat4& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Matrix, index);
		return data.m[index];
	}

	template<>
	inline const glm::mat4& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Matrix, index);
		return data.m[index];
	}

	template<>
	inline RTTI*& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Pointer, index);
		return data.r[index];
	}

	template<>
	inline RTTI* const& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Pointer, index);
		return data.r[index];
	}

	template<>
	inline Scope& Datum::Get(const size_t& index)
	{
		GetInit(Datum::DatumTypes::Table, index);
		Scope* scope = data.sc[index];
		assert(scope != nullptr);
		return *scope;
	}

	template<>
	inline const Scope& Datum::Get(const size_t& index) const
	{
		GetInit(Datum::DatumTypes::Table, index);
		Scope* scope = data.sc[index];
		assert(scope != nullptr);
		return *scope;
	}

	inline void Datum::GetInit(const Datum::DatumTypes& other_type, const size_t& index) const
	{
		if (other_type != type)
		{
			throw std::runtime_error("The Datum type is incorrect.");
		}

		if (index >= size)
		{
			throw std::runtime_error("Index out of range.");
		}
	}

#pragma endregion Get

#pragma region PushBack

	template<>
	inline void Datum::PushBack(const int& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Integer);
		new(data.i + size++)int(value);
	}

	template<>
	inline void Datum::PushBack(const float& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Float);
		new(data.f + size++)float(value);
	}

	template<>
	inline void Datum::PushBack(const std::string& value)
	{
		PotentiallyReserve(Datum::DatumTypes::String);
		new(data.s + size++)std::string(value);
	}

	template<>
	inline void Datum::PushBack(const glm::vec4& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Vector);
		new(data.v + size++)glm::vec4(value);
	}

	template<>
	inline void Datum::PushBack(const glm::mat4& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Matrix);
		new(data.m + size++)glm::mat4(value);
	}

	template<>
	inline void Datum::PushBack(RTTI* const& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Pointer);
		new(data.r + size++)RTTI* (value);
	}

	template <typename IncrementFunctor>
	inline void Datum::PushBack(std::string&& value)
	{
		PotentiallyReserve(Datum::DatumTypes::String);
		new(data.s + size++)std::string(std::forward<std::string>(value));
	}

	template <typename IncrementFunctor>
	inline void Datum::PushBack(Scope& value)
	{
		PotentiallyReserve(Datum::DatumTypes::Table);
		new(data.sc + size++)Scope*(&value);
	}

	template <typename IncrementFunctor>
	inline void Datum::PotentiallyReserve(const Datum::DatumTypes& other_type)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot reserve on external data.");
		}

		if (type != other_type)
		{
			throw std::runtime_error("The Datum type is incorrect.");
		}

		IncrementFunctor incrementor{};

		if (size == capacity)
		{
			size_t new_capacity = capacity + std::max(1_z, incrementor(size, capacity));
			Reserve(new_capacity);
		}
	}

#pragma endregion PushBack

#pragma region Front

	template<>
	inline int& Datum::Front()
	{
		return Get<int>(0_z);
	}

	template<>
	inline const int& Datum::Front() const
	{
		return Get<int>(0_z);
	}

	template<>
	inline float& Datum::Front()
	{
		return Get<float>(0_z);
	}

	template<>
	inline const float& Datum::Front() const
	{
		return Get<float>(0_z);
	}

	template<>
	inline std::string& Datum::Front()
	{
		return Get<std::string>(0_z);
	}

	template<>
	inline const std::string& Datum::Front() const
	{
		return Get<std::string>(0_z);
	}

	template<>
	inline glm::vec4& Datum::Front()
	{
		return Get<glm::vec4>(0_z);
	}

	template<>
	inline const glm::vec4& Datum::Front() const
	{
		return Get<glm::vec4>(0_z);
	}

	template<>
	inline glm::mat4& Datum::Front()
	{
		return Get<glm::mat4>(0_z);
	}

	template<>
	inline const glm::mat4& Datum::Front() const
	{
		return Get<glm::mat4>(0_z);
	}

	template<>
	inline RTTI*& Datum::Front()
	{
		return Get<RTTI*>(0_z);
	}

	template<>
	inline RTTI* const& Datum::Front() const
	{
		return Get<RTTI*>(0_z);
	}

	template<>
	inline Scope& Datum::Front()
	{
		return Get<Scope>(0_z);
	}

	template<>
	inline const Scope& Datum::Front() const
	{
		return Get<Scope>(0_z);
	}

#pragma endregion Front

#pragma region Back

	template<>
	inline int& Datum::Back()
	{
		return Get<int>(size - 1_z);
	}

	template<>
	inline const int& Datum::Back() const
	{
		return Get<int>(size - 1_z);
	}

	template<>
	inline float& Datum::Back()
	{
		return Get<float>(size - 1_z);
	}

	template<>
	inline const float& Datum::Back() const
	{
		return Get<float>(size - 1_z);
	}

	template<>
	inline std::string& Datum::Back()
	{
		return Get<std::string>(size - 1_z);
	}

	template<>
	inline const std::string& Datum::Back() const
	{
		return Get<std::string>(size - 1_z);
	}

	template<>
	inline glm::vec4& Datum::Back()
	{
		return Get<glm::vec4>(size - 1_z);
	}

	template<>
	inline const glm::vec4& Datum::Back() const
	{
		return Get<glm::vec4>(size - 1_z);
	}

	template<>
	inline glm::mat4& Datum::Back()
	{
		return Get<glm::mat4>(size - 1_z);
	}

	template<>
	inline const glm::mat4& Datum::Back() const
	{
		return Get<glm::mat4>(size - 1_z);
	}

	template<>
	inline RTTI*& Datum::Back()
	{
		return Get<RTTI*>(size - 1_z);
	}

	template<>
	inline RTTI* const& Datum::Back() const
	{
		return Get<RTTI*>(size - 1_z);
	}

	template<>
	inline Scope& Datum::Back()
	{
		return Get<Scope>(size - 1_z);
	}

	template<>
	inline const Scope& Datum::Back() const
	{
		return Get<Scope>(size - 1_z);
	}

#pragma endregion Back

#pragma region SetStorage

	template<>
	inline void Datum::SetStorage(int* new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::Integer, new_size);
	}

	template<>
	inline void Datum::SetStorage(float* new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::Float, new_size);
	}

	template<>
	inline void Datum::SetStorage(std::string* new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::String, new_size);
	}

	template<>
	inline void Datum::SetStorage(glm::vec4* new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::Vector, new_size);
	}

	template<>
	inline void Datum::SetStorage(glm::mat4* new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::Matrix, new_size);
	}

	template<>
	inline void Datum::SetStorage(RTTI** new_data, const size_t& new_size)
	{
		SetStorage(new_data, Datum::DatumTypes::Pointer, new_size);
	}

	inline void Datum::SetStorage(void* new_data, const Datum::DatumTypes& other_type, const size_t& new_size)
	{
		if ((type == DatumTypes::Unknown) || (other_type != type))
		{
			throw std::runtime_error("Cannot set the storage type on this type.");
		}

		if (!is_external && capacity > 0_z)
		{
			throw std::runtime_error("Cannot adjust storage, realloc called.");
		}

		data.vp = new_data;
		size = new_size;
		capacity = new_size;
		is_external = true;
	}

#pragma endregion SetStorage

#pragma region Remove

	template<>
	inline bool Datum::Remove(const int& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot remove on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

	template<>
	inline bool Datum::Remove(const float& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot remove on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

	template<>
	inline bool Datum::Remove(const std::string& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot resize on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

	template<>
	inline bool Datum::Remove(const glm::vec4& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot resize on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

	template<>
	inline bool Datum::Remove(const glm::mat4& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot resize on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

	template<>
	inline bool Datum::Remove(RTTI* const& value)
	{
		if (is_external)
		{
			throw std::runtime_error("Cannot resize on external data.");
		}

		return RemoveAt(IndexOf(value));
	}

#pragma endregion Remove

#pragma region IndexOf

	template<>
	inline const size_t Datum::IndexOf(const int& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.i[index] == value)
			{
				break;
			}
		}

		return index;
	}

	template<>
	inline const size_t Datum::IndexOf(const float& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.f[index] == value)
			{
				break;
			}
		}

		return index;
	}

	template<>
	inline const size_t Datum::IndexOf(const std::string& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.s[index] == value)
			{
				break;
			}
		}

		return index;
	}

	template<>
	inline const size_t Datum::IndexOf(const glm::vec4& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.v[index] == value)
			{
				break;
			}
		}

		return index;
	}

	template<>
	inline const size_t Datum::IndexOf(const glm::mat4& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.m[index] == value)
			{
				break;
			}
		}

		return index;
	}

	template<>
	inline const size_t Datum::IndexOf(RTTI* const& value) const
	{
		size_t index;
		for (index = 0; index < size; ++index)
		{
			if (data.r[index] == value)
			{
				break;
			}
		}

		return index;
	}

#pragma endregion IndexOf

	inline bool Datum::IsExternal()
	{
		return is_external;
	}
}