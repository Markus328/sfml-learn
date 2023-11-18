#ifndef BALL
#define BALL
#include "border.cpp"
#include "entity.cpp"
#include "soundpool.cpp"
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
#include <iostream>
#include <vector>

class Ball : public Entity {
  float radius;
  SoundPool &soundPool = SoundPool::getInstance();

public:
  static sf::SoundBuffer bop;
  static Ball *cloneBall(const Ball &ball) {
    // assert(Entity::getEntities()["Ball"]);
    int numBalls = Ball::getEntities()["Ball"].size();
    std::cout << "balllsss: " << numBalls << "\n";
    if (numBalls >= 400) {
      return nullptr;
    }

    return new Ball(ball);
  };
  Ball(const Ball &other)
      : Ball(other.position, other.radius, other.shape->getFillColor()) {
    this->speed = other.speed;
    this->e = other.e;
  }
  Ball(const sf::Vector2f &position, float radius, const sf::Color &color)
      : Entity(position, sf::Vector2f(radius, radius), "Ball",
               new sf::CircleShape()) {
    this->radius = radius;

    sf::CircleShape *circleShape = (sf::CircleShape *)this->shape.get();

    circleShape->setRadius(radius);
    circleShape->setFillColor(color);

    updateBoundingBox();
  }

  void instantCollide(const Entity *e, int side) {
    soundPool.play(Ball::bop);

    std::cout << "angle: " << getAngle() << "\n";
    setAngle(getAngle() + ((rand() % 3) - 1) * (5 * M_PI / 180));
    setSpeed(getASpeed());
    if (e->getCategory() != "Ball") {
      Ball *newBall = cloneBall(*this);
      // if (newBall != nullptr) {
      //   newBall->setSpeed(getSpeed());
      //   // newBall->setSpeed(
      //   //     sf::Vector2f(-this->speed.x, negateY * this->speed.y));
      // }
    }

    continuousCollide(e, side);
  }
  void continuousCollide(const Entity *e, int side) {

    float speedx = getSpeed().x, speedy = getSpeed().y;
    switch (side) {
    case SIDE_TOP:
      setSpeed(sf::Vector2f(speedx, -std::abs(speedy)));
      break;
    case SIDE_LEFT:
      setSpeed(sf::Vector2f(std::abs(speedx), speedy));
      break;
    case SIDE_BOTTOM:
      setSpeed(sf::Vector2f(speedx, std::abs(speedy)));
      break;
    case SIDE_RIGHT:
      setSpeed(sf::Vector2f(-std::abs(speedx), speedy));
      break;
    }
  }
  virtual void stepCollide() {
    assert(Entity::getEntities()["all"].size() > 0);
    int col = SIDE_NONE;
    for (const Entity *e : Entity::getEntities()["all"]) {
      if (e != this) {
        if ((col = this->collide(*e)) != SIDE_NONE) {
          instantCollide(e, col);
        } else if ((col = this->collisionTest(*e)) == SIDE_NONE) {
          continue;
        }
        continuousCollide(e, col);
      }
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
  void step(float elapsedTime) {
    if (!this->active) {
      return;
    }
    move(this->position.x + this->speed.x * elapsedTime,
         this->position.y - this->speed.y * elapsedTime);
  }
};
#endif
