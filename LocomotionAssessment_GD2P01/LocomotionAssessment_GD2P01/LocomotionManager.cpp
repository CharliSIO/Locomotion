#include "LocomotionManager.h"

void LocomotionManager::Update()
{
	sf::Event event;
	while (m_Windows[0]->GetWindow()->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_Windows[0]->GetWindow()->close();
		}
	}
}

void LocomotionManager::Render()
{
	for (Window* win : m_Windows)
	{
		win->Render();
	}
}

// Check if a window with input name already exists, if it does then return false
// If not then create window and add it to vector with attached name
// First window created, regardless of name, will be treated as correct window
bool LocomotionManager::CreateWindow(std::string _WindowName)
{
	if (!m_Windows.empty() && GetWindowByName(_WindowName) != nullptr)
	{
		std::cout << "ERROR: Cannot make window with existing name. " << std::endl;
		return false;
	}
	m_Windows.push_back(new Window(_WindowName));
	return true;
}

// Initialise static variables to prevent multiple declaration
LocomotionManager* LocomotionManager::m_pInstance = nullptr;
std::mutex LocomotionManager::m_Mutex;

std::vector<Window*> LocomotionManager::m_Windows;
