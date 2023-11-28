#include "ball.hpp"
#include "collider.hpp"
#include "collision.hpp"
#include "entity.hpp"
#include "soundpool.cpp"
#include "world.hpp"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <vector>

int Ball::ball_count = 0;

Ball &Ball::clone(World &world) const {
  // assert(Entity::getEntities()["Ball"]);
  // std::cout << "balllsss: " << numBalls << "\n";
  ball_count++;
  return *new Ball(*this, world);
};
Ball::Ball(const Ball &other, World &world)
    : Ball(other.getPosition(), other.getRadius(),
           other.getShape().getFillColor(), world) {
  this->setSpeed(other.getSpeed());
}
Ball::Ball(const sf::Vector2f &position, float radius, const sf::Color &color,
           World &world)
    : Collider(position, "Ball", world, new sf::CircleShape()) {

  sf::CircleShape *circleShape = (sf::CircleShape *)this->shape.get();
  circleShape->setRadius(radius);
  this->radius = radius;
  this->size = {radius, radius};
  circleShape->setFillColor(color);

  updateBoundingBox();
}

void Ball::instantaneousCollide(const Collider &obj, int side) {
  soundPool->play(Ball::bop);

  // std::cout << "angle: " << getAngle() << "\n";
  setAngle(getAngle() +
           ((rand() % 3) - 1) * ((rand() % 500) / 100.0 * M_PI / 180));
  setSpeed(getASpeed());
  // setSpeed(getASpeed() * 0.99);
  // std::cout << "!!!! " << this->getId() << ", " << obj.getId() << std::endl;

  // continuousCollide(obj, side);
}
void Ball::continuousCollide(const Collider &obj, int side) {

  auto speed = getSpeed();
  float speedx = speed.x;
  float speedy = speed.y;
  if (side == Collision::SIDE_TOP) {
    setSpeed(speedx, -std::abs(speedy));
  } else if (side == Collision::SIDE_LEFT) {
    setSpeed(std::abs(speedx), speedy);
  } else if (side == Collision::SIDE_BOTTOM) {
    setSpeed(speedx, std::abs(speedy));
  } else if (side == Collision::SIDE_RIGHT) {
    setSpeed(-std::abs(speedx), speedy);
  }
}

const sf::Vector2f &Ball::getSize() const { return this->size; }
// bool isAtBorder() {
//   // top
//   int col = SIDE_NONE;
//   bool colliding = false;
//   if ((col = collide(Border::getTop())) != SIDE_NONE) {
//     if (col != SIDE_TOP) {
//       this->active = false;
//       return false;
//     }
//     setSpeed(sf::Vector2f(this->speed.x, -std::abs(this->speed.y)));
//     colliding = true;
//   }
//   // left
//   if ((col = collide(Border::getLeft())) != SIDE_NONE) {
//     if (col != SIDE_LEFT) {
//       this->active = false;
//       return false;
//     }
//     setSpeed(sf::Vector2f(std::abs(this->speed.x), this->speed.y));
//     colliding = true;
//   }
//   // right
//   if ((col = collide(Border::getRight())) == SIDE_RIGHT) {
//     if (col != SIDE_RIGHT) {
//       this->active = false;
//       return false;
//     }
//     setSpeed(sf::Vector2f(-std::abs(this->speed.x), this->speed.y));
//     colliding = true;
//   }
//   // bottom
//   if ((col = collide(Border::getBottom())) == SIDE_BOTTOM) {
//     if (col != SIDE_BOTTOM) {
//       this->active = false;
//       return false;
//     }
//     setSpeed(sf::Vector2f(this->speed.x, std::abs(this->speed.y)));
//     colliding = true;
//   }
//   return colliding;
// }
void Ball::step(float elapsedTime) {
  Entity::step(elapsedTime);
  move(this->position.x + this->getSpeed().x * elapsedTime,
       this->position.y - this->getSpeed().y * elapsedTime);
}

float Ball::getRadius() const { return this->radius; }
