
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
Entity::Entity(std::string category, World &world) : world(world) {

  this->category = category;
  this->id = world.getNewId();
  world.enqueueNewEntity(*this);
};

bool Entity::operator==(const Entity &other) const {
  return other.getId() == this->getId();
}

void Entity::move(float x, float y) {
  setPosition(getPosition().x + x, getPosition().y + y);
}
void Entity::move(const sf::Vector2f &position) {
  move(position.x, position.y);
}
void Entity::setPosition(float x, float y) { getShape().setPosition(x, y); }
void Entity::setPosition(const sf::Vector2f &position) {
  setPosition(position.x, position.y);
}

uint16_t Entity::getId() const { return this->id; }
// std::string Entity::getCategory() const { return this->category; }
const World &Entity::getWorld() const { return this->world; }

void Entity::step(float elapsedTime) {
  if (!active) {
    return;
  }
}

sf::FloatRect Entity::getBounds() const { return getShape().getGlobalBounds(); }

Entity::EntityCastException::EntityCastException(uint16_t id)
    : std::runtime_error(
          fmt::format("Unable to cast entity with id: {} to type", id)) {}
