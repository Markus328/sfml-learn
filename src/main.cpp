
#include "world.hpp"
#include <SFML/System/Vector2.hpp>
#include "soundpool.cpp"
#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "ball.hpp"
#include "entity.hpp"

World world(sf::Vector2f(0, 0), sf::Vector2f(20000, 20000));

sf::String screenStr("Aperte ESC para sair");
bool requested = false;

sf::SoundBuffer Ball::bop;

Ball ball(sf::Vector2f(300, 300), 20, sf::Color(20, 80, 50), world);

int main() {
  SoundPool pool = SoundPool::getInstance();
  std::cout << "sizeof ball: " << sizeof(Ball);
  ball.setAngle(M_PI * 45 / 180);
  ball.setSpeed(200);
  world.commitNewEntities();
  sf::Clock clock;
  Ball::bop.loadFromFile("/home/markus/.config/util/sounds/voltest.wav");
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = sf::RenderWindow{
      {1440u, 900u}, "SFML Test", sf::Style::Default, settings};
  window.setFramerateLimit(90);
  while (window.isOpen()) {
    window.clear();
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::JoystickConnected:
        screenStr = "Joystick connected!";
      case sf::Event::KeyPressed:
        screenStr = "Pressed key " + std::to_string(event.key.code);
        {
          pool.play(Ball::bop);
          // std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        if (event.key.code == sf::Keyboard::Escape) {
          if (!requested) {
            requested = true;
            std::cout << "Request to close."
                      << "\n";
            screenStr = "Tem certeza que deseja sair? (aperte ESC de novo)";
          } else {
            window.close();
          }
        } else if (event.key.code == sf::Keyboard::Space) {
          world.commitNewEntities();
        }
        break;
      default:
        break;
      }
    }

    window.clear();
    sf::Font font;
    font.loadFromFile(
        std::filesystem::path("/home/markus/.local/share/fonts/opentype/"
                              "FantasqueSansMono-Regular.otf"));
    sf::Text txt(screenStr, font, 30);
    txt.setFillColor(sf::Color(255, 0, 0));
    window.draw(txt);

    float elapsedTime = clock.restart().asSeconds();
    for (Entity *e : world.getEntities()) {
      window.draw(e->getShape());
    }
    world.step(elapsedTime);
    window.display();
  }
}
