#ifndef BORDER
#define BORDER
#include "entity.cpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
class Border : public Entity {
private:
  Border(const sf::Vector2f &position, const sf::Vector2f &size,
         const std::string &name)
      : Entity(position, size, name) {
    this->shape->setFillColor(sf::Color(255, 0, 0));
  }

public:

  static Border top, left, bottom, right;
  static Border &getTop() {
    return top;
  }
  static Border &getLeft() {
    return left;
  }
  static Border &getBottom() {
    return bottom;
  }
  static Border &getRight() {
    return right;
  }
};

#endif // !BORDER
