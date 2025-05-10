#include "LocomotionManager.h"

int main()
{
    //tgui::setBackend(std::make_shared<tgui::BackendSFML>());
    tgui::Theme::setDefault("Dependencies/TGUI-1.9/themes/Black.txt");

    LocomotionManager* gManager = LocomotionManager::getInstance();
    gManager->Start();

    while (LocomotionManager::CheckIfApplicationOpen())
    {
        gManager->Update();
        gManager->Render();
    }

    tgui::setBackend(nullptr);
    return 0;
}