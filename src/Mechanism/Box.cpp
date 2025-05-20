#include "Mechanism/Box.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include <iostream>

Box::Box(const glm::vec2 &pos)
    : MechanismBase(pos, Color::NONE, 20.0f) {
  SetDrawable(std::make_shared<Util::Image>(
      RESOURCE_DIR "/material/props/misc/misc1.png"));
  SetPosition(pos);
  boxSize = GetScaledSize();
  SetPivot({0.0f, -boxSize.y / 2});

  velocityY = 0.0f;
  gravity = 0.4f;
  moveSpeed = 1.5f;
  grounded = false;
}

// 更新重力狀態
void Box::Update() {
  ApplyGravity();
}

void Box::ApplyGravity() {
  if (!m_GridSystem)
    return;

  if (!IsGrounded()) { // 不在地上
    velocityY += gravity;
    velocityY = std::min(velocityY, 12.0f);
    m_Transform.translation.y -= velocityY; // Apply velocity
  } else { // 在地上
    velocityY = 0.0f;
  }
}

bool Box::IsGrounded() {
  if (!m_GridSystem)
    return false;

  glm::vec2 pos = m_Transform.translation;

  glm::vec2 bottomLeft = {pos.x - (boxSize.x / 2.0f), pos.y};
  glm::vec2 bottomRight = {pos.x + (boxSize.x / 2.0f), pos.y};

  glm::ivec2 cellL = m_GridSystem->GameToCellPosition(bottomLeft);
  glm::ivec2 cellR = m_GridSystem->GameToCellPosition(bottomRight);

  CellType typeL = m_GridSystem->GetCell(cellL.x, cellL.y);
  CellType typeR = m_GridSystem->GetCell(cellR.x, cellR.y);

  return m_GridSystem->CanStandOn(typeL, true) ||
         m_GridSystem->CanStandOn(typeR, true);
}

// 角色推動箱子
void Box::OnCollisionWithCharacter(std::shared_ptr<Character> character) {
  if (!character || !m_GridSystem)
    return;

  if (IsCharacterOn(character.get())) {
    return;
  }

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;
  glm::vec2 charSize = character->GetSize();

  float boxLeft = boxPos.x - (boxSize.x / 2.0f);
  float boxRight = boxPos.x + (boxSize.x / 2.0f);
  float boxTop = boxPos.y + boxSize.y;
  float boxBottom = boxPos.y;

  float charLeft = charPos.x - (charSize.x / 2.0f);
  float charRight = charPos.x + (charSize.x / 2.0f);
  float charTop = charPos.y + charSize.y;
  float charBottom = charPos.y + 13.5f;

  bool horizontalOverlap = (charRight > boxLeft) && (charLeft < boxRight);
  bool verticalOverlap = (charBottom < boxTop) && (charTop > boxBottom);

  if (!horizontalOverlap || !verticalOverlap)
    return;

  float dx = charPos.x - boxPos.x;
  int pushDirection = 0;
  float pushDistance = 0.0f;

  if (dx > 0 && !character->IsFacingRight()) {
    pushDirection = -1;
    float adjustment = 5.0f;
    pushDistance = (charLeft - boxRight) - adjustment;
  } else if (dx < 0 && character->IsFacingRight()) {
    pushDirection = 1;
    float adjustment = 5.0f;
    pushDistance = (charRight - boxLeft) + adjustment;
  } else {
    return;
  }

  glm::vec2 newPos = m_Transform.translation;
  newPos.x += pushDistance;

  if (CheckCollisionWithTerrain(newPos)) {
    if (pushDirection > 0) {
      character->SetPosition(
          {boxLeft - (charSize.x / 2.0f) - 2.0f, character->GetPosition().y});
    } else {
      character->SetPosition(
          {boxRight + (charSize.x / 2.0f) + 2.0f, character->GetPosition().y});
    }
    return;
  }

  m_Transform.translation = newPos;
}

// 地形碰撞檢查
bool Box::CheckCollisionWithTerrain(const glm::vec2 &position) {
  if (!m_GridSystem)
    return false;

  float halfWidth = boxSize.x / 2.0f;

  glm::vec2 leftEdge = {position.x - halfWidth, position.y};
  glm::vec2 rightEdge = {position.x + halfWidth, position.y};

  glm::ivec2 leftCell = m_GridSystem->GameToCellPosition(leftEdge);
  glm::ivec2 rightCell = m_GridSystem->GameToCellPosition(rightEdge);

  CellType leftType = m_GridSystem->GetCell(leftCell.x, leftCell.y);
  CellType rightType = m_GridSystem->GetCell(rightCell.x, rightCell.y);

  bool canMoveLeft = m_GridSystem->CanMoveOn(leftType, true);
  bool canMoveRight = m_GridSystem->CanMoveOn(rightType, true);

  return !canMoveLeft || !canMoveRight;
}

// 判斷角色是否站在箱子上
bool Box::IsCharacterOn(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  float charBottom = charPos.y + 13.5f;
  float charLeft = charPos.x - 5.0f;
  float charRight = charPos.x + 5.0f;

  float boxTop = boxPos.y + boxSize.y;
  float boxLeft = boxPos.x - (boxSize.x / 2);
  float boxRight = boxPos.x + (boxSize.x / 2);

  bool verticalMatch =
      (charBottom >= boxTop - 5.0f) && (charBottom <= boxTop + 5.0f);
  bool horizontalOverlap =
      (charRight > boxLeft + 5.0f) && (charLeft < boxRight - 5.0f);

  return verticalMatch && horizontalOverlap;
}

// 檢查角色是否與箱子碰撞
bool Box::CheckCharacterCollision(std::shared_ptr<Character> character) {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  float charLeft = charPos.x - character->GetSize().x / 2.0f;
  float charRight = charPos.x + character->GetSize().x / 2.0f;
  float charBottom = charPos.y + 13.5f;
  float charTop = charPos.y + character->GetSize().y;

  float boxLeft = boxPos.x - boxSize.x / 2.0f;
  float boxRight = boxPos.x + boxSize.x / 2.0f;
  float boxBottom = boxPos.y;
  float boxTop = boxPos.y + boxSize.y;

  if (IsCharacterOn(character.get())) {
    return false;
  }

  bool horizontalOverlap = (charRight > boxLeft) && (charLeft < boxRight);
  bool verticalOverlap = (charTop > boxBottom) && (charBottom < boxTop);
  bool collision = horizontalOverlap && verticalOverlap;

  if (collision) {
    OnCollisionWithCharacter(character);
  }

  return collision;
}