#include "MovingAgent.h"
#include "LocomotionManager.h"
#include "Window.h"

MovingAgent::MovingAgent(sf::Vector2f _Position)
{
	m_Body = new sf::CircleShape(30.0f, 3);
	m_Body->setOrigin(sf::Vector2f(30.0f, 30.0f));
	m_Body->setPosition(_Position);
	m_Body->setFillColor(sf::Color::Green);
	m_TargetPosGizmo = new Line(m_Body->getPosition(), m_TargetPosition, sf::Color::Blue);
	m_VelocityGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Red);
	m_SteerForceGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Green);
	m_NeighbourGizmo.setFillColor(sf::Color::Transparent);
	m_NeighbourGizmo.setOutlineThickness(2.0f);
	m_NeighbourGizmo.setOrigin(sf::Vector2f(m_NeighbourGizmo.getRadius(), m_NeighbourGizmo.getRadius()));
}

void MovingAgent::Update()
{
	m_TargetPosition = LocomotionManager::GetMousePos();

	if ((m_TargetPosition - m_Body->getPosition()).length() >= 50.0f)
	{
		Seek();
		Flee();
	}
	Arrive();
	ManageFlocking();

	m_Body->move(m_Velocity * m_fSpeed * LocomotionManager::DeltaTime());
	if (m_Velocity.length() > 0.0f && (m_TargetPosition - m_Body->getPosition()).length() >= 10.0f)
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
	m_NeighbourGizmo.setPosition(m_TargetPosition);
}

void MovingAgent::Render(sf::RenderWindow* _RenWindow)
{
	_RenWindow->draw(*m_Body);
	m_TargetPosGizmo->Render(_RenWindow);
	m_VelocityGizmo->Render(_RenWindow);
	m_SteerForceGizmo->Render(_RenWindow);
	_RenWindow->draw(m_NeighbourGizmo);
}

void MovingAgent::ApplySteeringForce(sf::Vector2f _DesiredVelocity, float _MaxForce, float _ForceStrength, float _ForceWeight)
{
	m_SteeringForce = _DesiredVelocity - m_Velocity;
	m_SteeringForce = LocomotionManager::ClampVec2Magnitude(m_SteeringForce, _MaxForce);

	m_Velocity += m_SteeringForce * _ForceStrength * _ForceWeight * LocomotionManager::DeltaTime();
	m_Velocity = LocomotionManager::ClampVec2Magnitude(m_Velocity, m_fMaxSpeed);
}

void MovingAgent::Seek()
{
	sf::Vector2f vecToTarget = LocomotionManager::GetVec2Offset(m_TargetPosition, m_Body->getPosition());
	m_vSeekDesiredVelocity = vecToTarget.normalized() * m_fMaxSpeed;

	ApplySteeringForce(m_vSeekDesiredVelocity, m_fMaxSteerForce, m_fSeekStrength, m_fSeekWeight);
}

void MovingAgent::Flee()
{
	m_vFleeDesiredVelocity = -m_vSeekDesiredVelocity; // opposite of seek desired velocity

	ApplySteeringForce(m_vFleeDesiredVelocity, m_fMaxSteerForce, m_fFleeStrength, m_fFleeWeight);
}

void MovingAgent::Arrive()
{
	m_vArriveDesiredVelocity = m_TargetPosition - m_Body->getPosition();
	float distance = m_vArriveDesiredVelocity.length();
	if ( distance < m_ArriveRadius)
	{
		m_vArriveDesiredVelocity = m_vArriveDesiredVelocity.normalized() * m_fMaxSpeed * (distance / m_ArriveRadius);
	}
	else
	{
		m_vArriveDesiredVelocity = m_vArriveDesiredVelocity.normalized() * m_fMaxSpeed;
	}
	ApplySteeringForce(m_vArriveDesiredVelocity, m_fMaxSteerForce, m_fArrivalStrength, 1.0f);
}

void MovingAgent::ManageFlocking()
{
	sf::Vector2f selfPos = m_Body->getPosition();
	sf::Vector2f avgSeparation(0.0f, 0.0f);
	sf::Vector2f avgCohesionPos(0.0f, 0.0f);
	sf::Vector2f avgVelocity(0.0f, 0.0f);
	int iSCount = 0;
	int iCCount = 0;
	int iACount = 0;

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

					// Alignment
					auto otherVelocity = agent->m_Velocity;
					avgVelocity += otherVelocity;
					iACount++;
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

}

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
