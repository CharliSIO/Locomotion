#include "MovingAgent.h"
#include "LocomotionManager.h"

MovingAgent::MovingAgent()
{
	m_Body = new sf::CircleShape(30.0f, 3);
	m_Body->setFillColor(sf::Color::Green);
}

void MovingAgent::Update()
{
}

void MovingAgent::Render(sf::RenderWindow* _RenWindow)
{
	_RenWindow->draw(*m_Body);
}

void MovingAgent::ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight)
{
	sf::Vector2f steeringForce(_DesiredVelocity - m_Velocity);
	sf::Vector2f steeringForceNormalised = LocomotionManager::NormaliseFVector(steeringForce);
}
