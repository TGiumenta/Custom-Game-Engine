#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Reaction::Reaction(RTTI::IdType child_id) :
		ActionList(child_id)
	{

	}

	void Reaction::Update(WorldState&)
	{
		assert(false);
	}
}