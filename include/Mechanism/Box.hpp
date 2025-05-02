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

    void SetPosition(const glm::vec2 &position);
    void OnCollisionWithCharacter(std::shared_ptr<Character> character);
    bool IsCharacterOn(Character* character) const;
    glm::vec2 GetPosition() const;

    void ApplyGravity();
    bool IsGrounded();

    void SetInitialPosition(const glm::vec2 &pos);
    void Respawn();

private:
    float velocityY;
    float gravity;
    float moveSpeed;
    bool grounded;
    mutable SDL_Rect m_Rect;
    glm::vec2 m_InitialPosition;
};

#endif // BOX_HPP
