#include "ControlWindow.h"
#include "LocomotionManager.h"

ControlWindow::ControlWindow() : Window("Control Panel", sf::Vector2u(400.0f, 700.0f))
{
	CreateUI();
}

// load UI from tGUI txt file and set the functions
void ControlWindow::CreateUI()
{
	m_GUI.loadWidgetsFromFile("Resources/ControlWindow.txt");


	// get pointer to widget in gui builder, must have same name
	auto seekButton = m_GUI.get<tgui::Button>("seekButton");
	if (seekButton)
	{
		seekButton->onClick(&LocomotionManager::SetModeSeek, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto fleeButton = m_GUI.get<tgui::Button>("fleeButton");
	if (fleeButton)
	{
		fleeButton->onClick(&LocomotionManager::SetModeFlee, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto wanderButton = m_GUI.get<tgui::Button>("wanderButton");
	if (wanderButton)
	{
		wanderButton->onClick(&LocomotionManager::SetModeWander, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto pursuitButton = m_GUI.get<tgui::Button>("pursuitButton");
	if (pursuitButton)
	{
		pursuitButton->onClick(&LocomotionManager::SetModePursue, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto evadeButton = m_GUI.get<tgui::Button>("evadeButton");
	if (evadeButton)
	{
		evadeButton->onClick(&LocomotionManager::SetModeEvade, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto leaderFollowButton = m_GUI.get<tgui::Button>("leaderFollowButton");
	if (leaderFollowButton)
	{
		leaderFollowButton->onClick(&LocomotionManager::SetModeFollowLeader, LocomotionManager::GetWindowByName("Main Window"));
	}
	
	auto separationSlider = m_GUI.get<tgui::Slider>("separationSlider");
	if (separationSlider)
	{
		separationSlider->onValueChange(&LocomotionManager::SetSeparationWeight, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto cohesionSlider = m_GUI.get<tgui::Slider>("cohesionSlider");
	if (cohesionSlider)
	{
		cohesionSlider->onValueChange(&LocomotionManager::SetCohesionWeight, LocomotionManager::GetWindowByName("Main Window"));
	}
	auto alignmentSlider = m_GUI.get<tgui::Slider>("alignmentSlider");
	if (alignmentSlider)
	{
		alignmentSlider->onValueChange(&LocomotionManager::SetAlignmentWeight, LocomotionManager::GetWindowByName("Main Window"));
	}

	auto gizmoCheckBox = m_GUI.get<tgui::CheckBox>("gizmoCheckBox");
	if (gizmoCheckBox)
	{
		gizmoCheckBox->onCheck(&LocomotionManager::SetGizmosEnabled);
		gizmoCheckBox->onUncheck(&LocomotionManager::SetGizmosEnabled);
	}
}
