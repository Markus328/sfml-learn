#include "border.hpp"
#include "ball.hpp"
#include "collider.hpp"
#include "entity.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

Border::Border(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::string &name, World &world)
    : Entity(name, world), RectangleEntity(position, size), world(world) {
  getShape().setFillColor(sf::Color(200, 40, 40));
  updateBoundingBox();
  setup();
}

void Border::instantaneousCollide(const Collider &obj, int side) {
  if (Ball::ball_count < 1000 && obj.getCategory() == "Ball") {
    const Ball &ball = obj.safe_castTo<Ball>();
    Ball &clone = ball.clone(world);
    // clone.setup();
    clone.setPosition(500, 500);
    *(colliding.begin() + clone.getId()) = true;
  }
}
void Border::continuousCollide(const Collider &obj, int side) {}
// const sf::Vector2f &Border::getSize() const { return this->size; }
