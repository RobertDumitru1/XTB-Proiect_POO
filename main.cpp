#include <SFML/Graphics.hpp>
#include <optional> // Necesar pentru noul sistem de evenimente
#include "nlohmann/json.hpp"

int main() {



    
    // În SFML 3, VideoMode are un constructor mai explicit
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pong SFML 3.0");
}


































// window.setFramerateLimit(60);
    //
    // // Mingea
    // sf::CircleShape ball(15.f);
    // ball.setFillColor(sf::Color::White);
    // ball.setOrigin({15.f, 15.f}); // Setăm centrul ca punct de rotație/poziție
    // ball.setPosition({400.f, 300.f});
    //
    // sf::RectangleShape paddle({100.f, 50.f});
    // paddle.setFillColor(sf::Color::Red);
    // paddle.setOrigin({50.f, 25.f});
    // paddle.setPosition({400.f, 550.f});
    //
    // sf::Vector2f velocity{4.f, 4.f};
    //
    // while (window.isOpen()) {
    //     // GESTIONARE EVENIMENTE (Stil SFML 3)
    //     while (const std::optional event = window.pollEvent()) {
    //         if (event->is<sf::Event::Closed>()) {
    //             window.close();
    //         }
    //     }
    //
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    //         paddle.move({-4.f, 0});
    //     }
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    //         paddle.move({4.f, 0});
    //     }
    //
    //     if (paddle.getGlobalBounds().findIntersection(ball.getGlobalBounds())) {
    //         // velocity.x *= -1.f;
    //         velocity.y *= -1.f;
    //     }
    //
    //     // UPDATE
    //     ball.move(velocity);
    //
    //     // Coliziuni cu pereții (Folosind Vector2f pentru margini)
    //     const sf::Vector2f pos = ball.getPosition();
    //     if (pos.x - 15.f < 0.f || pos.x + 15.f > 800.f) velocity.x *= -1.f;
    //     if (pos.y - 15.f < 0.f || pos.y + 15.f > 600.f) velocity.y *= -1.f;
    //
    //     // RENDER
    //
    //     window.clear(sf::Color::Black);
    //     window.draw(paddle);
    //     window.draw(ball);
    //     window.display();
    // }
