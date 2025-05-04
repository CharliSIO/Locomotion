#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

class Actor
{
public:
	Actor() = default;
	virtual ~Actor() = default;

	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow* _RenWindow) = 0;
	void SetBoundsSize(sf::Vector2f _size)
	{
		m_Bounds.size = _size;
	}
	auto GetBoundsSize()
	{
		return m_Bounds.size;
	}
	auto GetWindow()
	{
		return m_Window;
	}
	void SetWindow(class Window* _window)
	{
		m_Window = _window;
	}
protected:
	sf::FloatRect m_Bounds;
	class Window* m_Window = nullptr;
};

