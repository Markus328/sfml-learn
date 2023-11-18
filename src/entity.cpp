#ifndef ENTITY
#define ENTITY

#undef NDEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

enum side {
  SIDE_NONE = 0,
  SIDE_TOP = 0x001,
  SIDE_LEFT = 0x010,
  SIDE_BOTTOM = ~SIDE_TOP,
  SIDE_RIGHT = ~SIDE_LEFT
};

class Entity {

private:
  static bool nearlyEqual(float a, float b, float epsilon) {
    if (a == b) {
      return true;
    }

    float absA = std::abs(a);
    float absB = std::abs(b);
    float diff = std::abs(a - b);

    if (a == 0 || b == 0 || diff < std::numeric_limits<float>::min()) {
      return diff < (epsilon * std::numeric_limits<float>::min());
    } else {
      return diff / (absA + absB) < epsilon;
    }
  }

  static std::unordered_map<std::string, std::vector<Entity *>> entities;
  static std::queue<Entity *> newEntitiesQueue;

  static void addEntity(Entity *e, std::string category) {
    e->category = category;
    newEntitiesQueue.push(e);
    // std::cout << "Creating a new " << e->getCategory() << "...\n";
    // std::cout << "New entities count: " << newEntitiesQueue.size() << "\n";
  }

protected:
  std::unique_ptr<sf::Shape> shape;
  sf::Vector2f speed;
  float aSpeed;
  float angle;
  sf::Vector2f position;
  std::string category;
  sf::FloatRect boundingBox;
  bool active = true;
  std::vector<const Entity *> e;

  virtual void updateBoundingBox() {
    this->boundingBox = this->shape->getGlobalBounds();
  };

public:
  static void commitNewEntities() {
    // std::cout << "Commiting new entities...\n";
    int count = 0;
    while (newEntitiesQueue.size() > 0) {
      count++;
      Entity *e = newEntitiesQueue.front();
      entities["all"].push_back(e);
      entities[e->getCategory()].push_back(e);
      newEntitiesQueue.pop();
      // std::cout << "-- " << e->getCategory() << "\n";
    }
    if (count > 0) {
      std::cout << "Commited new " << count << "entities, "
                << entities["all"].size() << "total\n";
    }
  }
  static std::unordered_map<std::string, std::vector<Entity *>> &getEntities() {
    return entities;
  }
  Entity(const sf::Vector2f &position, const sf::Vector2f &size,
         std::string category) {

    addEntity(this, category);
    sf::RectangleShape *rectShape = new sf::RectangleShape();
    this->shape.reset(rectShape);

    this->position = position;
    rectShape->setPosition(position);
    rectShape->setSize(size);
    updateBoundingBox();
  };
  Entity(const sf::Vector2f &position, const sf::Vector2f &size,
         std::string category, sf::Shape *shape) {

    addEntity(this, category);
    this->shape.reset(shape);
    this->position = position;
    this->shape->setPosition(position);
  };
  virtual void move(float x, float y) {
    this->position.x = x;
    this->position.y = y;
    this->shape->setPosition(this->position);
    updateBoundingBox();
  }
  virtual void move(const sf::Vector2f &position) {
    move(position.x, position.y);
  }
  virtual void setAngle(float angle) { this->angle = angle; }
  virtual void setSpeed(float aSpeed) {
    this->aSpeed = aSpeed;
    this->speed.x = std::cos(this->angle) * aSpeed;
    this->speed.y = std::sin(this->angle) * aSpeed;
  }
  virtual void setSpeed(const sf::Vector2f &speed) {
    this->speed = speed;
    this->aSpeed = std::sqrt(speed.x * speed.x + speed.y * speed.y);

    setAngle(std::atan2(speed.y, speed.x));
    // assert(getAngle() == 45 * M_PI / 180 || getAngle() == 315 * M_PI / 180);
  }
  virtual sf::FloatRect getBoundingBox() const { return this->boundingBox; }
  virtual sf::Vector2f getPosition() const { return this->position; }
  virtual sf::Vector2f getSpeed() const { return this->speed; }
  virtual float getASpeed() const { return this->aSpeed; }
  virtual float getAngle() const { return this->angle; }
  virtual sf::Shape &getShape() const { return *this->shape.get(); }
  virtual std::string getCategory() const { return this->category; }

#define COLFUNC1()                                                             \
  sf::FloatRect inner;                                                         \
  if (this->boundingBox.intersects(obj.getBoundingBox(), inner)) {

#define COLFUNC2()                                                             \
  /*std::cout << "Colision test of " << getCategory() << " and "*/             \
  /* << obj.getCategory() << "\n"; */                                          \
  /* std::cout << "a\n"; */                                                    \
  float width = this->boundingBox.width, height = this->boundingBox.height;    \
                                                                               \
  float x = this->getBoundingBox().left, y = this->getBoundingBox().top;       \
  sf::Vector2f innerCenter((inner.left + inner.width / 2),                     \
                           (inner.top + inner.height / 2));                    \
  float l_distance = std::fabs(innerCenter.x - x);                             \
  float t_distance = std::fabs(innerCenter.y - y);                             \
  float r_distance = std::fabs(innerCenter.x - (x + width));                   \
  float b_distance = std::fabs(innerCenter.y - (y + height));                  \
                                                                               \
  float minimal = std::min(std::min(l_distance, r_distance),                   \
                           std::min(t_distance, b_distance));                  \
                                                                               \
  /* std::cout << "minimal: " << minimal << "- top: " << t_distance */         \
  /* << "- bottom: " << b_distance << "- left: " << l_distance */              \
  /* << "- right: " << r_distance << "\n";                   */                \
  if (nearlyEqual(inner.width, width, 0.01f) &&                                \
      nearlyEqual(inner.height, height, 0.01f)) {                              \
    float objWidth = obj.getBoundingBox().width,                               \
          objHeight = obj.getBoundingBox().height;                             \
    if (objWidth * objHeight > width * height) {                               \
      return ~(obj.collisionTest(*this));                                      \
    } else {                                                                   \
      /* std::cout << "c\n"; */                                                \
      return SIDE_NONE;                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  if (l_distance == minimal)                                                   \
    return SIDE_LEFT;                                                          \
  if (t_distance == minimal)                                                   \
    return SIDE_TOP;                                                           \
  if (r_distance == minimal)                                                   \
    return SIDE_RIGHT;                                                         \
  if (b_distance == minimal)                                                   \
    return SIDE_BOTTOM;                                                        \
  }

  virtual int collisionTest(const Entity &obj) const {
    COLFUNC1()
    COLFUNC2()
    return SIDE_NONE;
  }

  virtual int collide(const Entity &obj) {
    std::vector<const Entity *>::iterator colObj = this->e.end();

    for (colObj = this->e.begin(); colObj != this->e.end(); ++colObj) {
      if (*colObj == &obj) {
        break;
      }
    }
    COLFUNC1()
    if (colObj == this->e.end()) {
      this->e.push_back(&obj);
    } else {
      return SIDE_NONE;
    }
    COLFUNC2()
    else if (colObj != this->e.end()) {
      this->e.erase(colObj);
    }
    return SIDE_NONE;
  }

  virtual void instantCollide(const Entity *e, int side) {}
  virtual void continuousCollide(const Entity *e, int side) {}

  virtual void stepCollide() {
    assert(Entity::getEntities()["all"].size() > 0);
    int col = SIDE_NONE;
    for (const Entity *e : Entity::getEntities()["all"]) {
      if (e != this) {
        if ((col = this->collide(*e)) != SIDE_NONE) {
          instantCollide(e, col);
        } else if ((col = this->collisionTest(*e)) != SIDE_NONE) {
          continuousCollide(e, col);
        }
      }
    }
  }
  virtual void step(float elapsedTime) {
    if (!active) {
      return;
    }
  };
};

#endif
