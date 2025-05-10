#include "ControlWindow.h"
#include "LocomotionManager.h"

ControlWindow::ControlWindow() : Window("Control Panel", sf::Vector2u(300.0f, 800.0f))
{
	CreateUI();
}

ControlWindow::~ControlWindow()
{
}

void ControlWindow::Update()
{
	
}

void ControlWindow::Render()
{
}

void ControlWindow::CreateUI()
{
	auto seekButton = tgui::Button::create("Seek");
	auto fleeButton = tgui::Button::create("Flee");
	auto wanderButton = tgui::Button::create("Wander");

	auto separationSlider = tgui::Slider::create(0.0f, 1.0f);
	auto cohesionSlider = tgui::Slider::create(0.0f, 1.0f);
	auto alignmentSlider = tgui::Slider::create(0.0f, 1.0f);

	separationSlider->setStep(0.1f);
	cohesionSlider->setStep(0.1f);
	alignmentSlider->setStep(0.1f);

	m_GUI.add(seekButton, "seekButton");
	m_GUI.add(fleeButton, "fleeButton");
	m_GUI.add(wanderButton, "wanderButton");

	m_GUI.add(separationSlider, "separationSlider");
	m_GUI.add(cohesionSlider, "cohesionSlider");
	m_GUI.add(alignmentSlider, "alignmentSlider");

	seekButton->setSize("50%", "10%"); seekButton->setOrigin(0.5f, 0.5f); seekButton->setPosition("30%", "10%");
	fleeButton->setSize("50%", "10%"); fleeButton->setOrigin(0.5f, 0.5f); fleeButton->setPosition("30%", "25%");
	wanderButton->setSize("50%", "10%"); wanderButton->setOrigin(0.5f, 0.5f); wanderButton->setPosition("30%", "40%");

	separationSlider->setSize("50%", "2%"); separationSlider->setOrigin(0.5f, 0.5f); separationSlider->setPosition("30%", "50%");
	cohesionSlider->setSize("50%", "2%"); cohesionSlider->setOrigin(0.5f, 0.5f); cohesionSlider->setPosition("30%", "55%");
	alignmentSlider->setSize("50%", "2%"); alignmentSlider->setOrigin(0.5f, 0.5f); alignmentSlider->setPosition("30%", "60%");

	seekButton->onClick(&LocomotionManager::SetModeSeek, LocomotionManager::GetWindowByName("Main Window"));
	fleeButton->onClick(&LocomotionManager::SetModeFlee, LocomotionManager::GetWindowByName("Main Window"));
	wanderButton->onClick(&LocomotionManager::SetModeWander, LocomotionManager::GetWindowByName("Main Window"));

	separationSlider->onValueChange(&LocomotionManager::SetSeparationWeight, LocomotionManager::GetWindowByName("Main Window"));
	cohesionSlider->onValueChange(&LocomotionManager::SetCohesionWeight, LocomotionManager::GetWindowByName("Main Window"));
	alignmentSlider->onValueChange(&LocomotionManager::SetAlignmentWeight, LocomotionManager::GetWindowByName("Main Window"));
}
