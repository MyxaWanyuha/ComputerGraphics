#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Event.hpp"
#include <memory>

namespace SimpleEngine {

class Application
{
public:
    Application();
    virtual ~Application();
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    virtual i32 start(u32 window_width, u32 window_height, const char* title);

    virtual void on_update(){}

private:
    std::unique_ptr<class Window> m_pWindow;

    EventDispatcher m_event_dispatcher;

    bool m_bCloseWindow = false;
};

} //SimpleEngine


#endif // APPLICATION_HPP
