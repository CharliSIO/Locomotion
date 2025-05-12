#pragma once
#include "Window.h"

// Class for the control panel window to change agent behaviour
class ControlWindow : public Window
{
public:
	ControlWindow();
	~ControlWindow() {};

	void Update() {};
	void Render() {};

protected:
	void CreateUI();
};

