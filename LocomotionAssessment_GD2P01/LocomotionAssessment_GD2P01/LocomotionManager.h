#pragma once

#include <threads.h>
#include <mutex>
#include <iostream>
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
		if (m_Windows.empty()) return false;
		return m_Windows[0]->GetWindow()->isOpen();
	}

private:

	static LocomotionManager* m_pInstance;
	static std::mutex m_Mutex;

	static std::vector<Window*> m_Windows;

	LocomotionManager() {};
};


