#include "pch.h"
#include "ActionDestroyAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{
	}

	void ActionDestroyAction::Update(WorldState& state)
	{
		state.SetCurrentAction(this);
		state.AddDestroyAction(*GetParent(), target);
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{			
			{ "Target", Datum::DatumTypes::String, 1, offsetof(ActionDestroyAction, target) },
		};
	}
}
