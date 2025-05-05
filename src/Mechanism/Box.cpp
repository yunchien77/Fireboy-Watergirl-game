#include "Mechanism/Box.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <iostream>

Box::Box() {
  SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR
                                            "/material/props/misc/misc1.png"));

  SetZIndex(20);

  velocityY = 0.0f;
  gravity = 0.4f;
  moveSpeed = 1.5f;
  grounded = false;
  boxSize = GetScaledSize();
  SetPivot({0.0f, -boxSize.y / 2});
}

void Box::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

glm::vec2 Box::GetPosition() const { return m_Transform.translation; }

void Box::Update() { ApplyGravity(); }

void Box::ApplyGravity() {
  if (!m_GridSystem)
    return;

  if (!IsGrounded()) { // 不在地上
    velocityY += gravity;
    velocityY = std::min(velocityY, 12.0f);
    m_Transform.translation.y -= velocityY; // Apply velocity
  } else {                                  // 在地上
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

void Box::OnCollisionWithCharacter(std::shared_ptr<Character> character) {
  if (!character || !m_GridSystem)
    return;

  // 首先判斷角色是否站在箱子上方
  if (IsCharacterOn(character.get())) {
    // 角色站在箱子上，不進行推動
    return;
  }

  float charX = character->GetPosition().x;
  float boxX = m_Transform.translation.x;
  float dx = charX - boxX;

  // 決定箱子的推動方向
  int pushDirection = 0;
  float pushDistance = 0.0f;

  // 根據角色的位置和面向來決定箱子推動方向
  if (dx > 0 && dx < 30.0f && !character->IsFacingRight()) {
    // 角色在箱子右側，向左推
    pushDirection = -1;
    pushDistance = charX - 30.0f - boxX;
  } else if (dx < 0 && dx > -30.0f && character->IsFacingRight()) {
    // 角色在箱子左側，向右推
    pushDirection = 1;
    pushDistance = charX + 30.0f - boxX;
  } else {
    // 不滿足推動條件
    return;
  }

  // 檢查箱子新位置是否會與地形碰撞
  glm::vec2 newPos = m_Transform.translation;
  newPos.x += pushDistance;

  if (CheckCollisionWithTerrain(newPos)) {
    // 箱子移動方向有障礙物，拒絕角色的移動
    // 讓角色回到原位置（被箱子擋住）
    if (pushDirection > 0) {
      // 角色從左側推，將角色放在箱子左側
      character->SetPosition(
          {m_Transform.translation.x - 30.0f, character->GetPosition().y});
    } else {
      // 角色從右側推，將角色放在箱子右側
      character->SetPosition(
          {m_Transform.translation.x + 30.0f, character->GetPosition().y});
    }
    return;
  }

  // 沒有碰撞，允許推動，設置新位置
  m_Transform.translation = newPos;
}

// 檢查箱子與地形的碰撞
bool Box::CheckCollisionWithTerrain(const glm::vec2 &position) {
  if (!m_GridSystem)
    return false;

  float halfWidth = boxSize.x / 2.0f;

  // 檢查箱子的左邊緣和右邊緣
  glm::vec2 leftEdge = {position.x - halfWidth, position.y};
  glm::vec2 rightEdge = {position.x + halfWidth, position.y};

  // 轉換為網格座標
  glm::ivec2 leftCell = m_GridSystem->GameToCellPosition(leftEdge);
  glm::ivec2 rightCell = m_GridSystem->GameToCellPosition(rightEdge);

  // 取得這些位置的單元格類型
  CellType leftType = m_GridSystem->GetCell(leftCell.x, leftCell.y);
  CellType rightType = m_GridSystem->GetCell(rightCell.x, rightCell.y);

  // 判斷這些位置是否可以移動 (不是牆壁)
  bool canMoveLeft = m_GridSystem->CanMoveOn(leftType, true);
  bool canMoveRight = m_GridSystem->CanMoveOn(rightType, true);

  // 如果任一邊緣會碰到不可移動的地形，返回 true 表示有碰撞
  return !canMoveLeft || !canMoveRight;
}

bool Box::IsCharacterOn(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  // 角色底部位置
  float charBottom = charPos.y + 13.5f;

  // 腳部位置
  float charLeft = charPos.x - 5.0f;
  float charRight = charPos.x + 5.0f;

  // 箱子頂部位置
  float boxTop = boxPos.y + boxSize.y;
  float boxLeft = boxPos.x - (boxSize.x / 2);
  float boxRight = boxPos.x + (boxSize.x / 2);

  // 垂直方向檢查：腳部是否在箱子頂部附近
  bool verticalMatch =
      (charBottom >= boxTop - 5.0f) && (charBottom <= boxTop + 5.0f);

  // 水平方向檢查：角色是否與箱子有足夠的重疊
  bool horizontalOverlap =
      (charRight > boxLeft + 5.0f) && (charLeft < boxRight - 5.0f);

  // 只有當垂直和水平都符合時，才算站在箱子上
  return verticalMatch && horizontalOverlap;
}

void Box::Respawn() {
  SetPosition(m_InitialPosition);
  SetVisible(true);
}

void Box::SetInitialPosition(const glm::vec2 &pos) { m_InitialPosition = pos; }

// 檢查角色是否與箱子碰撞
bool Box::CheckCharacterCollision(std::shared_ptr<Character> character) {
  if (!character)
    return false;

  // 獲取角色和箱子的位置與尺寸
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 boxPos = m_Transform.translation;

  // 計算角色的邊界
  float charLeft = charPos.x - character->GetSize().x / 2.0f;
  float charRight = charPos.x + character->GetSize().x / 2.0f;
  float charBottom = charPos.y + 13.5f;
  float charTop = charPos.y + character->GetSize().y;

  // 計算箱子的邊界
  float boxLeft = boxPos.x - boxSize.x / 2.0f;
  float boxRight = boxPos.x + boxSize.x / 2.0f;
  float boxBottom = boxPos.y;
  float boxTop = boxPos.y + boxSize.y;

  // 檢查角色是否在箱子上方
  if (IsCharacterOn(character.get())) {
    return false; // 站在箱子上不觸發碰撞
  }

  // 檢查是否存在水平和垂直重疊
  bool horizontalOverlap = (charRight > boxLeft) && (charLeft < boxRight);
  bool verticalOverlap = (charTop > boxBottom) && (charBottom < boxTop);

  // 如果兩個軸都有重疊，表示發生碰撞
  bool collision = horizontalOverlap && verticalOverlap;

  if (collision) {
    // 如果碰撞，處理推箱子邏輯
    OnCollisionWithCharacter(character);
  }

  return collision;
}