#pragma once
#include "Line.h"

constexpr float Deg2Rad = (3.1412f * 2.0f) / 360.0f;
constexpr float Rad2Deg = 360.0f/ (3.1412f * 2.0f);

// Moving agent that exhibits locomotion behaviours
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

	// SET FORCE WEIGHTINGS FOR DIFFERENT MODES

	void SetSeek()
	{
		m_Body->setFillColor(sf::Color::Cyan);
		m_fSeekWeight = 1.0f;
		m_fArriveWeight = 1.0f;
		m_fFleeWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fPursuitWeight = 0.0f;
		m_fEvadeWeight = 0.0f;
		m_LeaderFollowWeight = 0.0f;
	}

	void SetFlee()
	{
		m_Body->setFillColor(sf::Color::Blue);
		m_fFleeWeight = 1.0f;
		m_fSeekWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fArriveWeight = 0.0f;
		m_fPursuitWeight = 0.0f;
		m_fEvadeWeight = 0.0f;
		m_LeaderFollowWeight = 0.0f;
	}

	void SetWander()
	{
		m_Body->setFillColor(sf::Color::Green);
		m_fWanderWeight = 1.0f;
		m_fSeekWeight = 0.0f;
		m_fFleeWeight = 0.0f;
		m_fArriveWeight = 0.0f;
		m_fPursuitWeight = 0.0f;
		m_fEvadeWeight = 0.0f;
		m_LeaderFollowWeight = 0.0f;
	}

	void SetPursue()
	{
		m_Body->setFillColor(sf::Color::Yellow);
		m_fSeekWeight = 0.0f;
		m_fArriveWeight = 1.0f;
		m_fFleeWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fPursuitWeight = 1.0f;
		m_fEvadeWeight = 0.0f;
		m_LeaderFollowWeight = 0.0f;
	}

	void SetEvade()
	{
		m_Body->setFillColor(sf::Color::Red);
		m_fSeekWeight = 0.0f;
		m_fArriveWeight = -1.0f;
		m_fFleeWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fPursuitWeight = 0.0f;
		m_fEvadeWeight = 1.0f;
		m_LeaderFollowWeight = 0.0f;
	}

	void SetLeaderFollow()
	{
		m_Body->setFillColor(sf::Color::Magenta);
		m_fSeekWeight = 1.0f;
		m_fArriveWeight = 2.0f;
		m_fFleeWeight = 0.0f;
		m_fWanderWeight = 0.0f;
		m_fPursuitWeight = 0.0f;
		m_fEvadeWeight = 0.0f;
		m_LeaderFollowWeight = 1.0f;
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

	auto GetSightCircle()
	{
		return m_SightProjection;
	}

protected:
	sf::CircleShape* m_Body;

	sf::Vector2f m_TargetPosition{ 150, 200 };

	float m_fSpeed{ 15.0f };
	float m_fMaxSpeed{ 25.0f };
	float m_fMaxSteerForce{ 15.0f };
	sf::Vector2f m_Velocity{ 1.0f, 1.0f };

	void ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight);


	// LOCOMOTION BEHAVIOURS - INDIVIDUAL ---------
	
	// weighting for seek behaviour
	float m_fSeekWeight{ 0.0f };
	float m_fSeekStrength{ 1.0f };
	sf::Vector2f m_vSeekDesiredVelocity;

	float m_fFleeWeight{ 0.0f };
	float m_fFleeStrength{ 1.0f };
	sf::Vector2f m_vFleeDesiredVelocity;

	float m_fPursuitWeight{ 0.0f };
	float m_fPursuitStrength{ 1.5f };
	sf::Vector2f m_vPursuitDesiredVelocity;
	sf::Vector2f m_vTargetVelocity;

	float m_fEvadeWeight{ 0.0f };
	float m_fEvadeStrength{ 1.0f };
	sf::Vector2f m_vEvadeDesiredVelocity;

	float m_fArriveWeight{ 0.0f };
	float m_fArrivalStrength{ 1.0f };
	sf::Vector2f m_vArriveDesiredVelocity;

	// -- WANDER
	float m_fWanderWeight{ 1.0f };
	float m_fWanderStrength{ 1.0f };
	sf::Vector2f m_vWanderDesiredVelocity;

	float m_WanderDist{ 20.0f };
	float m_WanderRadius{ 60.0f };
	sf::Angle m_TargetWanderAngle{ sf::degrees(0.0f) };
	sf::Vector2f m_CircleCentre{ 0.0f, 0.0f };
	sf::Vector2f m_WanderTarget{ 0.0f, 0.0f };
	//
	bool m_bTargetMouse = true;

	sf::CircleShape m_SightProjection{ 50.0f, 10 };
	float m_ProjectionDistance{ 40.0f };
	float m_LeaderFollowWeight{ 0.0f };
	// --------------------------

	// FLOCKING ------------------------------------

	// steering force (shared over behaviours)
	sf::Vector2f m_SteeringForce;
	float m_NeighbourRadius{ 50.0f };
	float m_ArriveRadius{ 40.0f };
	float m_BodyStrongSeparationRadius{ 20.0f };

	float m_fSeparationStrength{ 1.0f };
	float m_fSeparationWeight{ 0.0f };

	float m_fCohesionStrength{ 1.0f };
	float m_fCohesionWeight{ 0.0f };

	float m_fAlignmentStrength{ 1.0f };
	float m_fAlignmentWeight{ 0.0f };

	// --------------------------------------

	void Seek();
	void Flee();
	void Wander();
	void Pursuit();
	void Evade();
	void Arrive();

	void FollowLeader();

	void ManageFlocking();

	void StayInBounds();

	// -------------

	// GIZMOS --------------------------------
	float m_GizmoDrawLength{ 50.0f };
	Line* m_TargetPosGizmo;
	Line* m_VelocityGizmo;
	Line* m_SteerForceGizmo;
	sf::CircleShape m_NeighbourGizmo = sf::CircleShape(m_NeighbourRadius, 10.0f );
};

