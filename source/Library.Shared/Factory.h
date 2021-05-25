#pragma once

#include "HashMap.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	/// <summary>
	/// A templated abstract base class that will allow users to create classes based 
	/// on string name. This will be extremely useful for parsing through JSON, since files 
	/// will always contain strings and we can use that to our advantage.
	/// </summary>
	template <typename T>
	class Factory
	{
	public:
		/// <summary>
		/// Defaulted destructor that's meant to be overwritten 
		/// </summary> 
		virtual ~Factory() = default;

		/// <summary>
		/// Polymorphic method which retrieves the name of the class that's contained in a particular factory
		/// </summary>
		/// <returns> The class name </returns>
		virtual const std::string& ClassName() const = 0;
		
		/// <summary>
		/// Polymorphic method which creates a new factory which is owned by the caller
		/// </summary>
		/// <remarks> Wrapper for that class' default constructor </remarks>
		/// <returns> A pointer to the new factory of whichever type defined by the macro </returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Creates a new Factory by name which is owned by the caller
		/// </summary>
		/// <param name="factory_name"> The class name to default construct </param>
		/// <returns> A pointer to the new factory based on the parameter </returns>
		static gsl::owner<T*> Create(const std::string& factory_name);
		
		/// <summary>
		/// Finds a factory by name in the static map
		/// </summary>
		/// <param name="factory_name"> The factory name to search for </param>
		/// <returns> A pointer to the factory found, or nullptr if not found </returns>
		static const Factory* const Find(const std::string& factory_name);
		
		/// <summary>
		/// Queries the size of the map
		/// </summary>
		/// <returns> Number of factories within the HashMap </returns>
		static size_t Size();
		
		/// <summary>
		/// A wrapper for clearing hashmap's clear, which removes all the factories
		/// </summary>
		static void Clear();
		
		/// <summary>
		/// Queries the size of the map and asks if it's equal to 0
		/// </summary>
		/// <returns> An indicator if there are no factories added to the map </returns>
		static bool IsEmpty();
		
		/// <summary>
		/// A wrapper for Find, determines if a key is in the map without returning the Factory
		/// </summary>
		/// <param name="factory_name"> The factory to search for </param>
		/// <returns> An indicator if the factory is in the map </returns>
		static bool ContainsKey(const std::string& factory_name);

	protected:
		/// <summary>
		/// Adds a new constructed factory to the map. Used by the macro 
		/// </summary>
		/// <param name="factory"> The factory to add (not by name) </param>
		/// <exception cref="runtime_error"> If the factory already exists in the map </exception>
		static void AddFactory(const Factory& factory);
		
		/// <summary>
		/// Removes the parameterized factory from the map. Used by the macro
		/// </summary>
		/// <param name="factory"> The factory to remove (not by name) </param>
		static void RemoveFactory(const Factory& factory);

	private:
		static HashMap<const std::string, const Factory* const> factories;
	};

#define ConcreteFactory(ConcreteProduct, AbstractProduct)												\
	class ConcreteProduct##Factory final : FieaGameEngine::Factory<AbstractProduct>						\
	{																									\
	public:																								\
		ConcreteProduct##Factory() { AddFactory(*this); }												\
		~ConcreteProduct##Factory() { RemoveFactory(*this); }											\
		const std::string& ClassName() const override  { return class_name; }							\
		gsl::owner<AbstractProduct*> Create() const override { return new ConcreteProduct(); }			\
																										\
	private:																							\
		inline static const std::string class_name = #ConcreteProduct;														\
	};		
};

#include "Factory.inl"