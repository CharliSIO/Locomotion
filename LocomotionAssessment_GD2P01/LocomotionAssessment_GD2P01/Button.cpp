#include "Button.h"
#include "LocomotionManager.h"

Button::Button(sf::Vector2f _Size, std::string _Text, sf::Color _Colour)
{
	m_ButtonShape.setSize(_Size);

	m_Colour = _Colour;
	m_ButtonShape.setFillColor(m_Colour);
	m_HoverColour = sf::Color(m_Colour.r * 0.75, m_Colour.g * 0.75, m_Colour.b * 0.75);

	m_ButtonText = sf::Text(m_Font, _Text, 15);
}

void Button::Update()
{
	if (LocomotionManager::GetActiveWindow() == m_Window)
	{
		if (m_ButtonBounds.contains(LocomotionManager::GetMousePos()))
		{
			m_ButtonShape.setFillColor(m_HoverColour);
			if (LocomotionManager::MouseReleased)
			{
				OnClicked();
			}
		}
		else
		{
			m_ButtonShape.setFillColor(m_Colour);
		}
	}
}

void Button::Render(sf::RenderWindow* _RenWindow)
{
	_RenWindow->draw(m_ButtonShape);
	_RenWindow->draw(m_ButtonText);
}

void Button::OnClicked()
{
	Action();
}

void Button::Action()
{
}
