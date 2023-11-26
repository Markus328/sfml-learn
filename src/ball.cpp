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

Ball *Ball::clone(World &world) {
  // assert(Entity::getEntities()["Ball"]);
  int numBalls = world.getEntities<Ball>().size();
  // std::cout << "balllsss: " << numBalls << "\n";
  if (numBalls >= 1000) {
    return nullptr;
  }

  return new Ball(*this, world);
};
Ball::Ball(const Ball &other, World &world)
    : Ball(other.getPosition(), other.getRadius(),
           other.getShape().getFillColor(), world) {
  this->setSpeed(other.getSpeed());
}
Ball::Ball(sf::Vector2f position, float radius, sf::Color color, World &world)
    : Collider(position, sf::Vector2f(radius, radius), "Ball",
               new sf::CircleShape(), world) {

  sf::CircleShape *circleShape = (sf::CircleShape *)this->shape.get();

  circleShape->setRadius(radius);
  this->radius = radius;
  circleShape->setFillColor(color);

  updateBoundingBox();
}

void Ball::instantaneousCollide(const Collider &obj, CollisionSide side) {
  soundPool->play(Ball::bop);

  // std::cout << "angle: " << getAngle() << "\n";
  setAngle(getAngle() + ((rand() % 3) - 1) * (5 * M_PI / 180));
  // setSpeed(getASpeed() * 0.99);
  // std::cout << "!!!! " << this->getId() << ", " << obj.getId() << std::endl;

  continuousCollide(obj, side);
}
void Ball::continuousCollide(const Collider &obj, CollisionSide side) {

  float speedx = getSpeed().x, speedy = getSpeed().y;
  if (side == Collision::SIDE_TOP) {
    setSpeed(sf::Vector2f(speedx, -std::abs(speedy)));
  } else if (side == Collision::SIDE_LEFT) {
    setSpeed(sf::Vector2f(std::abs(speedx), speedy));
  } else if (side == Collision::SIDE_BOTTOM) {
    setSpeed(sf::Vector2f(speedx, std::abs(speedy)));
  } else if (side == Collision::SIDE_RIGHT) {
    setSpeed(sf::Vector2f(-std::abs(speedx), speedy));
  }
}
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
