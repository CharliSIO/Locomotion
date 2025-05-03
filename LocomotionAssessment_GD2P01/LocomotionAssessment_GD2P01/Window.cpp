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
		obj->Update();
	}
}

void Window::Render()
{
	m_Window.clear(sf::Color::Yellow);
	for (auto &obj: m_AttachedObjects)
	{
		obj->Render(&m_Window);
	}
}
