#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 1000u}), "car_sim");
    window.setFramerateLimit(60);
    

    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(sf::Vector2f(400.f, 800.f));

    float x, y, vx, vy, a;
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.draw(circle);
        window.display();
        printf("velocity vector: ");
        scanf("%f %f", &vx, &vy);
        printf("position (400 800 for default): ");
        scanf("%f %f", &x, &y);
        printf("acceleration");
        scanf("%f", &a);

        circle.setPosition(sf::Vector2(x,y));
        sf::Vector2f pos = circle.getPosition();
        sf::Vector2f velocity = sf::Vector2f(vx, vy);

        circle.setPosition(pos+=velocity);
        

    }
}
