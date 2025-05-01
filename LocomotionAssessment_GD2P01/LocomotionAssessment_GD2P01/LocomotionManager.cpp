#include "LocomotionManager.h"

void LocomotionManager::Start()
{
	m_InitialSetupComplete = true;
	CreateWindow("Main Window");
	CreateLocomotionAgent("Main Window");
	CreateLocomotionAgent("Main Window");
	CreateLocomotionAgent("Main Window");
}

void LocomotionManager::Update()
{
	for (int i = 0; i < m_Windows.size(); i++)
	{
		sf::Event event;
		while (m_Windows[i]->GetWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_Windows[i]->GetWindow()->close();
				delete m_Windows[i];
				m_Windows.erase(m_Windows.begin() + i);
			}
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

void LocomotionManager::CreateLocomotionAgent(std::string _InWindowName)
{
	MovingAgent* newAgent = new MovingAgent;
	GetWindowByName(_InWindowName)->GetAttachedObjects()->push_back(std::move(newAgent));
}

// Initialise static variables to prevent multiple declaration
bool LocomotionManager::m_InitialSetupComplete = false;
LocomotionManager* LocomotionManager::m_pInstance = nullptr;
std::mutex LocomotionManager::m_Mutex;

std::vector<Window*> LocomotionManager::m_Windows;
