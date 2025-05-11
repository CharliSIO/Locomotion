#include "LocomotionManager.h"

int main()
{
    tgui::Theme::setDefault("Dependencies/TGUI-1.9/themes/Black.txt");

    LocomotionManager* gManager = LocomotionManager::getInstance();
    gManager->Start();

    while (LocomotionManager::CheckIfApplicationOpen())
    {
        gManager->Update();
        gManager->Render();
    }

    return 0;
}