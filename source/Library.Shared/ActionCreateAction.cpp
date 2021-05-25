#include "pch.h"
#include "Factory.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{
	}

	void ActionCreateAction::Update(WorldState& state)
	{
		using namespace std::string_literals;

		state.SetCurrentAction(this);
		state.AddCreateAction(*GetParent(), prototype);
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, target)},
			{ "Prototype", Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, prototype)}
		};
	}
}