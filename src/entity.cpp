
#include "entity.hpp"
#include "collider.hpp"
#include "collision.hpp"
#include "soundpool.cpp"
#include "world.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

Entity::Entity(const sf::Vector2f &position, std::string category,
               World &world) {

  this->category = category;
  this->position = position;
  this->world = &world;
  this->id = world.getNewId();
  world.enqueueNewEntity(*this);
}

Entity::Entity(const sf::Vector2f &position, const sf::Vector2f &size,
               std::string category, World &world)
    : Entity(position, category, world) {

  sf::RectangleShape *rectShape = new sf::RectangleShape();
  this->shape.reset(rectShape);

  rectShape->setPosition(position);
  rectShape->setSize(size);
};
Entity::Entity(const sf::Vector2f &position, const sf::Vector2f &size,
               std::string category, sf::Shape *shape, World &world)
    : Entity(position, category, world) {

  this->shape.reset(shape);
  this->shape->setPosition(position);
};

void Entity::move(float x, float y) {
  this->position.x = x;
  this->position.y = y;
  this->shape->setPosition(this->position);
}
void Entity::move(const sf::Vector2f &position) {
  move(position.x, position.y);
}

uint16_t Entity::getId() const { return this->id; }
const sf::Vector2f &Entity::getPosition() const { return this->position; }
const sf::Shape &Entity::getShape() const { return *this->shape.get(); }
sf::Shape &Entity::getShape() { return *this->shape.get(); }
std::string Entity::getCategory() const { return this->category; }
const World &Entity::getWorld() const { return *this->world; }

void Entity::step(float elapsedTime) {
  if (!active) {
    return;
  }
}

Entity::EntityCastException::EntityCastException(uint16_t id)
    : std::runtime_error(
          fmt::format("Unable to cast entity with id: {} to type", id)) {}
