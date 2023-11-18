#ifndef SOUND_POOL
#define SOUND_POOL

#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cstddef>
#include <iostream>
#include <vector>

class SoundPool {
  std::vector<sf::Sound *> sounds =
      std::vector<sf::Sound *>(256, std::nullptr_t());

  SoundPool(){};

public:
  static SoundPool &getInstance() {
    static SoundPool instance;
    return instance;
  }
  ~SoundPool() {
    for (auto &s : sounds) {
      delete s;
    }
  }
  void play(const sf::SoundBuffer &buffer) {
    sf::Sound *suitableInstance;
    for (auto &s : sounds) {
      if (s == std::nullptr_t()) {
        s = new sf::Sound();
        suitableInstance = s;
        std::cout << "New sound: " << s << "\n";
        break;
      }
      if (s->getStatus() == sf::Sound::Stopped) {
        suitableInstance = s;
        std::cout << "Using old sound: " << s << "\n";
        break;
      }
    }

    suitableInstance->setBuffer(buffer);
    suitableInstance->play();
  }
};
#endif // !SOUND_POOL
