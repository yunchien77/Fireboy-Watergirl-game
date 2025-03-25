#ifndef WATERGIRL_HPP
#define WATERGIRL_HPP

#include "Character.hpp"

class Watergirl : public Character {
public:
  explicit Watergirl();
  void UpdateAnimation() override;
  bool IsFireboy() const override { return false; }

private:
  int animationFrame;
};

#endif // WATERGIRL_HPP
