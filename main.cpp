#include <memory>
#include "Logger.h"
#include "Menu.h"

int main()
{
    Logger::Debug("Starting JSnake application");

    auto menu = std::make_unique<Menu>();

    bool success = menu->Initialize();
    if(success)
    {
        menu->RunLoop();
    }
    else
    {
        Logger::Error("Menu initialization failed");
    }

    menu->Shutdown();

    Logger::Debug("Shutting down JSnake application");

    return 0;
}
