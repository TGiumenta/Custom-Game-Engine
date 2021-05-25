#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf final : public Action
	{
		RTTI_DECLARATIONS(ActionListIf, Action)

	public:
		/// <summary>
		/// Default constructor which creates the actionlistif by calling the protected actionlist constructor which takes the id of this actionlistif
		/// </summary>
		ActionListIf();

		/// <summary>
		/// Copy constructor which creates a new actionlistif based on the existing other actionlistif
		/// </summary>
		/// <param name="other"> The original actionlistif to copy </param>
		ActionListIf(const ActionListIf& other) = default;

		/// <summary>
		/// Move constructor which creates a new actionlistif based on the existing other actionlistif
		/// </summary>
		/// <param name="other"> The original actionlistif to copy and move </param>
		ActionListIf(ActionListIf&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actionlistif
		/// </summary>
		/// <param name = "other"> The actionlistif to equate this actionlistif to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionListIf& operator=(const ActionListIf& other) = default;

		/// <summary>
		/// Default equality operator for actionlistif
		/// </summary>
		/// <param name = "other"> The actionlistif to equate this actionlistif to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs actionlistif (this) after equalizing them </returns>
		ActionListIf& operator=(ActionListIf&& other) = default;

		/// <summary>
		/// Default destructor which destroys the actionlistif
		/// </summary>
		~ActionListIf() = default;

		/// <summary>
		/// Queries the condition and goes down the "then" and "else" actions depending on the conditions
		/// </summary>
		/// <param name="state"> The state used to update current action </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Queries the list of prescribed attributes of actionlistif
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Queries the condition of the actionlistif
		/// </summary>
		/// <returns> The target of the actionlistif </returns>
		bool Condition() { return condition; }

		/// <summary>
		/// Sets the condition of the actionlistif to the parameter
		/// </summary>
		/// <param name="new_target"> The new actionlistif condition </param>
		void SetCondition(bool new_condition) { condition = new_condition; }

		/// <summary>
		/// Queries the order vector and finds all the "then" actions of this actionlistif
		/// </summary>
		/// <returns> The "then" actions of this actionlistif </returns>
		Datum& ThenActions() { return order.at(then_identifier)->second; }

		/// <summary>
		/// Queries the order vector and finds all the "else" actions of this actionlistif
		/// </summary>
		/// <returns> The "else" actions of this actionlistif </returns>
		Datum& ElseActions() { return order.at(else_identifier)->second; }

	private:
		void LoopActions(WorldState& state, Datum& actions);

		int32_t condition;

		static const size_t then_identifier = 2;
		static const size_t else_identifier = 3;
	};

	ConcreteFactory(ActionListIf, Scope)
}

