#include <SFML/Graphics.hpp>
int main()
{
    auto window = sf::RenderWindow{ { 800u, 600u }, "SFML Test" };
    window.setFramerateLimit(10);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
