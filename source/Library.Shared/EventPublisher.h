#pragma once

#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Class used by various managers or classes that will be actually handling the usage of the events.
	/// These classes are going to be handling the message payload and using it to instigate some action
	/// </summary>
	class EventSubscriber
	{

	public:
		/// <summary>
		/// Destructor meant to be overridden by derived classes, currently has no implementation in the base class
		/// </summary>
		virtual ~EventSubscriber() {};

		/// <summary>
		/// Handles the payload event, determines what event is being used, and does the handling of the event in 
		/// the derived class
		/// </summary>
		/// <param name="event"> The event which carries the payload </param>
		virtual void Notify(const class EventPublisher& event) = 0;
	};

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		/// <summary>
		/// Default constructor deleted to prevent direct instantiation
		/// </summary>
		EventPublisher() = delete;

		/// <summary>
		/// Copy constructor which creates a new publisher based on the existing other publisher
		/// </summary>
		/// <param name="other"> The original publisher to copy </param>
		EventPublisher(const EventPublisher& other) = default;

		/// <summary>
		/// Copy constructor which creates a new publisher based on the existing other publisher
		/// </summary>
		/// <param name="other"> The original publisher to copy </param>
		/// <remarks> Copy constructor uses rvalues instead of lvalues </remarks>
		EventPublisher(EventPublisher&& other) = default;
		
		/// <summary>
		/// Default equality operator for publisher
		/// </summary>
		/// <param name = "other"> The publisher to equate this publisher to </param>
		/// <returns> The lhs publisher (this) after equalizing them </returns>
		EventPublisher& operator=(const EventPublisher& other) = default;
		
		/// <summary>
		/// Default equality operator for publisher
		/// </summary>
		/// <param name = "other"> The publisher to equate this publisher to </param>
		/// <remarks> Equality operator uses rvalues instead of lvalues </remarks>
		/// <returns> The lhs publisher (this) after equalizing them </returns>
		EventPublisher& operator=(EventPublisher&& other) = default;
		
		/// <summary>
		/// Default destructor with intention to be overriden
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Calls notify on all of the contained subscribers
		/// </summary>
		void Deliver() const;

	protected:
		/// <summary>
		/// Default constructor which requires a list of subscribers for this publisher to hold
		/// </summary>
		explicit EventPublisher(const Vector<EventSubscriber*>& new_subscribers);

		const Vector<EventSubscriber*>* subscribers;
	};
}
