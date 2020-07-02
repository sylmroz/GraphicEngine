#pragma once

#include "../Subject.hpp"
#include "../Ranges.hpp"

namespace GraphicEngine::Core::Inputs
{
	template <typename EventType, template<typename> typename Container>
	class GenericClickEvent
	{
	public:

		void setNewEvents(const Container<EventType>& newEvents)
		{
			auto filteredEvent = GraphicEngine::Core::Ranges::filter(newEvents, [&](EventType e)
				{
					return std::find(std::begin(m_prevEvents), std::end(m_prevEvents), e) == std::end(m_prevEvents);
				});

			callEvents(m_allEventsDown, m_eventDown, newEvents);
			callEvents(m_allEventsUp, m_eventUp, filteredEvent);

			m_prevEvents = newEvents;
		}

		template <typename Callback>
		void onDown(Callback callback)
		{
			m_eventDown.subscribe(callback);
		}

		template <typename Callback>
		void onUp(Callback callback)
		{
			m_eventUp.subscribe(callback);
		}

		template <typename Callback>
		void onAllDown(Callback callback)
		{
			m_allEventsDown.subscribe(callback);
		}

		template <typename Callback>
		void onAllUp(Callback callback)
		{
			m_allEventsUp.subscribe(callback);
		}

	protected:
		void callEvents(Subject<Container<EventType>>& allEventsListener, Subject<EventType>& eventListener, const Container<EventType>& container)
		{
			allEventsListener.notify(container);
			for (auto e : container)
			{
				eventListener.notify(e);
			}
		}

		Container<EventType> m_prevEvents;
		Subject<Container<EventType>> m_allEventsDown;
		Subject<EventType> m_eventDown;
		Subject<Container<EventType>> m_allEventsUp;
		Subject<EventType> m_eventUp;
	};
}