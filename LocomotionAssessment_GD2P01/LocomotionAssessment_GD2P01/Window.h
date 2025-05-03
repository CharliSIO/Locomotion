#pragma once
#include "MovingAgent.h"
#include <SFML/Window.hpp>

using uint = unsigned int;

// Class for holding an SFML window
class Window
{
public:
	Window(std::string _WindowName) // construct with name
	{
		m_WindowName = _WindowName;
		m_Window.setTitle(m_WindowName);
		m_Window.create(sf::VideoMode(m_WindowSize), m_WindowName);
	}
	~Window();

	void Update();
	void Render();


	sf::RenderWindow* GetWindow()
	{
		return &m_Window;
	}

	void ReCreateWindow()
	{
		m_Window.create(sf::VideoMode(m_WindowSize), m_WindowName);
	}

	std::string GetWindowName()
	{
		return m_WindowName;
	}

	auto GetAttachedObjects()
	{
		return &m_AttachedObjects;
	}

private:
	sf::RenderWindow m_Window{ sf::VideoMode(m_WindowSize), "Window" };
	sf::Vector2<uint> m_WindowSize{ 800, 800 };
	std::string m_WindowName{ "Window" };

	std::vector<Actor*> m_AttachedObjects;

};

