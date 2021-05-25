#pragma once

#include <chrono>
#include <memory>
#include "EventPublisher.h"
#include "Vector.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated class which is carries a message (information) based on the event. This class also carries 
	/// the list of subscribers which will know how to deliver this event.
	/// </summary>
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)

	public:
		/// <summary>
		/// Constructor which creates an event based on the payload, aka the message
		/// </summary>
		/// <param name="new_message"> The payload which contains the information of the event </param>
		explicit Event(const T& new_message);
		
		/// <summary>
		/// Constructor which creates an event based on the payload, aka the message
		/// </summary>
		/// <param name="new_message"> The payload which contains the information of the event </param>
		/// <remarks> Payload is passed by rvalue instead of lvalue </remarks>
		explicit Event(T&& new_message);

		/// <summary>
		/// Copy constructor which creates a new event based on the existing other event
		/// </summary>
		/// <param name="other"> The original event to copy </param>
		Event(const Event& other) = default;
		
		/// <summary>
		/// Copy constructor which creates a new event based on the existing other event
		/// </summary>
		/// <param name="other"> The original event to copy </param>
		/// <remarks> Copy constructor uses rvalues instead of lvalues </remarks>
		Event(Event&& other) = default;
		
		/// <summary>
		/// Default equality operator for event
		/// </summary>
		/// <param name = "other"> The event to equate this event to </param>
		/// <returns> The lhs event (this) after equalizing them </returns>
		Event& operator=(const Event& other) = default;
		
		/// <summary>
		/// Default equality operator for event
		/// </summary>
		/// <param name = "other"> The event to equate this event to </param>
		/// <remarks> Equality operator uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs event (this) after equalizing them </returns>
		Event& operator=(Event&& other) = default;
		
		/// <summary>
		/// Destructor meant to be overridden by child events
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Adds the parameterized subscriber to the list of subscribers for this event, barring duplicates
		/// </summary>
		/// <param name="subscriber"> The subscriber to add to the list </param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Queries the list to determine the amount of subscribers this event needs to speak to
		/// </summary>
		/// <returns> The number of subscribers for this event </returns>
		static size_t Size();

		/// <summary>
		/// Removes the parameterized subscriber from the list of subscribers for this event
		/// </summary>
		/// <param name="subscriber"></param>
		/// <returns></returns>
		static bool Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Clears the event queue and also shrinks it down to 0 capacity awaiting new events
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Queries this event and returns back the information of this event 
		/// </summary>
		/// <returns> The payload, which is the information of the event </returns>
		const T& Message() const;

	private:
		T message;
		static Vector<EventSubscriber*> subscribers;
	};

	/// <summary>
	/// Class (with a defaulted rule of 6) that handles all of the events and the delivery of those events
	/// </summary>
	class EventQueue final
	{
	public:
		/// <summary>
		/// Adds a publisher to the queue
		/// </summary>
		/// <param name="publisher"> The publisher to enqueue </param>
		/// <param name="delay"> Amount of time to delay the event from firing </param>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime& game_time, std::chrono::milliseconds delay = std::chrono::milliseconds(0));
		
		/// <summary>
		/// Immediately deliver the event via the parameterized publisher
		/// </summary>
		/// <param name="publisher"> The publisher to send ie immediately deliver </param>
		void Send(std::shared_ptr<EventPublisher> publisher) { publisher->Deliver(); }

		/// <summary>
		/// Update the entire queue, deliver expired events and remove them from the queue
		/// </summary>
		/// <param name="game_time"> The game_time reference for the world to use as a reference for time </param>
		void Update(const GameTime& game_time);
		
		/// <summary>
		/// Clears the event queue
		/// </summary>
		void Clear() { events.Clear(); }

		/// <summary>
		/// Queries the queue and determines if it has no events in it
		/// </summary>
		/// <returns> An indicator if there are any events in the queue </returns>
		bool IsEmpty() const { return events.IsEmpty(); }

		/// <summary>
		/// Determines the amount of events in the queue
		/// </summary>
		/// <returns> An indicator of the size of the vector </returns>
		size_t Size() const { return events.Size(); }

	private:
		struct QueueEntry
		{
			std::shared_ptr<EventPublisher> event_publisher;
			std::chrono::high_resolution_clock::time_point enqueue_time;
			std::chrono::milliseconds delay;

			bool IsExpired(std::chrono::high_resolution_clock::time_point current) const;
		};

		Vector<QueueEntry> buffer;
		Vector<QueueEntry> events;
	};
}

#include "Event.inl"