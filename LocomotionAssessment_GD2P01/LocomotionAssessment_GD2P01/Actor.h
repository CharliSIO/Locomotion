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
protected:
};

