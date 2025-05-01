#ifndef BOX_HPP
#define BOX_HPP

#include "Util/GameObject.hpp"
#include <memory>

class Character;

class Box : public Util::GameObject {
public:
    Box();

    void Update();
    void Draw();

    void SetPosition(float x, float y);
    void OnCollisionWithCharacter(std::shared_ptr<Character> character);
    bool IsCharacterOn(Character* character) const;
    glm::vec2 GetPosition() const;

    void ApplyGravity();
    bool IsGrounded();

private:
    float velocityY;
    float gravity;
    float moveSpeed;

    bool grounded;
};

#endif // BOX_HPP
