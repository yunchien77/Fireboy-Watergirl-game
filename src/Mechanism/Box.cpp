#include "Mechanism/Box.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <iostream>

Box::Box() {
    SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/material/props/misc/misc1.png"));
    SetPivot({0.5f, 1.0f});
    SetZIndex(20);

    velocityY = 0.0f;
    gravity = 0.4f;
    moveSpeed = 1.5f;
    grounded = false;
}

void Box::SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
}

glm::vec2 Box::GetPosition() const {
    return m_Transform.translation;
}

void Box::Update() {
    ApplyGravity();
}

void Box::ApplyGravity() {
    if (!IsGrounded()) {
        velocityY += gravity;
        m_Transform.translation.y += velocityY;
    } else {
        velocityY = 0.0f;
    }
}

bool Box::IsGrounded() {
    return grounded;
}

void Box::OnCollisionWithCharacter(std::shared_ptr<Character> character) {
    float charX = character->GetPosition().x;
    float charY = character->GetPosition().y;
    float boxX = m_Transform.translation.x;
    float boxY = m_Transform.translation.y;

    if (std::abs(charX - boxX) < 30.0f && std::abs(charY - boxY) < 40.0f) {
        std::cout << "Pushing box!" << std::endl;
        m_Transform.translation.x += moveSpeed;  // 固定往右推測試用
    }

    if (character->IsMoving() && character->IsFacingRight() && charX < boxX && std::abs(charY - boxY) < 16.0f) {
        m_Transform.translation.x += moveSpeed;
    } else if (character->IsMoving() && !character->IsFacingRight() && charX > boxX && std::abs(charY - boxY) < 16.0f) {
        m_Transform.translation.x -= moveSpeed;
    }
}

bool Box::IsCharacterOn(Character* character) const {
    glm::vec2 charPos = character->GetPosition();
    glm::vec2 boxPos = m_Transform.translation;
    glm::vec2 boxSize = GetScaledSize();

    float charBottom = charPos.y + 13.5f;
    float charLeft = charPos.x - 5.0f;
    float charRight = charPos.x + 5.0f;

    float boxTop = boxPos.y + 11.5f;
    float boxLeft = boxPos.x - (boxSize.x / 2);
    float boxRight = boxPos.x + (boxSize.x / 2);

    bool verticalMatch = (charBottom >= boxTop - 2.0f) && (charBottom <= boxTop + 2.0f);
    bool horizontalOverlap = (charRight > boxLeft + 2.0f) && (charLeft < boxRight - 2.0f);

    return verticalMatch && horizontalOverlap;
}

void Box::Draw() {
    GameObject::Draw();
}

void Box::Respawn() {
    SetPosition(m_InitialPosition);
    SetVisible(true);
}

void Box::SetInitialPosition(const glm::vec2 &pos) {
    m_InitialPosition = pos;
}
