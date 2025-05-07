#include "LocomotionManager.h"

void LocomotionManager::Start()
{
	m_InitialSetupComplete = true;
	CreateWindow("Main Window", {1200, 900});
	CreateLocomotionAgent(sf::Vector2f({ 100, 100 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 300, 200 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 700, 400 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 600, 400 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 700, 600 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 200, 400 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 800, 300 }), "Main Window");
	CreateLocomotionAgent(sf::Vector2f({ 400, 400 }), "Main Window");
	m_Windows[0]->SetActorBoundingBox(m_Windows[0]->GetWindowSize());

	m_MouseGizmo = new sf::CircleShape(10.0f, 10);
	m_MouseGizmo->setOrigin(sf::Vector2f(10.0f, 10.0f));
	m_MouseGizmo->setOutlineColor(sf::Color::Blue);
	m_MouseGizmo->setFillColor(sf::Color::Transparent);
	m_MouseGizmo->setOutlineThickness(-1.0f);
}

void LocomotionManager::Update()
{
	// Update delta time
	m_DeltaTime = GameClock.getElapsedTime().asSeconds();
	GameClock.restart();
	UpdateMousePosWorld();
	m_MouseGizmo->setPosition(m_MousePosWorld);

	for (auto& window : m_Windows)
	{
		window->Update();
	}

	// Check for events in windows
	for (int i = 0; i < m_Windows.size(); i++)
	{
		if (m_Windows[i]->GetWindow()->hasFocus())
		{
			m_ActiveWindow = m_Windows[i];
		}
		while (const std::optional event = m_Windows[i]->GetWindow()->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				m_Windows[i]->GetWindow()->close();
				delete m_Windows[i];
				m_Windows.erase(m_Windows.begin() + i);
				m_ActiveWindow = nullptr;
				break;
			}
		}
	}
}

void LocomotionManager::Render()
{
	for (Window* win : m_Windows)
	{
		win->Render();
		if (win->GetWindow()->hasFocus())
		{
			win->GetWindow()->draw(*m_MouseGizmo);
		}
		win->GetWindow()->display();
	}
}

// Check if a window with input name already exists, if it does then return false
// If not then create window and add it to vector with attached name
// First window created, regardless of name, will be treated as correct window
bool LocomotionManager::CreateWindow(std::string _WindowName, sf::Vector2u _Size)
{
	if (!m_Windows.empty() && GetWindowByName(_WindowName) != nullptr)
	{
		std::cout << "ERROR: Cannot make window with existing name. " << std::endl;
		return false;
	}
	m_Windows.push_back(new Window(_WindowName, _Size));
	return true;
}

void LocomotionManager::CreateLocomotionAgent(sf::Vector2f _Position, std::string _InWindowName)
{
	MovingAgent* newAgent = new MovingAgent(_Position);
	GetWindowByName(_InWindowName)->GetAttachedObjects()->push_back(std::move(newAgent));
}

// Initialise static variables to prevent multiple declaration
bool LocomotionManager::m_InitialSetupComplete = false;
bool LocomotionManager::MouseReleased = false;
LocomotionManager* LocomotionManager::m_pInstance = nullptr;
std::mutex LocomotionManager::m_Mutex;

std::vector<Window*> LocomotionManager::m_Windows;

sf::Clock LocomotionManager::GameClock;
float LocomotionManager::m_DeltaTime{ 0.0f };
sf::Vector2f LocomotionManager::m_MousePosWorld{ 0.0f, 0.0f };
sf::CircleShape* LocomotionManager::m_MouseGizmo = nullptr;
Window* LocomotionManager::m_ActiveWindow;
