#ifndef FIREBOY_HPP
#define FIREBOY_HPP

#include "Character.hpp"

class Fireboy : public Character {
public:
  explicit Fireboy();
  void UpdateAnimation() override;
  bool IsFireboy() const override { return true; }

private:
  int animationFrame;
};

#endif // FIREBOY_HPP
