#pragma once

#include <string>
#include <cstddef>

namespace FieaGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;
		static IdType TypeIdClass() { return 0; }

		virtual ~RTTI() = default;

		virtual FieaGameEngine::RTTI::IdType TypeIdInstance() const = 0;

		virtual RTTI* QueryInterface(const IdType)
		{
			return nullptr;
		}

		virtual bool Is(IdType) const
		{
			return false;
		}

		virtual bool Is(const std::string&) const
		{
			return false;
		}

		template <typename T>
		const T* As() const
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<const T*>(this) : nullptr);
		}

		template <typename T>
		T* As()
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}

		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#define RTTI_DECLARATIONS(Type, ParentType)																						\
		public:																													\
			static std::string TypeName() { return std::string(#Type); }														\
			static FieaGameEngine::RTTI::IdType TypeIdClass() { return sRunTimeTypeId; }										\
			FieaGameEngine::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
			FieaGameEngine::RTTI* QueryInterface(const RTTI::IdType id) override												\
            {																													\
				return (id == sRunTimeTypeId ? reinterpret_cast<FieaGameEngine::RTTI*>(this) : ParentType::QueryInterface(id)); \
            }																													\
			bool Is(FieaGameEngine::RTTI::IdType id) const override																\
			{																													\
				return (id == sRunTimeTypeId ? true : ParentType::Is(id));														\
			}																													\
			bool Is(const std::string& rtti_name) const override																\
			{																													\
				return (rtti_name == TypeName() ? true : ParentType::Is(rtti_name));											\
			}																													\
			private:																											\
				static const FieaGameEngine::RTTI::IdType sRunTimeTypeId;

#define RTTI_DEFINITIONS(Type) const FieaGameEngine::RTTI::IdType Type::sRunTimeTypeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type::sRunTimeTypeId);
}