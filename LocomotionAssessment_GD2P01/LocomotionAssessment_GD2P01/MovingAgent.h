#pragma once
#include "Line.h"
class MovingAgent : public Actor
{
public:
	MovingAgent(sf::Vector2f _Position);
	~MovingAgent() {};

	void Update() override;
	void Render(sf::RenderWindow* _RenWindow) override;

	auto GetPosition()
	{
		return m_Body->getPosition();
	}
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

	sf::Vector2f m_vArriveDesiredVelocity;
	float m_fArrivalStrength{ 2.5f };

	sf::Vector2f m_SteeringForce;
	float m_NeighbourRadius{ 100.0f };
	float m_ArriveRadius{ 70.0f };

	float m_fSeparationStrength{ 2.5f };
	float m_fSeparationWeight{ 1.0f };

	float m_fCohesionStrength{ 0.0f };
	float m_fCohesionWeight{ 0.0f };

	float m_fAlignmentStrength{ 1.0f };
	float m_fAlignmentWeight{ 0.0f };

	void Seek();
	void Flee();
	void Wander();
	void Pursuit();
	void Evade();
	void Arrive();

	void ManageFlocking();

	void StayInBounds();

	// -------------

	// GIZMOS --------------------------------
	float m_GizmoDrawLength{ 50.0f };
	Line* m_TargetPosGizmo;
	Line* m_VelocityGizmo;
	Line* m_SteerForceGizmo;
	sf::CircleShape m_NeighbourGizmo = sf::CircleShape(m_NeighbourRadius, 10.0f );
	//sf::CircleShape m_ArrivalGizmo;
};

