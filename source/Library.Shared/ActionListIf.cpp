#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf() :
		Action(ActionListIf::TypeIdClass())
	{
	}

	void ActionListIf::Update(WorldState& state)
	{
		state.SetCurrentAction(this);

		if (condition)
		{
			LoopActions(state, ThenActions());
		}
		else
		{
			LoopActions(state, ElseActions());
		}
	}

	void ActionListIf::LoopActions(WorldState& state, Datum& actions)
	{
		for (size_t index = 0; index < actions.Size(); ++index)
		{
			Action* action = static_cast<Action*>(&actions.Get<Scope>(index));
			assert(action != nullptr);
			action->Update(state);
		}
	}

	const Vector<Signature> ActionListIf::Signatures()
	{
		return Vector<Signature>
		{
			{ "Condition", Datum::DatumTypes::Integer, 1, offsetof(ActionListIf, condition)},
			{ "Then", Datum::DatumTypes::Table, 0, 0 },
			{ "Else", Datum::DatumTypes::Table, 0, 0 },
		};
	}
}
