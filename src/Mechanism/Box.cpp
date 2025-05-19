#include "Mechanism/Box.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <iostream>

Box::Box() : MechanismBase({0.0f, 0.0f}, Color::WHITE, 20.0f) {
  SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR
                                            "/material/props/misc/misc1.png"));

  m_VelocityY = 0.0f;
  m_Gravity = 0.4f;
  m_MoveSpeed = 1.5f;
  m_Grounded = false;
  m_BoxSize = GetScaledSize();
  SetPivot({0.0f, -m_BoxSize.y / 2});
}

void Box::SetPosition(const glm::vec2 &position) {
  MechanismBase::SetPosition(position);
}

glm::vec2 Box::GetPosition() const { return MechanismBase::GetPosition(); }

void Box::Update() { ApplyGravity(); }

void Box::ApplyGravity() {
  if (!m_GridSystem)
    return;

  if (!IsGrounded()) { // Not on ground
    m_VelocityY += m_Gravity;
    m_VelocityY = std::min(m_VelocityY, 12.0f);
    m_Transform.translation.y -= m_VelocityY; // Apply velocity
  } else {                                    // On ground
    m_VelocityY = 0.0f;
  }
}

bool Box::IsGrounded() {
  if (!m_GridSystem)
    return false;

  glm::vec2 pos = m_Transform.translation;

  glm::vec2 bottomLeft = {pos.x - (m_BoxSize.x / 2.0f), pos.y};
  glm::vec2 bottomRight = {pos.x + (m_BoxSize.x / 2.0f), pos.y};

  glm::ivec2 cellL = m_GridSystem->GameToCellPosition(bottomLeft);
  glm::ivec2 cellR = m_GridSystem->GameToCellPosition(bottomRight);

  CellType typeL = m_GridSystem->GetCell(cellL.x, cellL.y);
  CellType typeR = m_GridSystem->GetCell(cellR.x, cellR.y);

  return m_GridSystem->CanStandOn(typeL, true) ||
         m_GridSystem->CanStandOn(typeR, true);
}

void Box::OnCollisionWithCharacter(std::shared_ptr<Character> character) {
  if (!character || !m_GridSystem)
    return;

  // First check if character is standing on the box
  if (IsCharacterOn(character.get())) {
    // Character is on the box, don't allow pushing
    return;
  }

  // Get character and box positions and sizes
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;
  glm::vec2 charSize = character->GetSize();

  // Calculate box collision bounds
  float boxLeft = boxPos.x - (m_BoxSize.x / 2.0f);
  float boxRight = boxPos.x + (m_BoxSize.x / 2.0f);
  float boxTop = boxPos.y + m_BoxSize.y;
  float boxBottom = boxPos.y;

  // Calculate character collision bounds
  float charLeft = charPos.x - (charSize.x / 2.0f);
  float charRight = charPos.x + (charSize.x / 2.0f);
  float charTop = charPos.y + charSize.y;
  float charBottom = charPos.y + 13.5f; // Character's feet position

  // Determine horizontal overlap
  bool horizontalOverlap = (charRight > boxLeft) && (charLeft < boxRight);

  // Determine vertical overlap
  bool verticalOverlap = (charBottom < boxTop) && (charTop > boxBottom);

  // If not enough body overlap, don't allow pushing
  if (!horizontalOverlap || !verticalOverlap) {
    return;
  }

  // Calculate horizontal distance between character and box center
  float dx = charPos.x - boxPos.x;

  // Check push conditions: direction and collision
  int pushDirection = 0;
  float pushDistance = 0.0f;

  // Vertical overlap confirmed, now check horizontal push conditions
  if (dx > 0 && !character->IsFacingRight()) {
    // Character is to the right of the box, facing left, trying to push left
    pushDirection = -1;

    // Horizontal movement amount - adjust based on character position
    float adjustment =
        5.0f; // Fine-tuning coefficient, can be adjusted as needed
    pushDistance = (charLeft - boxRight) - adjustment;
  } else if (dx < 0 && character->IsFacingRight()) {
    // Character is to the left of the box, facing right, trying to push right
    pushDirection = 1;

    // Horizontal movement amount - adjust based on character position
    float adjustment =
        5.0f; // Fine-tuning coefficient, can be adjusted as needed
    pushDistance = (charRight - boxLeft) + adjustment;
  } else {
    // Push conditions not met (character position or direction incorrect)
    return;
  }

  // Check if the box's new position would collide with terrain
  glm::vec2 newPos = m_Transform.translation;
  newPos.x += pushDistance;

  if (CheckCollisionWithTerrain(newPos)) {
    // There's an obstacle in the box's movement direction, reject character
    // movement Return character to original position (blocked by box)
    if (pushDirection > 0) {
      // Character pushing from left, place character on box's left side
      character->SetPosition(
          {boxLeft - (charSize.x / 2.0f) - 2.0f, character->GetPosition().y});
    } else {
      // Character pushing from right, place character on box's right side
      character->SetPosition(
          {boxRight + (charSize.x / 2.0f) + 2.0f, character->GetPosition().y});
    }
    return;
  }

  // No collision, allow pushing, set new position
  m_Transform.translation = newPos;
}

// Check box collision with terrain
bool Box::CheckCollisionWithTerrain(const glm::vec2 &position) {
  if (!m_GridSystem)
    return false;

  float halfWidth = m_BoxSize.x / 2.0f;

  // Check box's left and right edges
  glm::vec2 leftEdge = {position.x - halfWidth, position.y};
  glm::vec2 rightEdge = {position.x + halfWidth, position.y};

  // Convert to grid coordinates
  glm::ivec2 leftCell = m_GridSystem->GameToCellPosition(leftEdge);
  glm::ivec2 rightCell = m_GridSystem->GameToCellPosition(rightEdge);

  // Get cell types at these positions
  CellType leftType = m_GridSystem->GetCell(leftCell.x, leftCell.y);
  CellType rightType = m_GridSystem->GetCell(rightCell.x, rightCell.y);

  // Determine if these positions are movable (not walls)
  bool canMoveLeft = m_GridSystem->CanMoveOn(leftType, true);
  bool canMoveRight = m_GridSystem->CanMoveOn(rightType, true);

  // If either edge would hit immovable terrain, return true indicating
  // collision
  return !canMoveLeft || !canMoveRight;
}

bool Box::IsCharacterOn(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  // Character bottom position
  float charBottom = charPos.y + 13.5f;

  // Foot position
  float charLeft = charPos.x - 5.0f;
  float charRight = charPos.x + 5.0f;

  // Box top position
  float boxTop = boxPos.y + m_BoxSize.y;
  float boxLeft = boxPos.x - (m_BoxSize.x / 2);
  float boxRight = boxPos.x + (m_BoxSize.x / 2);

  // Vertical check: feet near box top
  bool verticalMatch =
      (charBottom >= boxTop - 5.0f) && (charBottom <= boxTop + 5.0f);

  // Horizontal check: character has sufficient overlap with box
  bool horizontalOverlap =
      (charRight > boxLeft + 5.0f) && (charLeft < boxRight - 5.0f);

  // Only when both vertical and horizontal match, is character considered on
  // box
  return verticalMatch && horizontalOverlap;
}

void Box::Respawn() {
  MechanismBase::Respawn();
  m_VelocityY = 0.0f;
  m_Grounded = false;
}

void Box::SetInitialPosition(const glm::vec2 &pos) {
  MechanismBase::SetInitialPosition(pos);
}

// Check if character collides with box
bool Box::CheckCharacterCollision(std::shared_ptr<Character> character) {
  if (!character)
    return false;

  // Get character and box positions and sizes
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  // Calculate character bounds
  float charLeft = charPos.x - character->GetSize().x / 2.0f;
  float charRight = charPos.x + character->GetSize().x / 2.0f;
  float charBottom = charPos.y + 13.5f;
  float charTop = charPos.y + character->GetSize().y;

  // Calculate box bounds
  float boxLeft = boxPos.x - m_BoxSize.x / 2.0f;
  float boxRight = boxPos.x + m_BoxSize.x / 2.0f;
  float boxBottom = boxPos.y;
  float boxTop = boxPos.y + m_BoxSize.y;

  // Check if character is on top of box
  if (IsCharacterOn(character.get())) {
    return false; // Standing on box doesn't trigger collision
  }

  // Check for horizontal and vertical overlap
  bool horizontalOverlap = (charRight > boxLeft) && (charLeft < boxRight);
  bool verticalOverlap = (charTop > boxBottom) && (charBottom < boxTop);

  // If both axes overlap, collision has occurred
  bool collision = horizontalOverlap && verticalOverlap;

  if (collision) {
    // If collision, handle box pushing logic
    OnCollisionWithCharacter(character);
  }

  return collision;
}
