#pragma once

#include "Action.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class ActionIncrement final : public Action
	{
	public:
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// Default constructor which creates the actionincrement by calling the protected Action constructor which takes the id of this action
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// Copy constructor which creates a new actionincrement based on the existing other actionincrement
		/// </summary>
		/// <param name="other"> The original action to copy </param>
		ActionIncrement(const ActionIncrement& other) = default;

		/// <summary>
		/// Move constructor which creates a new actionincrement based on the existing other actionincrement
		/// </summary>
		/// <param name="other"> The original actionincrement to copy and move </param>
		ActionIncrement(ActionIncrement&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actionincrement
		/// </summary>
		/// <param name = "other"> The action to equate this actionincrement to </param>
		/// <returns> The lhs actionincrement (this) after equalizing them </returns>
		ActionIncrement& operator=(const ActionIncrement& other) = default;

		/// <summary>
		/// Default equality operator for actionincrement
		/// </summary>
		/// <param name = "other"> The actionincrement to equate this actionincrement to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs actionincrement (this) after equalizing them </returns>
		ActionIncrement& operator=(ActionIncrement&& other) = default;

		/// <summary>
		/// Default destructor which destroys the actionincrement
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Clones this current actionincrement on the heap
		/// </summary>
		/// <returns> A pointer to the new heap allocated actionincrement, meant to be deleted by the caller </returns>
		gsl::owner<ActionIncrement*> Clone() const override { return new ActionIncrement(*this); }

		/// <summary>
		/// Updates the current actionincrement
		/// </summary>
		/// <param name="state"> The state used to update current entities and actions </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Queries the list of prescribed attributes of this actionincrement
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Queries the target of the actionincrement
		/// </summary>
		/// <returns> The target of the actionincrement </returns>
		std::string& Target() { return target; }

		/// <summary>
		/// Queries the target of the actioncreate
		/// </summary>
		/// <remarks> Specifically the const version of target </remarks>
		/// <returns> The target of the actioncreate </returns>
		const std::string& Target() const { return target; }
		
		/// <summary>
		/// Sets the target of the actionincrement to the parameter
		/// </summary>
		/// <param name="new_target"> The new actionincrement target </param>
		void SetTarget(const std::string& new_target) { target = new_target; }

		/// <summary>
		/// Sets the target of the actionincrement to the parameter
		/// </summary>
		/// <param name="new_target"> The new actionincrement target </param>
		void SetTarget(std::string&& new_target) { target = new_target; }

		/// <summary>
		/// Queries the step of the actionincrement
		/// </summary>
		/// <returns> The step of the actionincrement </returns>
		float Step() const { return step; }

		/// <summary>
		/// Sets the step of the actionincrement to the parameter
		/// </summary>
		/// <param name="new_step"> The new actionincrement step </param>
		void SetStep(float new_step) { step = new_step; }

	private:
		std::string target;
		float step = 1.0f;
	};

	ConcreteFactory(ActionIncrement, Scope)
}

