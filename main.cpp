#include <memory>
#include "Menu.h"

int main()
{
    auto menu = std::make_unique<Menu>();
    bool success = menu->Initialize();
    if(success)
    {
        menu->RunLoop();
    }
    menu->Shutdown();
    return 0;
}
