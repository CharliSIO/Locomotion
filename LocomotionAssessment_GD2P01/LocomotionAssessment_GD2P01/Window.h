#pragma once
#include "MovingAgent.h"
#include <SFML/Window.hpp>

using uint = unsigned int;

// Class for holding an SFML window
class Window
{
public:
	Window(std::string _WindowName, sf::Vector2u _Size) // construct with name
	{
		m_WindowName = _WindowName;
		m_WindowSize = _Size;
		m_Window.setTitle(m_WindowName);
		m_Window.create(sf::VideoMode(m_WindowSize), m_WindowName);

		m_GUI.setWindow(m_Window);
	}
	~Window();

	void Update();
	void Render();

	void SetActorBoundingBox(sf::Vector2f _Size)
	{
		for (auto& obj : m_AttachedObjects)
		{
			obj->SetBoundsSize(_Size);
		}
	}

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

	auto GetWindowSize()
	{
		return sf::Vector2f(m_WindowSize.x, m_WindowSize.y);
	}

	auto GetGUI()
	{
		return &m_GUI;
	}

protected:
	sf::RenderWindow m_Window{ sf::VideoMode(m_WindowSize), "Window" };
	sf::Vector2u m_WindowSize{ 800, 800 };
	std::string m_WindowName{ "Window" };

	std::vector<Actor*> m_AttachedObjects;

	tgui::Gui m_GUI;

};

