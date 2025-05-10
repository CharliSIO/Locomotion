#pragma once
#include "Window.h"
class ControlWindow : public Window
{
public:
	ControlWindow();
	~ControlWindow();

	void Update();
	void Render();

protected:
	void CreateUI();
};

