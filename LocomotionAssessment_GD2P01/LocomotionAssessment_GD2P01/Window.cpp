#include "Window.h"

void Window::Render()
{
	m_Window.clear(sf::Color::Yellow);
	for (Actor* obj: m_AttachedActors)
	{
		obj->Render(&m_Window);
	}

	sf::CircleShape shape(200.f, 100);
	shape.setFillColor(sf::Color(204, 77, 5)); // Color circle
	shape.setPosition(200, 200); // Center circle
	m_Window.draw(shape);

	m_Window.display();
}
