#include "MovingAgent.h"
#include "LocomotionManager.h"

MovingAgent::MovingAgent()
{
	m_Body = new sf::CircleShape(30.0f, 3);
	m_Body->setOrigin(sf::Vector2f(30.0f, 30.0f));
	m_Body->setFillColor(sf::Color::Green);
	m_TargetPosGizmo = new Line(m_Body->getPosition(), m_TargetPosition, sf::Color::Blue);
	m_VelocityGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Red);
	m_SteerForceGizmo = new Line(m_Body->getPosition(), m_Body->getPosition() + m_Velocity * m_GizmoDrawLength, sf::Color::Green);
}

void MovingAgent::Update()
{
	m_TargetPosition = LocomotionManager::GetMousePos();
	Seek();

	m_Body->move(m_Velocity * m_fSpeed * LocomotionManager::DeltaTime());
	if (m_Velocity != sf::Vector2f(0.0f, 0.0f))
	{
		m_Body->setRotation(sf::Vector2f(0.0f, -1.0f).angleTo(m_Velocity));
	}

	m_TargetPosGizmo->Update(m_Body->getPosition(), m_TargetPosition);
	if (m_Velocity.lengthSquared() != 0.0f)
	{
		m_VelocityGizmo->Update(m_Body->getPosition(), m_Body->getPosition() + m_Velocity.normalized() * m_GizmoDrawLength);
		m_SteerForceGizmo->Update(m_Body->getPosition() + m_Velocity.normalized() * m_GizmoDrawLength, m_Body->getPosition() + m_SteeringForce.normalized() * m_GizmoDrawLength);
	}
}

void MovingAgent::Render(sf::RenderWindow* _RenWindow)
{
	_RenWindow->draw(*m_Body);
	m_TargetPosGizmo->Render(_RenWindow);
	m_VelocityGizmo->Render(_RenWindow);
	m_SteerForceGizmo->Render(_RenWindow);
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
