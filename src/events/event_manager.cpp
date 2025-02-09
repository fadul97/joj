#include "events/event_manager.h"

#include <iostream>

joj::EventManager::EventManager()
{
}

joj::EventManager::~EventManager()
{
}

void joj::EventManager::subscribe(const EventType type, Callback callback)
{
    m_listeners[type].push_back(callback);
}

void joj::EventManager::publish(const JEvent& event)
{
    if (m_listeners.find(event.type) != m_listeners.end())
    {
        for (auto& callback : m_listeners[event.type])
            callback(event);
    }
}

joj::EventManager& joj::EventManager::instance()
{
    static EventManager instance;
    return instance;
}