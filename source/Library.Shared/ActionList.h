#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Default constructor which creates the actionlist by calling the protected action constructor which takes the id of this actionlist
		/// </summary>
		ActionList();

		/// <summary>
		/// Copy constructor which creates a new actionlist based on the existing other actionlist
		/// </summary>
		/// <param name="other"> The original actionlist to copy </param>
		ActionList(const ActionList& other) = default;

		/// <summary>
		/// Move constructor which creates a new actionlist based on the existing other actionlist
		/// </summary>
		/// <param name="other"> The original actionlist to copy and move </param>
		ActionList(ActionList&& other) noexcept = default;

		/// <summary>
		/// Default equality operator for actionlist
		/// </summary>
		/// <param name = "other"> The actionlist to equate this actionlist to </param>
		/// <returns> The lhs action (this) after equalizing them </returns>
		ActionList& operator=(const ActionList& other) = default;

		/// <summary>
		/// Default equality operator for actionlist
		/// </summary>
		/// <param name = "other"> The actionlist to equate this actionlist to </param>
		/// <remarks> Uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs actionlist (this) after equalizing them </returns>
		ActionList& operator=(ActionList&& other) = default;

		/// <summary>
		/// Default destructor which destroys the actionlist
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// Queries the list of prescribed attributes of actionlist
		/// </summary>
		/// <returns> The prescribed attributes for the class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Updates the current actionlist and all its children
		/// </summary>
		/// <param name="state"> The state used to update current action </param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Clones this current actionlist on the heap
		/// </summary>
		/// <returns> A pointer to the new heap allocated actionlist, meant to be deleted by the caller </returns>
		gsl::owner<ActionList*> Clone() const override { return new ActionList(*this); }

		/// <summary>
		/// Creates a new action, wraps scope adopt mainly
		/// </summary>
		/// <param name="class_name"> Name of the class to create a factory of </param>
		/// <param name="instance_name"> Name of the action </param>
		void CreateAction(const std::string& class_name, const std::string& instance_name);

		/// <summary>
		/// Queries the order vector and finds all the actions of this actionlist
		/// </summary>
		/// <returns> The actions of this actionlist </returns>
		Datum& Actions() { return order.at(actions_identifier)->second; }

	protected:
		/// <summary>
		/// Default constructor which creates the actionlist by calling the protected action constructor which takes the id of the parameter
		/// </summary>
		/// <param name="child_id"></param>
		explicit ActionList(const RTTI::IdType child_id);

	private:
		static const size_t actions_identifier = 2;
	};

	ConcreteFactory(ActionList, Scope)
}

