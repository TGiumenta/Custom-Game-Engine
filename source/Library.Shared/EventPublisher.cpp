#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& new_subscribers) :
		subscribers(&new_subscribers)
	{
	}

	void EventPublisher::Deliver() const
	{
		for (auto& subscriber : *subscribers)
		{
			assert(subscriber != nullptr);
			subscriber->Notify(*this);
		}
	}
}