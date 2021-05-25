#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() : 
		Reaction(ReactionAttributed::TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	const Vector<Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "Subtype", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, subtype)},
		};
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		const Event<EventMessageAttributed>* attributed_event = event.As<Event<EventMessageAttributed>>();
		if (attributed_event)
		{
			if (attributed_event->Message().Subtype() == subtype)
			{
				EventMessageAttributed& message = const_cast<EventMessageAttributed&>(attributed_event->Message());
				for (auto const& attribute : message.AuxiliaryAttributes())
				{
					Datum& datum = message.AppendAuxililaryAttribute(attribute->first);
					datum.SetType(attribute->second.Type());
					for (size_t index = 0; index < attribute->second.Size(); ++index)
					{
						datum[index] = attribute->second[index];
					}
				}

				ActionList::Update(attributed_event->Message().GetWorldState());
			}
		}
	}
}