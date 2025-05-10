#include "Window.h"

Window::~Window()
{
	for (int i = 0; i < m_AttachedObjects.size(); i++)
	{
		delete m_AttachedObjects[i];
		m_AttachedObjects[i] = nullptr;
	}
}

void Window::Update()
{
	for (auto& obj : m_AttachedObjects)
	{
		obj->SetWindow(this);
		obj->Update();
		if (obj->GetBoundsSize() == sf::Vector2f(0.0f, 0.0f))
		{
			obj->SetBoundsSize(GetWindowSize());
		}
	}
}

void Window::Render()
{
	m_Window.clear(sf::Color::Black);
	for (auto &obj: m_AttachedObjects)
	{
		obj->Render(&m_Window);
	}
}
