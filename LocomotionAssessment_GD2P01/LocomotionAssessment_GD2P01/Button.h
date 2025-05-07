#pragma once
#include "UIElement.h"
class Button : public UIElement
{
public:
	Button( sf::Vector2f _Size, std::string _Text, sf::Color _Colour);
	~Button() {}

	void Update() override;
	void Render(sf::RenderWindow* _RenWindow) override;

	void OnClicked();
	void Action() override;
protected:
	sf::RectangleShape m_ButtonShape;
	sf::FloatRect m_ButtonBounds;
	sf::Font m_Font{ "Resources/BubblegumSans-Regular.ttf" };
	sf::Text m_ButtonText{ m_Font };

	sf::Color m_Colour;
	sf::Color m_HoverColour;
};

