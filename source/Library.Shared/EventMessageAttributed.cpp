#include "pch.h"
#include "EventMessageAttributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed(WorldState& new_world_state, const std::string& new_subtype) :
		Attributed::Attributed(EventMessageAttributed::TypeIdClass()), 
		world_state(new_world_state),
		subtype(new_subtype)
	{
	}
}