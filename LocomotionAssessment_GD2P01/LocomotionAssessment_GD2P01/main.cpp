#include "LocomotionManager.h"

int main()
{
    LocomotionManager* gManager = LocomotionManager::getInstance();
    gManager->Start();

    while (LocomotionManager::CheckIfApplicationOpen())
    {
        gManager->Update();
        gManager->Render();
    }

    return 0;
}