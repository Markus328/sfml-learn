
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

// Entity::Entity(const sf::Vector2f &position, const sf::Vector2f &size,
//                std::string category, World &world)
//     : Entity(position, category, world) {

//   sf::RectangleShape *rectShape = new sf::RectangleShape();
//   this->shape.reset(rectShape);

//   rectShape->setPosition(position);
//   rectShape->setSize(size);
// };
Entity::Entity(const sf::Vector2f &position, std::string category, World &world,
               sf::Shape *shape)
    : position(shape->getPosition()), world(world) {

  this->shape.reset(shape);
  this->shape->setPosition(position);

  this->category = category;
  this->id = world.getNewId();
  world.enqueueNewEntity(*this);
};

bool Entity::operator==(const Entity &other) const {
  return other.getId() == this->getId();
}

void Entity::move(float x, float y) { this->shape->setPosition(x, y); }
void Entity::move(const sf::Vector2f &position) {
  move(position.x, position.y);
}

uint16_t Entity::getId() const { return this->id; }
const sf::Vector2f &Entity::getPosition() const { return this->position; }
const sf::Shape &Entity::getShape() const { return *this->shape.get(); }
sf::Shape &Entity::getShape() { return *this->shape.get(); }
// std::string Entity::getCategory() const { return this->category; }
const World &Entity::getWorld() const { return this->world; }

void Entity::step(float elapsedTime) {
  if (!active) {
    return;
  }
}

Entity::EntityCastException::EntityCastException(uint16_t id)
    : std::runtime_error(
          fmt::format("Unable to cast entity with id: {} to type", id)) {}
