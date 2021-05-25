#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	void ActionIncrement::Update(WorldState& /*state*/)
	{
		auto [datum, scope] = Search(target);
		if (datum == nullptr)
		{
			throw std::runtime_error("Target not found.");
		}

		datum->Set(datum->Get<float>(0) + step);
	}

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", Datum::DatumTypes::String, 1, offsetof(ActionIncrement, target)},
			{ "Step", Datum::DatumTypes::Float, 1, offsetof(ActionIncrement, step) },
		};
	}
}
