#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

// Class for holding an SFML window
class Window
{
public:
	Window(std::string _WindowName) // construct with name
	{
		m_WindowName = _WindowName;
		m_Window.setTitle(m_WindowName);
		m_Window.create(sf::VideoMode(m_WindowSize.x, m_WindowSize.y), m_WindowName);
	}
	~Window() = default;

	sf::RenderWindow* GetWindow()
	{
		return &m_Window;
	}

private:
	sf::RenderWindow m_Window{ sf::VideoMode(m_WindowSize.x, m_WindowSize.y), "Window" };
	sf::Vector2f m_WindowSize{ 800, 800 };
	std::string m_WindowName{ "Window" };
};

