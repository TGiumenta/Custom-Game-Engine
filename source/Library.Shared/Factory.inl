#include "pch.h"
#include "Factory.h"

namespace FieaGameEngine
{
	template <typename T>
	HashMap<const std::string, const Factory<T>* const> Factory<T>::factories;

	template <typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& factory_name)
	{
		auto it = factories.Find(factory_name);
		return it != factories.end() ? it->second->Create() : nullptr;
	}

	template <typename T>
	inline const Factory<T>* const Factory<T>::Find(const std::string& factory_name)
	{
		auto it = factories.Find(factory_name);
		return it != factories.end() ? it->second : nullptr;
	}

	template <typename T>
	inline size_t Factory<T>::Size()
	{
		return factories.Size();
	}

	template <typename T>
	inline void Factory<T>::Clear()
	{
		factories.Clear();
	}

	template <typename T>
	inline bool Factory<T>::IsEmpty()
	{
		return (factories.Size() == 0);
	}

	template <typename T>
	inline bool Factory<T>::ContainsKey(const std::string& factory_name)
	{
		return (factories.Find(factory_name) != factories.end());
	}

	template <typename T>
	inline void Factory<T>::AddFactory(const Factory<T>& factory)
	{
		auto [was_inserted, it] = factories.Insert(std::make_pair(factory.ClassName(), &factory));

		if (!was_inserted)
		{
			throw std::runtime_error("Cannot add a factory of this type. Did you already add this factory?");
		}
	}

	template <typename T>
	inline void Factory<T>::RemoveFactory(const Factory<T>& factory)
	{
		factories.Remove(factory.ClassName());
	}
}