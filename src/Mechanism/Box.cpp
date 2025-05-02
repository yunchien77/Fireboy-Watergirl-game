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
    if (!m_GridSystem) return;

    if (!IsGrounded()) {
        velocityY += gravity;
        velocityY = std::min(velocityY, 12.0f);  // 防止加速過快
        //m_Transform.translation.y += velocityY; //加這個他會浮起來
    } else {
        velocityY = 0.0f;
    }
}

bool Box::IsGrounded() {
    if (!m_GridSystem) return false;

    glm::vec2 pos = m_Transform.translation;
    glm::vec2 size = GetScaledSize();

    glm::vec2 bottomLeft = {pos.x - size.x / 2.0f + 1.0f, pos.y + 1.0f};
    glm::vec2 bottomRight = {pos.x + size.x / 2.0f - 1.0f, pos.y + 1.0f};

    glm::ivec2 cellL = m_GridSystem->GameToCellPosition(bottomLeft);
    glm::ivec2 cellR = m_GridSystem->GameToCellPosition(bottomRight);

    CellType typeL = m_GridSystem->GetCell(cellL.x, cellL.y);
    CellType typeR = m_GridSystem->GetCell(cellR.x, cellR.y);

    return m_GridSystem->CanStandOn(typeL, true) || m_GridSystem->CanStandOn(typeR, true);
}

void Box::OnCollisionWithCharacter(std::shared_ptr<Character> character) {
    float charX = character->GetPosition().x;
    float charY = character->GetPosition().y;
    float boxX = m_Transform.translation.x;
    float boxY = m_Transform.translation.y;

    float dx = charX - boxX;
    //float dy = std::abs(charY - boxY);

    if (dx > 0 && dx < 30.0f && !character->IsFacingRight()) {
        m_Transform.translation.x = character->GetPosition().x - 30.0f;
    } else if (dx < 0 && dx > -30.0f && character->IsFacingRight()) {
        m_Transform.translation.x = character->GetPosition().x + 30.0f;
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