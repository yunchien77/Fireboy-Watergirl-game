#ifndef WATERGIRL_HPP
#define WATERGIRL_HPP

#include "Character.hpp"

class Watergirl : public Character {
public:
  explicit Watergirl();
  void UpdateAnimation() override;
  bool IsFireboy() const override { return false; }

  float getX() const override;
  float getY() const override;
  int getWidth() const override;
  int getHeight() const override;

private:
  int animationFrame;
};

#endif // WATERGIRL_HPP
