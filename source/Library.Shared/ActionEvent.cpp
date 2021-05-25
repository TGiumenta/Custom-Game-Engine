#include "pch.h"
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent(EventQueue& new_queue) :
		Action(ActionEvent::TypeIdClass()),
		queue(&new_queue)
	{
	}

	const Vector<Signature> ActionEvent::Signatures()
	{
		return Vector<Signature>
		{
			{ "Subtype", Datum::DatumTypes::String, 1, offsetof(ActionEvent, subtype)},
			{ "Delay", Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, delay)},
		};
	}

	void ActionEvent::Update(WorldState& state)
	{
		if (queue != nullptr)
		{
			EventMessageAttributed message_attributed(state, subtype);

			for (auto& attribute : AuxiliaryAttributes())
			{
				Datum& datum = message_attributed.AppendAuxililaryAttribute(attribute->first);
				datum.SetType(attribute->second.Type());
				for (size_t index = 0; index < attribute->second.Size(); ++index)
				{
					datum[index] = attribute->second[index];
				}
			}

			std::shared_ptr<Event<EventMessageAttributed>> e = std::make_shared<Event<EventMessageAttributed>>(message_attributed);
			queue->Enqueue(e, *state.GetGameTime(), std::chrono::milliseconds(delay));
		}
	}
}