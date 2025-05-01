#pragma once

#include <threads.h>
#include <mutex>
#include <vector>
#include "Window.h"

class LocomotionManager
{
public:
	LocomotionManager(const LocomotionManager& _Object) = delete;

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

	bool CreateWindow(std::string _WindowName);

	// Iterate through all windows and check if a window with certain name exists
	// If it does, return it, otherwise return nullptr
	// Can only have one window of each name active
	Window* GetWindowByName(std::string _WindowName)
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

	void CreateLocomotionAgent(std::string _InWindowName);

	static float Vec2Magnitude(sf::Vector2f _vector)
	{
		return std::sqrtf(std::powf(_vector.x, 2) + std::powf(_vector.y, 2));
	}
	static sf::Vector2f NormaliseFVector(sf::Vector2f _vector)
	{
		float magnitude = Vec2Magnitude(_vector);
		return sf::Vector2f(_vector.x / magnitude, _vector.y / magnitude);
	}

	static sf::Vector2f ClampVec2Magnitude(sf::Vector2f _vec, float _maxMag)
	{
		
	}

private:
	static bool m_InitialSetupComplete;
	static LocomotionManager* m_pInstance;
	static std::mutex m_Mutex;

	static std::vector<Window*> m_Windows;

	LocomotionManager() {};
};


