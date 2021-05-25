#include "pch.h"
#include "Event.h"
#include <algorithm>

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime& game_time, std::chrono::milliseconds delay)
	{
		QueueEntry entry{ std::move(publisher), game_time.CurrentTime(), delay };
		buffer.PushBack(entry);
	}

	void EventQueue::Update(const GameTime& game_time)
	{
		for (QueueEntry& entry : buffer)
		{
			events.PushBack(entry);
		}

		auto it_partition = std::partition(events.begin(), events.end(), [current = game_time.CurrentTime()](QueueEntry entry)
		{return !entry.IsExpired(current); });

		for (auto it = it_partition; it != events.end(); ++it)
		{
			it->event_publisher->Deliver();
		}

		events.Remove(it_partition, events.end());

		buffer.Clear();
	}

	bool EventQueue::QueueEntry::IsExpired(std::chrono::high_resolution_clock::time_point current) const
	{
		return (current > (enqueue_time + delay));
	}
}