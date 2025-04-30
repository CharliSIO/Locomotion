#include "LocomotionManager.h"

int main()
{
    LocomotionManager* gManager = LocomotionManager::getInstance();
    if (!gManager->CreateWindow("Main Window")) return -1;

    while (LocomotionManager::CheckIfApplicationOpen())
    {
        gManager->Update();
        gManager->Render();
    }

    return 0;
}