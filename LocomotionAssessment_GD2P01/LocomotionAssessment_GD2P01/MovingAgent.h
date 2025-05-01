#pragma once
#include "Actor.h"
class MovingAgent : public Actor
{
public:
	MovingAgent();
	~MovingAgent() {};

	void Update() override;
	void Render(sf::RenderWindow* _RenWindow) override;
protected:
	sf::CircleShape* m_Body;

	sf::Vector2f m_TargetPosition;

	float m_fCurrentSpeed;
	float m_fMaxSpeed;
	sf::Vector2f m_Velocity;

	void ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight);


};

