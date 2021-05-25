#pragma once

#include "ActionList.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// Default constructor deleted to prevent direct instantiation
		/// </summary>
		Reaction() = delete;

		/// <summary>
		/// Copy constructor which creates a new reaction based on the existing other reaction
		/// </summary>
		/// <param name="other"> The original reaction to copy </param>
		Reaction(const Reaction& other) = default;

		/// <summary>
		/// Move constructor which creates a new reaction based on the existing other reaction
		/// </summary>
		/// <param name="other"> The original reaction to copy and move </param>
		Reaction(Reaction&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for reaction
		/// </summary>
		/// <param name = "other"> The reaction to equate this reaction to </param>
		/// <returns> The lhs reaction (this) after equalizing them </returns>
		Reaction& operator=(const Reaction& other) = default;

		/// <summary>
		/// Default equality operator for reaction
		/// </summary>
		/// <param name = "other"> The ewaction to equate this reaction to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs reaction (this) after equalizing them </returns>
		Reaction& operator=(Reaction&& other) noexcept = default;

		/// <summary>
		/// Pure virtual destructor which indicates an abstract base class
		/// </summary>
		virtual ~Reaction() = 0 {};

		/// <summary>
		/// Update method override with no defined implementation, this is intentional
		/// </summary>
		/// <param name="state"> The state of the world </param>
		virtual void Update(WorldState& state) override;

	protected:
		/// <summary>
		/// Creates a reaction and feeds the the child's id to the actionlist constructor
		/// </summary>
		/// <param name="child_id"> The children's RTTI ID </param>
		explicit Reaction(RTTI::IdType child_id);
	};
}