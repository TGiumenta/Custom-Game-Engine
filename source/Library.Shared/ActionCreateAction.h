#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		/// <summary>
		/// Default constructor which creates the actioncreate by calling the protected action constructor which takes the id of this actioncreate
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Copy constructor which creates a new actioncreate based on the existing other actioncreate
		/// </summary>
		/// <param name="other"> The original actioncreate to copy </param>
		ActionCreateAction(const ActionCreateAction& other) = default;

		/// <summary>
		/// Move constructor which creates a new actioncreate based on the existing other actioncreate
		/// </summary>
		/// <param name="other"> The original actioncreate to copy and move </param>
		ActionCreateAction(ActionCreateAction&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actioncreate
		/// </summary>
		/// <param name = "other"> The actioncreate to equate this actioncreate to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;

		/// <summary>
		/// Default equality operator for actioncreate
		/// </summary>
		/// <param name = "other"> The actioncreate to equate this actioncreate to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs actioncreate (this) after equalizing them </returns>
		ActionCreateAction& operator=(ActionCreateAction&& other) = default;
		
		/// <summary>
		/// Default destructor which destroys the actioncreate
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Updates the current actioncreate by adding it to the state's destruction queue
		/// </summary>
		/// <param name="state"> The state used to update current action </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Queries the list of prescribed attributes of this actioncreate
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Queries the target of the actioncreate
		/// </summary>
		/// <returns> The target of the actioncreate </returns>
		std::string& Target() { return target; }

		/// <summary>
		/// Sets the target of the actioncreate to the parameter
		/// </summary>
		/// <param name="new_target"> The new actioncreate target </param>
		void SetTarget(const std::string& new_target) { target = new_target; }

		/// <summary>
		/// Queries the class prototype of the actioncreate
		/// </summary>
		/// <returns> The class prototype of the actioncreate </returns>
		std::string& Prototype() { return prototype; }

		/// <summary>
		/// Sets the class prototype of the actioncreate to the parameter
		/// </summary>
		/// <param name="new_target"> The new actioncreate class prototype </param>
		void SetPrototype(const std::string& new_prototype) { prototype = new_prototype; }

	private:
		std::string target;
		std::string prototype;
	};

	ConcreteFactory(ActionCreateAction, Scope)
}