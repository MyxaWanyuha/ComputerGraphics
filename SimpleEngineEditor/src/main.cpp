#include <iostream>
#include <memory>
#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Application.hpp"

class MyApp : public SimpleEngine::Application
{
public:
    virtual ~MyApp() override{};
    virtual void on_update() override
    {
        //std::cout << "Update frame: " << ++f << '\n';
    }
    i32 f = 0;
};

int main()
{
    auto myApp = std::make_unique<MyApp>();

    i32 retCode = myApp->start(1024, 768, "TEST APP");

    return retCode;
}
