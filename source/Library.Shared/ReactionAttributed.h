#pragma once

#include "Reaction.h"
#include "Factory.h"
#include "Vector.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:
		/// <summary>
		/// Default constructor for object initialization, subscribes from EventMessageAttribute
		/// </summary>
		ReactionAttributed();
		
		/// <summary>
		/// Copy constructor which creates a new reactionattributed based on the existing other reactionattributed
		/// </summary>
		/// <param name="other"> The original reactionattributed to copy </param>
		ReactionAttributed(const ReactionAttributed& other) = default;

		/// <summary>
		/// Move constructor which creates a new reactionattributed based on the existing other reactionattributed
		/// </summary>
		/// <param name="other"> The original reactionattributed to copy and move </param>
		ReactionAttributed(ReactionAttributed&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for reactionattributed
		/// </summary>
		/// <param name = "other"> The reactionattributed to equate this reactionattributed to </param>
		/// <returns> The lhs reactionattributed (this) after equalizing them </returns>
		ReactionAttributed& operator=(const ReactionAttributed& other) = default;
		
		/// <summary>
		/// Default equality operator for reactionattributed
		/// </summary>
		/// <param name = "other"> The reactionattributed to equate this reactionattributed to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs reactionattributed (this) after equalizing them </returns>
		ReactionAttributed& operator=(ReactionAttributed&& other) noexcept = default;
		
		/// <summary>
		/// Destructor meant to be overridden by all its children, unsubscribes from EventMessageAttribute
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// Overridden update that does nothing, but that is intended behavior. Do not remove this!
		/// </summary>
		/// <param name="world_state"> The state used to update current entities and actions </param>
		virtual void Update(WorldState& /*world_state*/) override {};

		/// <summary>
		/// Queries the list of prescribed attributes of reactionattributed
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Handles the payload event, determines what event is being used, and does the handling of the event in 
		/// the derived class
		/// </summary>
		/// <param name="event"> The event which carries the payload </param>
		virtual void Notify(const class EventPublisher& event) override;

		/// <summary>
		/// Queries the member subtype
		/// </summary>
		/// <returns> The subtype of this ReactionAttributed </returns>
		inline std::string& Subtype() { return subtype; }

		/// <summary>
		/// Queries the member subtype
		/// </summary>
		/// <remarks> Handles the const version of the other Subtype </remarks>
		/// <returns> The subtype of this ReactionAttributed </returns>
		inline const std::string& Subtype() const { return subtype; }

		/// <summary>
		/// Sets the current Subtype to the parameter
		/// </summary>
		/// <param name="new_subtype"> The subtype to set </param>
		inline void SetSubtype(const std::string& new_subtype) { subtype = new_subtype; }

		/// <summary>
		/// Sets the current Subtype to the parameter
		/// </summary>
		/// <param name="new_subtype"> The subtype to set </param>
		/// <remarks> Handles rvalues as opposed to lvalues </remarks>
		inline void SetSubtype(std::string&& new_subtype) { subtype = new_subtype; }

	private:
		std::string subtype;
	};

	ConcreteFactory(ReactionAttributed, Scope)
}