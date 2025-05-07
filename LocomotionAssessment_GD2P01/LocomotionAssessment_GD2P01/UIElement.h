#pragma once
#include "Actor.h"
class UIElement : public Actor
{
public:
	UIElement() {}
	~UIElement() {}

	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow* _RenWindow) = 0;

	virtual void Action() = 0;
protected:
	
};

