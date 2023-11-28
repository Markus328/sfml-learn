// #ifndef BORDER
// #define BORDER
#include "border.hpp"
#include "ball.hpp"
#include "collider.hpp"
#include "entity.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

Border::Border(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::string &name, World &world)
    : world(world), Collider(position, name, world) {
  this->shape->setFillColor(sf::Color(255, 0, 0));
  this->size = size;
  ((sf::RectangleShape *)this->shape.get())->setSize(size);
  updateBoundingBox();
}

void Border::instantaneousCollide(const Collider &obj, int side) {
  if (Ball::ball_count < 1000 && obj.getCategory() == "Ball") {
    const Ball &ball = obj.safe_castTo<Ball>();
    Ball &clone = ball.clone(world);
    // clone.move(500, 500);
    *(colliding.begin() + clone.getId()) = true;
  }
}
void Border::continuousCollide(const Collider &obj, int side) {}
const sf::Vector2f &Border::getSize() const { return this->size; }
