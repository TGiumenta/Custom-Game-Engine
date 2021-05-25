#pragma once

#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class WorldState;

	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:
		/// <summary>
		/// Default constructor deleted to prevent instantiation without a world state
		/// </summary>
		EventMessageAttributed() = delete;

		/// <summary>
		/// Default constructor specifically used to assign world state and potentially subtype 
		/// </summary>
		/// <param name="new_world_state"> The worldstate to set </param>
		/// <param name="new_subtype"> The subtype to set </param>
		EventMessageAttributed(WorldState& new_world_state, const std::string& new_subtype = "");

		/// <summary>
		/// Queries this subtype
		/// </summary>
		/// <returns> The subtype of this payload </returns>
		inline std::string& Subtype() { return subtype; }

		/// <summary>
		/// Queries this subtype
		/// </summary>
		/// <returns> The subtype of this payload </returns>
		inline const std::string& Subtype() const { return subtype; }

		/// <summary>
		/// Sets this subtype to the parameter
		/// </summary>
		/// <param name="new_subtype"> The subtype which which to set </param>
		inline void SetSubtype(const std::string& new_subtype) { subtype = new_subtype; }

		/// <summary>
		/// Sets this subtype to the parameter
		/// </summary>
		/// <param name="new_subtype"> The subtype which which to set </param>
		inline void SetSubtype(std::string&& new_subtype) { subtype = new_subtype; }

		/// <summary>
		/// Queries this payload's worldstate
		/// </summary>
		/// <returns> The payload's worldstate </returns>
		inline WorldState& GetWorldState() const { return world_state; }

	private:
		std::string subtype;
		WorldState& world_state;
	};
}

