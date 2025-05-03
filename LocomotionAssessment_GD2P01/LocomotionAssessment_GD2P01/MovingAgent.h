#pragma once
#include "Line.h"
class MovingAgent : public Actor
{
public:
	MovingAgent();
	~MovingAgent() {};

	void Update() override;
	void Render(sf::RenderWindow* _RenWindow) override;
protected:
	sf::CircleShape* m_Body;

	sf::Vector2f m_TargetPosition{ 150, 200 };

	float m_fSpeed{ 10.0f };
	float m_fMaxSpeed{ 20.0f };
	float m_fMaxSteerForce{ 10.0f };
	sf::Vector2f m_Velocity{ 0.0f, 0.0f };

	void ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight);


	// LOCOMOTION BEHAVIOURS - INDIVIDUAL ---------
	float m_fSeekWeight{ 1.0f };
	float m_fSeekStrength{ 1.0f };
	sf::Vector2f m_vSeekDesiredVelocity;

	float m_fFleeWeight{ 0.0f };
	float m_fFleeStrength{ 1.0f };
	sf::Vector2f m_vFleeDesiredVelocity;

	float m_fWanderWeight{ 0.0f };
	float m_fWanderStrength{ 1.0f };
	sf::Vector2f m_vWanderDesiredVelocity;

	sf::Vector2f m_SteeringForce;

	void Seek();
	void Flee();
	void Wander();

	// -------------

	// GIZMOS --------------------------------
	float m_GizmoDrawLength{ 50.0f };
	Line* m_TargetPosGizmo;
	Line* m_VelocityGizmo;
	Line* m_SteerForceGizmo;

};

