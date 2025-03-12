#ifndef WATERGIRL_HPP
#define WATERGIRL_HPP

#include "Character.hpp"

class Watergirl : public Character {
public:
    explicit Watergirl();
    void UpdateAnimation() override;

private:
    int animationFrame;
};

#endif // WATERGIRL_HPP
