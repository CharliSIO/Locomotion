#pragma once
#include "Line.h"

constexpr float Deg2Rad = (3.1412f * 2.0f) / 360.0f;
constexpr float Rad2Deg = 360.0f/ (3.1412f * 2.0f);

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

	void SetSeek()
	{
		m_fSeekWeight = 1.0f;
		m_fArriveWeight = 1.0f;
		m_fFleeWeight = 0.0f;
		m_fWanderWeight = 0.0f;
	}

	void SetFlee()
	{
		m_fFleeWeight = 1.0f;
		m_fSeekWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fArriveWeight = 0.0f;
	}

	void SetWander()
	{
		m_fWanderWeight = 1.0f;
		m_fSeekWeight = 0.0f;
		m_fFleeWeight = 0.0f;
		m_fArriveWeight = 0.0f;
	}

	void SetSeparationWeight(float _val)
	{
		m_fSeparationWeight = _val;
	}

	void SetCohesionWeight(float _val)
	{
		m_fCohesionWeight = _val;
	}

	void SetAlignmentWeight(float _val)
	{
		m_fAlignmentWeight = _val;
	}

protected:
	sf::CircleShape* m_Body;

	sf::Vector2f m_TargetPosition{ 150, 200 };

	float m_fSpeed{ 10.0f };
	float m_fMaxSpeed{ 20.0f };
	float m_fMaxSteerForce{ 10.0f };
	sf::Vector2f m_Velocity{ 1.0f, 1.0f };

	void ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight);


	// LOCOMOTION BEHAVIOURS - INDIVIDUAL ---------
	// 
	// weighting for seek behaviour
	float m_fSeekWeight{ 0.0f };
	float m_fSeekStrength{ 1.0f };
	sf::Vector2f m_vSeekDesiredVelocity;

	float m_fFleeWeight{ 0.0f };
	float m_fFleeStrength{ 1.0f };
	sf::Vector2f m_vFleeDesiredVelocity;

	float m_fArriveWeight{ 0.0f };
	float m_fArrivalStrength{ 1.0f };
	sf::Vector2f m_vArriveDesiredVelocity;

	// -- WANDER
	float m_fWanderWeight{ 1.0f };
	float m_fWanderStrength{ 1.0f };
	sf::Vector2f m_vWanderDesiredVelocity;

	float m_WanderAdjustTimer{ 0.0f };
	float m_WanderAdjustInterval{ 2.0f };
	float m_WanderDist{ 200.0f };
	float m_WanderRadius{ 1.0f };
	sf::Vector2f m_CircleCentre{ 0.0f, 0.0f };
	sf::Vector2f m_CircumferenceTarget{ 0.0f, 0.0f };

	float m_WanderAngle{ 0.0f };
	float m_TargetWanderAngle{ 0.0f };
	float m_TargetAngleMultiplier{ 0.0f };
	float m_AngleLerpSpeed{ 50.0f };
	//

	// --------------------------

	// FLOCKING ------------------------------------

	// steering force (shared over behaviours)
	sf::Vector2f m_SteeringForce;
	float m_NeighbourRadius{ 100.0f };
	float m_ArriveRadius{ 70.0f };
	float m_BodyStrongSeparationRadius{ 40.0f };

	float m_fSeparationStrength{ 1.0f };
	float m_fSeparationWeight{ 1.0f };

	float m_fCohesionStrength{ 0.0f };
	float m_fCohesionWeight{ 0.0f };

	float m_fAlignmentStrength{ 0.0f };
	float m_fAlignmentWeight{ 0.0f };

	// --------------------------------------

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

