#include "Window.h"

int main()
{
    Window MainWindow("Main Window");
    sf::CircleShape shape(200.f, 100);
    shape.setFillColor(sf::Color(204, 77, 5)); // Color circle
    shape.setPosition(200, 200); // Center circle

    while (MainWindow.GetWindow()->isOpen())
    {
        sf::Event event;
        while (MainWindow.GetWindow()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                MainWindow.GetWindow()->close();
        }

        MainWindow.GetWindow()->clear(sf::Color(18, 33, 43)); // Color background
        MainWindow.GetWindow()->draw(shape);
        MainWindow.GetWindow()->display();
    }

    return 0;
}