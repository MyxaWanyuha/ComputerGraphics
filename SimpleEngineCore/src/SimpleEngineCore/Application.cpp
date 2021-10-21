#include "SimpleEngineCore/Application.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Window.hpp"

namespace SimpleEngine {

Application::Application()
{
    LOG_INFO("Starting Application");
}

Application::~Application()
{
    LOG_INFO("Closing Application");
}

i32 Application::start(u32 window_width, u32 window_height, const char* title)
{
    m_pWindow = std::make_unique<Window>(title, window_width, window_height);

    m_event_dispatcher.add_event_listener<EventMouseMoved>(
        [](EventMouseMoved& event)
        {
            //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
        });

    m_event_dispatcher.add_event_listener<EventWindowResize>(
        [](EventWindowResize& event)
        {
            LOG_INFO("[Resized] Change size to {0}x{1}", event.width, event.height);
        });

    m_event_dispatcher.add_event_listener<EventWindowClose>(
        [&](EventWindowClose& event)
        {
            LOG_INFO("[WindowClose]");
            m_bCloseWindow = true;
        });

    m_pWindow->set_event_callback(
        [&](BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        });

    m_event_dispatcher.add_event_listener<EventKeyPressed>(
        [&](EventKeyPressed& event)
        {
            //LOG_INFO("Pressed key {0} | scancode {1} | action {2} | mods {3}", event.key, event.scancode, event.action, event.mods);
        });

    m_event_dispatcher.add_event_listener<EventKeyReleased>(
        [&](EventKeyReleased& event)
        {
            //LOG_INFO("Released key {0} | scancode {1} | action {2} | mods {3}", event.key, event.scancode, event.action, event.mods);
        });

    while(!m_bCloseWindow)
    {
        m_pWindow->on_update();
        on_update();
    }

    m_pWindow = nullptr;

    return 0;
}

}
