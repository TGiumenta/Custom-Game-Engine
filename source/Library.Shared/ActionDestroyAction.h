#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default constructor which creates the actiondestroy by calling the protected action constructor which takes the id of this actiondestroy
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Copy constructor which creates a new actiondestroy based on the existing other actiondestroy
		/// </summary>
		/// <param name="other"> The original action to copy </param>
		ActionDestroyAction(const ActionDestroyAction& other) = default;

		/// <summary>
		/// Move constructor which creates a new actiondestroy based on the existing other actiondestroy
		/// </summary>
		/// <param name="other"> The original action to copy and move </param>
		ActionDestroyAction(ActionDestroyAction&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actiondestroy
		/// </summary>
		/// <param name = "other"> The actiondestroy to equate this actiondestroy to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& other) = default;

		/// <summary>
		/// Default equality operator for actiondestroy
		/// </summary>
		/// <param name = "other"> The actiondestroy to equate this actiondestroy to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& other) = default;

		/// <summary>
		/// Default destructor which destroys the actiondestroy
		/// </summary>
		~ActionDestroyAction() = default;

		/// <summary>
		/// Updates the current actiondestroy by adding it to the state's creation queue
		/// </summary>
		/// <param name="state"> The state used to update current action </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Queries the list of prescribed attributes of this actiondestroy
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Queries the target of the actiondestroy
		/// </summary>
		/// <returns> The target of the actiondestroy </returns>
		std::string& Target() { return target; }

		/// <summary>
		/// Sets the target of the actiondestroy to the parameter
		/// </summary>
		/// <param name="new_target"> The new actiondestroy target </param>
		void SetTarget(const std::string& new_target) { target = new_target; }

	private:
		std::string target;
	};

	ConcreteFactory(ActionDestroyAction, Scope)
}