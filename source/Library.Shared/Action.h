#pragma once

#include "Attributed.h"
#include "WorldState.h"
#include "Vector.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>
		/// Default constructor deleted to prevent direct instantiation
		/// </summary>
		Action() = delete;

		/// <summary>
		/// Copy constructor which creates a new action based on the existing other action
		/// </summary>
		/// <param name="other"> The original action to copy </param>
		Action(const Action& other) = default;
		
		/// <summary>
		/// Move constructor which creates a new action based on the existing other action
		/// </summary>
		/// <param name="other"> The original action to copy and move </param>
		Action(Action&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for action
		/// </summary>
		/// <param name = "other"> The action to equate this action to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		Action& operator=(const Action& other) = default;

		/// <summary>
		/// Default equality operator for action
		/// </summary>
		/// <param name = "other"> The action to equate this action to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs action (this) after equalizing them </returns>
		Action& operator=(Action&& other) = default;

		/// <summary>
		/// Destructor defaulted but meant to be overridden by all its children
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Queries the list of prescribed attributes of action
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Updates the current action
		/// </summary>
		/// <remarks> No implementation in the base, but meant to be overridden </remarks>
		/// <param name="state"> The state used to update current entities and actions </param>
		virtual void Update(WorldState& state) = 0;

		/// <summary>
		/// Queries the name of the action
		/// </summary>
		/// <returns> The name of the action </returns>
		std::string& Name() { return name; }

		/// <summary>
		/// Queries the name of the action
		/// </summary>
		/// <returns> The name of the action </returns>
		const std::string& Name() const { return name; }

		/// <summary>
		/// Sets the name of the action to the parameter
		/// </summary>
		/// <param name="new_name"> The new action name </param>
		void SetName(const std::string& new_name) { name = new_name; }

		/// <summary>
		/// Sets the name of the action to the parameter
		/// </summary>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <param name="new_name"> The new action name </param>
		void SetName(std::string&& new_name) { name = std::move(new_name); }

	protected:
		/// <summary>
		/// Creates an action and feeds the the child's id to the attributed constructor
		/// </summary>
		/// <param name="child_id"> The children's RTTI ID </param>
		explicit Action(const RTTI::IdType child_id);

	private:
		std::string name;
	};
}
