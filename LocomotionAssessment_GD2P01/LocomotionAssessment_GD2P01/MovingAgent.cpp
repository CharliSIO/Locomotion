#include "MovingAgent.h"
#include "LocomotionManager.h"
#include "Window.h"
#include <cstdlib>
#include <math.h>
#include <cmath>

MovingAgent::MovingAgent(sf::Vector2f _Position)
{
	std::srand(time(0));

	m_Body = new sf::CircleShape(15.0f, 3);
	m_Body->setOrigin(sf::Vector2f(15.0f, 15.0f));
	m_Body->setPosition(_Position);
	m_Body->setFillColor(sf::Color::Green);
	m_TargetPosGizmo = new Line(m_Body->getPosition(), m_TargetPosition, sf::Color::Blue);
	m_VelocityGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Red);
	m_SteerForceGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Green);
	m_NeighbourGizmo.setFillColor(sf::Color::Transparent);
	m_SightProjection.setFillColor(sf::Color::Transparent);
	m_NeighbourGizmo.setOutlineThickness(2.0f);
	m_SightProjection.setOutlineThickness(2.0f);
	m_NeighbourGizmo.setOrigin(sf::Vector2f(m_NeighbourGizmo.getRadius(), m_NeighbourGizmo.getRadius()));
	m_SightProjection.setOrigin(sf::Vector2f(m_SightProjection.getRadius(), m_SightProjection.getRadius()));
}

void MovingAgent::Update()
{
	m_TargetPosition = LocomotionManager::GetMousePos();
	
	FollowLeader(); // leader following first, to update target pos if following a leader

	if ((m_TargetPosition - m_Body->getPosition()).length() >= 10.0f) // don't run if too close
	{
		Seek();
		Flee();
		Pursuit();
		Evade();
	}
	Wander();
	Arrive();
	ManageFlocking();


	m_Body->move(m_Velocity * m_fSpeed * LocomotionManager::DeltaTime()); 

	// change rotation only for large enough velocity changes 
	// otherwise changes very slightly with small changes with confliction steering forces
	if (m_Velocity.length() > 5.0f && (m_TargetPosition - m_Body->getPosition()).length() >= 10.0f) 
	{
		m_Body->setRotation(sf::Vector2f(0.0f, -1.0f).angleTo(m_Velocity));
	}

	StayInBounds();

	// UPDATE GIZMOS	
	m_TargetPosGizmo->Update(m_Body->getPosition(), m_TargetPosition);
	if (m_Velocity.lengthSquared() != 0.0f && m_SteeringForce.lengthSquared() != 0.0f)
	{
		m_VelocityGizmo->Update(m_Body->getPosition(), m_Body->getPosition() + m_Velocity.normalized() * m_GizmoDrawLength);
		m_SteerForceGizmo->Update(m_Body->getPosition() + m_Velocity.normalized() * m_GizmoDrawLength, m_Body->getPosition() + m_SteeringForce.normalized() * m_GizmoDrawLength);
	}
	m_NeighbourGizmo.setPosition(m_Body->getPosition());
	m_SightProjection.setPosition(m_Body->getPosition() + (m_Velocity.normalized() * m_ProjectionDistance));
}

// draw self and gizmos to window buffer
void MovingAgent::Render(sf::RenderWindow* _RenWindow)
{
	_RenWindow->draw(*m_Body); // render self

	if (LocomotionManager::GetGizmosEnabled()) // render gizmos
	{
		m_TargetPosGizmo->Render(_RenWindow);
		m_VelocityGizmo->Render(_RenWindow);
		m_SteerForceGizmo->Render(_RenWindow);
		_RenWindow->draw(m_NeighbourGizmo);
		_RenWindow->draw(m_SightProjection);
	}
}

// apply steering force and clamp it to maximum force. Apply it to the velocity, clamp to max velocity
void MovingAgent::ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight)
{
	m_SteeringForce = _DesiredVelocity - m_Velocity;
	m_SteeringForce = LocomotionManager::ClampVec2Magnitude(m_SteeringForce, _MaxForce);

	m_Velocity += m_SteeringForce * _ForceStrength * _ForceWeight * LocomotionManager::DeltaTime();
	m_Velocity = LocomotionManager::ClampVec2Magnitude(m_Velocity, m_fMaxSpeed);
}

void MovingAgent::Seek()
{
	sf::Vector2f vecToTarget = m_TargetPosition - m_Body->getPosition();
	m_vSeekDesiredVelocity = vecToTarget.normalized() * m_fMaxSpeed;

	ApplySteeringForce(m_vSeekDesiredVelocity, m_fMaxSteerForce, m_fSeekStrength, m_fSeekWeight);
}

void MovingAgent::Flee()
{
	m_vFleeDesiredVelocity = -m_vSeekDesiredVelocity; // opposite of seek desired velocity

	ApplySteeringForce(m_vFleeDesiredVelocity, m_fMaxSteerForce, m_fFleeStrength, m_fFleeWeight);
}

// add random angle to wander desired velocity to create random smooth movement
void MovingAgent::Wander()
{
	m_TargetWanderAngle += sf::degrees((std::rand() % 3) - 1) * (std::rand() % 20);

	m_CircleCentre = m_Body->getPosition() + (m_Velocity.normalized() * m_WanderDist);
	m_WanderTarget = (m_CircleCentre + m_Velocity.normalized() * m_WanderRadius).rotatedBy(m_TargetWanderAngle);
	m_WanderTarget = m_WanderTarget.normalized() * m_fMaxSpeed;
	ApplySteeringForce(m_WanderTarget, m_fMaxSteerForce, m_fWanderStrength, m_fWanderWeight);
}

void MovingAgent::Pursuit()
{
	if (LocomotionManager::GetPursueTarget() != nullptr && LocomotionManager::GetPursueTarget() != this)
	{
		m_vTargetVelocity = LocomotionManager::GetPursueTarget()->m_Velocity;
		m_TargetPosition = LocomotionManager::GetPursueTarget()->GetPosition();

		sf::Vector2f predictedTargetPos = m_TargetPosition + (m_vTargetVelocity.normalized() * (m_fMaxSpeed * 10));

		sf::Vector2f vecToTarget = predictedTargetPos - m_Body->getPosition();

		m_vPursuitDesiredVelocity = vecToTarget.normalized() * m_fMaxSpeed;
		ApplySteeringForce(m_vPursuitDesiredVelocity, m_fMaxSteerForce, m_fPursuitStrength, m_fPursuitWeight);
	}
}

void MovingAgent::Evade()
{
	if (LocomotionManager::GetPursueTarget() != nullptr && LocomotionManager::GetPursueTarget() != this)
	{
		m_vTargetVelocity = LocomotionManager::GetPursueTarget()->m_Velocity;
		m_TargetPosition = LocomotionManager::GetPursueTarget()->GetPosition();

		sf::Vector2f predictedTargetPos = m_TargetPosition + (m_vTargetVelocity.normalized() * (m_fMaxSpeed * 10));

		sf::Vector2f vecToTarget = predictedTargetPos - m_Body->getPosition();

		m_vEvadeDesiredVelocity = -vecToTarget.normalized() * m_fMaxSpeed;
		ApplySteeringForce(m_vEvadeDesiredVelocity, m_fMaxSteerForce, m_fEvadeStrength, m_fEvadeWeight);
	}
}

// slow down when close to target, within arrival radius
void MovingAgent::Arrive()
{
	m_vArriveDesiredVelocity = m_TargetPosition - m_Body->getPosition();
	float distance = m_vArriveDesiredVelocity.length();
	if ( distance < m_ArriveRadius && distance > 0.0f)
	{
		m_vArriveDesiredVelocity = m_vArriveDesiredVelocity.normalized() * m_fMaxSpeed * (distance / m_ArriveRadius);
		ApplySteeringForce(m_vArriveDesiredVelocity, m_fMaxSteerForce, m_fArrivalStrength, m_fArriveWeight);
	}
}

// seek to point behind leader, if in front of leader move quickly away at 90 degrees to leader velocity
void MovingAgent::FollowLeader()
{
	if (LocomotionManager::GetPursueTarget() != nullptr && LocomotionManager::GetPursueTarget() != this)
	{
		m_vTargetVelocity = LocomotionManager::GetPursueTarget()->m_Velocity;
		m_TargetPosition = LocomotionManager::GetPursueTarget()->GetPosition() - (m_vTargetVelocity.normalized() * 30.0f);
		auto evasionCircle = LocomotionManager::GetPursueTarget()->GetSightCircle();

		if ((evasionCircle.getPosition() - m_Body->getPosition()).lengthSquared() <= (evasionCircle.getRadius() * evasionCircle.getRadius()))
		{
			// FLEE AWAY FROM IN CIRCLE
			sf::Vector2f vecToTarget = evasionCircle.getPosition() - m_Body->getPosition();
			sf::Vector2f rightAngleDir = m_vTargetVelocity.perpendicular();

			float dirMult = vecToTarget.dot(rightAngleDir) > 0.0f ? -1.0f : 1.0f; // check which direction to flee in

			m_SteeringForce = rightAngleDir * dirMult; 

			m_Velocity += m_SteeringForce * m_fMaxSteerForce * m_LeaderFollowWeight * LocomotionManager::DeltaTime();
			m_Velocity = LocomotionManager::ClampVec2Magnitude(m_Velocity, m_fMaxSpeed); // 90 deg is the steering force so skip SteeringForce function
		}
	}
	else if (LocomotionManager::GetPursueTarget() == this)
	{
		m_fSeparationWeight = 0.0f;
		m_fCohesionWeight = 0.0f;
		m_fAlignmentWeight = 0.0f;
	}
}

// manage flocking algorithms - separation, cohesion, alignment
void MovingAgent::ManageFlocking()
{
	sf::Vector2f selfPos = m_Body->getPosition();
	sf::Vector2f avgSeparation(0.0f, 0.0f);
	sf::Vector2f avgCohesionPos(0.0f, 0.0f);
	sf::Vector2f avgVelocity(0.0f, 0.0f);
	int iSCount = 0;
	int iCCount = 0;
	int iACount = 0;

	// strong separation for near to body
	sf::Vector2f avgSeparationStrong(0.0f, 0.0f);
	int iSStrongCount = 0;

	auto allActors = m_Window->GetAttachedObjects();
	std::vector<MovingAgent*> otherAgents;

	for (auto& a : *allActors)
	{
		if (a != this)
		{
			// is the actor a moving agent for locomotion calculations?
			if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
			{
				otherAgents.push_back(agent);
				auto posDifference = selfPos - agent->GetPosition();
				float distanceLength = posDifference.length();

				if (distanceLength <= m_NeighbourRadius) // is in neighbour rad
				{
					// Separation
					if (distanceLength > 0.0f)
					{
						auto direction = posDifference.normalized();
						direction = direction / distanceLength;
						avgSeparation += direction;
						iSCount++;
					}

					// Cohesion
					avgCohesionPos += agent->GetPosition();
					iCCount++;
				}
				if (distanceLength <= m_NeighbourRadius + 100) // is in neighbour rad
				{
					// Alignment
					auto otherVelocity = agent->m_Velocity;
					avgVelocity += otherVelocity;
					iACount++;
				}
				if (distanceLength <= m_BodyStrongSeparationRadius) // is in small neighbour rad
				{
					auto direction = posDifference.normalized();
					direction = direction / distanceLength;
					avgSeparationStrong += direction;
					iSStrongCount++;
				}
			}
		}
	}
	
	// Apply forces for each flocking function
	if (iSCount > 0 && avgSeparation.lengthSquared() != 0.0f)
	{
		avgSeparation /= (float)iSCount;
		avgSeparation = avgSeparation.normalized() * m_fMaxSpeed;
		ApplySteeringForce(avgSeparation, m_fMaxSteerForce, m_fSeparationStrength, m_fSeparationWeight);
	}
	if (iCCount > 0 && ((avgCohesionPos / (float)iCCount) - selfPos).lengthSquared() != 0.0f)
	{
		auto cohesionCentre = avgCohesionPos / (float)iCCount;
		auto desiredVel = (cohesionCentre - selfPos).normalized() * m_fMaxSpeed;

		ApplySteeringForce(desiredVel, m_fMaxSteerForce, m_fCohesionStrength, m_fCohesionWeight);
	}
	if (iACount > 0 && avgVelocity.lengthSquared() != 0.0f)
	{
		avgVelocity /= (float)iACount;
		avgVelocity = avgVelocity.normalized() * m_fMaxSpeed;
		ApplySteeringForce(avgVelocity, m_fMaxSteerForce, m_fAlignmentWeight, m_fAlignmentWeight);
	}

	// to ensure agents do not overlay in same spot even with cohesion/seek etc
	if (iSStrongCount > 0 && avgSeparationStrong.lengthSquared() != 0.0f)
	{
		avgSeparationStrong /= (float)iSStrongCount;
		avgSeparationStrong = avgSeparationStrong.normalized() * (m_fMaxSpeed);
		ApplySteeringForce(avgSeparationStrong, m_fMaxSteerForce, m_fSeparationStrength , 1.0f);
	}
}

// move between sides of the screen if out of bounds (wrap around)
void MovingAgent::StayInBounds()
{
	if (!m_Bounds.contains(m_Body->getPosition()))
	{
		if (m_Body->getPosition().x >= m_Bounds.size.x)
		{
			m_Body->move(sf::Vector2f(-m_Bounds.size.x, 0.0f));
		}
		else if (m_Body->getPosition().x <= 0.0f)
		{
			m_Body->move(sf::Vector2f(m_Bounds.size.x, 0.0f));
		}

		if (m_Body->getPosition().y >= m_Bounds.size.y)
		{
			m_Body->move(sf::Vector2f(0.0f, -m_Bounds.size.y));
		}
		else if (m_Body->getPosition().y <= 0.0f)
		{
			m_Body->move(sf::Vector2f(0.0f, m_Bounds.size.y));
		}
	}
}
