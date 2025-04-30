#pragma once
#include "SFML/Graphics.hpp"

class Actor
{
public:
	Actor() = default;
	~Actor() = default;

	virtual void Update() {};
	virtual void Render(sf::RenderWindow* _RenWindow) {};
protected:
};

