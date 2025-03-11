#ifndef _JOJ_EVENT_MANAGER_H
#define _JOJ_EVENT_MANAGER_H

#include "core/defines.h"

#include <functional>
#include "jevent.h"
#include <unordered_map>
#include "core/vector.h"

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
		std::unordered_map<EventType, Vector<Callback>> m_listeners;

		EventManager();
		~EventManager();
	};
}

#endif // _JOJ_EVENT_MANAGER_H