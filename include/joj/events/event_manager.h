#ifndef _JOJ_EVENT_MANAGER_H
#define _JOJ_EVENT_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include <functional>
#include "jevent.h"
#include <unordered_map>

namespace joj
{
	class JAPI EventManager
	{
	public:
		using Callback = std::function<void(const JEvent&)>;

		static EventManager& instance();

		void subscribe(const EventType type, Callback callback);
		void publish(const JEvent& event);

	private:
		std::unordered_map<EventType, std::vector<Callback>> m_listeners;

		EventManager();
		~EventManager();
	};
}

#endif // _JOJ_EVENT_MANAGER_H