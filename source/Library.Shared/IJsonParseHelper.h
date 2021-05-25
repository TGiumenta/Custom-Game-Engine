#pragma once
#include <json/json.h>
#include "JsonParseCoordinator.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	/// <summary>
	/// An Abstract base class which creates the interface for all of the helpers for the 
	/// coordinator. This will be able to handle however the helper wants to interpret and 
	/// read in any data.
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

	public:
		/// <summary>
		/// Resets the helper to its initial state 
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Cleans up the helper based on how it was initialized
		/// </summary>
		virtual void Cleanup();

		/// <summary>
		/// Saves off the key/value pair into a data structure which parses a particular pair
		/// </summary>
		/// <param name="shared"> The SharedData associated with the coordinator which needs to be checked </param>
		/// <param name="key"> The key associated with the member pair </param>
		/// <param name="value"> The value associated with the member pair</param>
		/// <param name="is_array_element"> Determines if the element is from an array </param>
		/// <param name="index"> Index of potential array </param>
		/// <returns> Indicator if the parse start was successful </returns>
		virtual bool StartHandler(
			JsonParseCoordinator::SharedData& shared,
			const std::string& key,
			const Json::Value& value, 
			bool is_array_element,
			size_t index) = 0;

		/// <summary>
		/// Handles ending the states of the helper which is parsing the key/value pair
		/// </summary>
		/// <param name="shared"> The SharedData associated with the coordinator which needs to be checked </param>
		/// <param name="key"> The key associated with the member pair </param>
		/// <returns> Indicator if the parse end was successful </returns>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& shared, const std::string& key) = 0;

		/// <summary>
		/// Creates a new version of the particular helper. 
		/// <remarks> Pure virtual indicating that you cannot instantiate this base helper </remarks>
		/// </summary>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0 {};
	};
}