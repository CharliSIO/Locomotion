#pragma once

#include <threads.h>
#include <mutex>
#include <vector>
#include "ControlWindow.h"
#include <SFML/System/Vector2.hpp>

class LocomotionManager
{
public:

	static LocomotionManager* getInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new LocomotionManager;
		}
		return m_pInstance;
	}

	void Start();

	void Update();
	void Render();

	static bool CreateWindow(std::string _WindowName, sf::Vector2u _Size);
	void CreateControlPanel();

	// Iterate through all windows and check if a window with certain name exists
	// If it does, return it, otherwise return nullptr
	// Can only have one window of each name active
	static Window* GetWindowByName(std::string _WindowName)
	{
		for (int i = 0; i < m_Windows.size(); i++)
		{
			if (_WindowName == m_Windows[i]->GetWindowName())
			{
				return m_Windows[i];
			}
		}
		return nullptr;
	}

	// Recreates the window of certain name
	inline void ResetWindow(std::string _WindowName)
	{
		GetWindowByName(_WindowName)->ReCreateWindow();
	}

	static bool CheckIfApplicationOpen()
	{
		return (!m_Windows.empty());
	}


	void CreateLocomotionAgent(sf::Vector2f _Position, std::string _InWindowName);

	static sf::Vector2f ClampVec2Magnitude(const sf::Vector2f& _vec, float _maxMag)
	{
		float vecMag = _vec.length();
		if (_vec.lengthSquared() > _maxMag * _maxMag)
		{
			return _vec.normalized() * _maxMag;
		}
		else return _vec;
	}
	static sf::Vector2f GetVec2Offset(const sf::Vector2f& _vecA, const sf::Vector2f& _vecB)
	{
		return sf::Vector2f(_vecA.x - _vecB.x, _vecA.y - _vecB.y);
	}

	static sf::Vector2f GetMousePos()
	{
		return m_MousePosWorld;
	}

	static auto GetActiveWindow()
	{
		return m_ActiveWindow;
	}
	static float DeltaTime()
	{
		return m_DeltaTime;
	}

	static bool MouseReleased;

	// change basic locomotion modes
	static void SetModeSeek(Window* _window)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetSeek();
				}
			}
		}
	}

	static void SetModeFlee(Window* _window)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetFlee();
				}
			}
		}
	}

	static void SetModeWander(Window* _window)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetWander();
				}
			}
		}
	}

	static void SetModePursue(Window* _window)
	{
		bool setFirstAgentSeek = false;
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					if (!setFirstAgentSeek)
					{
						setFirstAgentSeek = true;
						agent->SetSeek();
						m_PursueTargetAgent = agent;
					}
					else
					{
						agent->SetPursue();
					}
				}
			}
		}
	}
	static auto& GetPursueTarget()
	{
		return m_PursueTargetAgent;
	}

	static void SetSeparationWeight(Window* _window, float _newValue)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetSeparationWeight(_newValue);
				}
			}
		}
	}

	static void SetCohesionWeight(Window* _window, float _newValue)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetCohesionWeight(_newValue);
				}
			}
		}
	}

	static void SetAlignmentWeight(Window* _window, float _newValue)
	{
		if (_window != nullptr)
		{
			auto actors = _window->GetAttachedObjects();
			for (auto& a : *actors)
			{
				if (MovingAgent* agent = dynamic_cast<MovingAgent*>(a))
				{
					agent->SetAlignmentWeight(_newValue);
				}
			}
		}
	}

private:
	static bool m_InitialSetupComplete;
	static LocomotionManager* m_pInstance;
	static std::mutex m_Mutex;

	static std::vector<Window*> m_Windows;

	static sf::Clock GameClock;
	static float m_DeltaTime;

	static sf::Vector2f m_MousePosWorld;
	static sf::CircleShape* m_MouseGizmo;
	static Window* m_ActiveWindow;

	static MovingAgent* m_PursueTargetAgent;

	LocomotionManager(const LocomotionManager&) = delete;
	LocomotionManager() {};

	void UpdateMousePosWorld()
	{
		if (m_ActiveWindow != nullptr && GetWindowByName("Main Window") != nullptr)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*m_ActiveWindow->GetWindow());
			m_MousePosWorld = GetWindowByName("Main Window")->GetWindow()->mapPixelToCoords(mousePos);
		}
		else
		{
			m_MousePosWorld = (sf::Vector2f)sf::Mouse::getPosition(*m_Windows[0]->GetWindow());
		}
	}
};


