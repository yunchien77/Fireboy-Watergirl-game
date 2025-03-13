#ifndef FIREBOY_HPP
#define FIREBOY_HPP

#include "Character.hpp"

class Fireboy : public Character {
public:
  explicit Fireboy();
  void UpdateAnimation() override;

private:
  int animationFrame;
};

#endif // FIREBOY_HPP
