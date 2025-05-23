#ifndef FIREBOY_HPP
#define FIREBOY_HPP

#include "Character.hpp"

class Fireboy : public Character {
public:
  explicit Fireboy();
  void UpdateAnimation() override;
  bool IsFireboy() const override { return true; }

  float getX() const override;
  float getY() const override;
  int getWidth() const override;
  int getHeight() const override;

private:
  int animationFrame;
};

#endif // FIREBOY_HPP
