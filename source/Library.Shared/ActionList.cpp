#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() : 
		Action(ActionList::TypeIdClass())
	{
	}

	ActionList::ActionList(const RTTI::IdType child_id) :
		Action(child_id)
	{
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions", Datum::DatumTypes::Table, 0, 0 },
		};
	}

	void ActionList::CreateAction(const std::string& class_name, const std::string& instance_name)
	{
		Scope* context = Factory<Scope>::Create(class_name);
		assert(context != nullptr);
		
		Action* action = context->As<Action>();
		assert(action != nullptr);

		Adopt(*context, "Actions");
		action->SetName(instance_name);
	}

	void ActionList::Update(WorldState& state)
	{
		state.SetCurrentAction(this);

		Datum& actions = Actions();
		for (size_t index = 0; index < actions.Size(); ++index)
		{
			Scope* child = &actions.Get<Scope>(index);
			assert(child->Is(Action::TypeIdClass()));
			Action& action = static_cast<Action&>(*child);
			action.Update(state);
		}
	}
}