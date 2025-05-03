#pragma once
#include "Actor.h"
class Line : public Actor
{
public:
	Line(sf::Vector2f _start, sf::Vector2f _end, sf::Color _colour)
	{
		Update(_start, _end);
		m_Line[0].color = _colour;
		m_Line[1].color = _colour;
	}

	void Update() override {}
	void Update(sf::Vector2f _start, sf::Vector2f _end)
	{
		m_Line[0].position.x = _start.x;
		m_Line[0].position.y = _start.y;
		m_Line[1].position.x = _end.x;
		m_Line[1].position.y = _end.y;
	}

	void Render(sf::RenderWindow* _RenWindow) override
	{
		_RenWindow->draw(m_Line, sizeof m_Line, sf::PrimitiveType::Lines);
	}

private:
	sf::Vertex m_Line[2];
};

