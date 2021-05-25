#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Action::Action(const RTTI::IdType child_id) :
		Attributed(child_id)
	{
	}

	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Action, name)},
		};
	}
}
