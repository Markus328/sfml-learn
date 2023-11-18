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
#include <cmath>
#include <iostream>
#include <vector>

class Ball : public Entity {
  float radius;
  sf::SoundBuffer bop;
  SoundPool soundPool = SoundPool::getInstance();

public:
  Ball(const Ball &other)
      : Ball(other.position, other.radius, other.shape->getFillColor()) {
    this->speed = other.speed;
    this->collidingEntities = other.collidingEntities;
  }
  Ball(const sf::Vector2f &position, float radius, const sf::Color &color)
      : Entity(position, sf::Vector2f(radius, radius), "Ball",
               new sf::CircleShape()) {
    this->radius = radius;

    sf::CircleShape *circleShape = (sf::CircleShape *)this->shape.get();

    circleShape->setRadius(radius);
    circleShape->setFillColor(color);

    updateBoundingBox();

    bop.loadFromFile("/home/markus/.config/util/sounds/voltest.wav");
  }

  bool isColliding() {
    int col = SIDE_NONE;
    bool colliding = false;
    short negateX = 1, negateY = 1;
    for (const Entity *e : Entity::getEntities()) {
      if (e != this && e->getCategory() != this->category &&
          (col = this->collide(*e)) != SIDE_NONE) {
        colliding = true;
        float speedx = getSpeed().x, speedy = getSpeed().y;
        switch (col) {
        case SIDE_TOP:
          negateX = -1;
          setSpeed(sf::Vector2f(speedx, -std::abs(speedy)));
          break;
        case SIDE_LEFT:
          negateY = -1;
          setSpeed(sf::Vector2f(std::abs(speedx), speedy));
          break;
        case SIDE_BOTTOM:
          negateX = -1;
          setSpeed(sf::Vector2f(speedx, std::abs(speedy)));
          break;
        case SIDE_RIGHT:
          negateY = -1;
          setSpeed(sf::Vector2f(-std::abs(speedx), speedy));
          break;
        }
      }
    }

    if (colliding) {
      Ball *newBall = new Ball(*this);
      newBall->setSpeed(
          sf::Vector2f(negateX * this->speed.x, negateY * this->speed.y));
    }
    return colliding;
  }
  bool isAtBorder() {
    // top
    int col = SIDE_NONE;
    bool colliding = false;
    if ((col = collide(Border::getTop())) != SIDE_NONE) {
      if (col != SIDE_TOP) {
        this->active = false;
        return false;
      }
      setSpeed(sf::Vector2f(this->speed.x, -std::abs(this->speed.y)));
      colliding = true;
    }
    // left
    if ((col = collide(Border::getLeft())) != SIDE_NONE) {
      if (col != SIDE_LEFT) {
        this->active = false;
        return false;
      }
      setSpeed(sf::Vector2f(std::abs(this->speed.x), this->speed.y));
      colliding = true;
    }
    // right
    if ((col = collide(Border::getRight())) == SIDE_RIGHT) {
      if (col != SIDE_RIGHT) {
        this->active = false;
        return false;
      }
      setSpeed(sf::Vector2f(-std::abs(this->speed.x), this->speed.y));
      colliding = true;
    }
    // bottom
    if ((col = collide(Border::getBottom())) == SIDE_BOTTOM) {
      if (col != SIDE_BOTTOM) {
        this->active = false;
        return false;
      }
      setSpeed(sf::Vector2f(this->speed.x, std::abs(this->speed.y)));
      colliding = true;
    }
    return colliding;
  }
  void step(float elapsedTime) {
    if (!this->active) {
      return;
    }
    move(this->position.x + this->speed.x * elapsedTime,
         this->position.y + -this->speed.y * elapsedTime);

    if (isColliding()) {
      soundPool.play(this->bop);
    }
  }
};
#endif
