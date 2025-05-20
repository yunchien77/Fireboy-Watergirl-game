#include "Mechanism/Platform.hpp"

#include "Character/Character.hpp"
#include "Util/Image.hpp"

Platform::Platform(PlatformColor color, const glm::vec2 &pos,
                   const glm::vec2 &moveOffset)
    : MechanismBase(pos, color, 25.0f), m_MoveOffset(moveOffset) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
}

std::string Platform::GetImagePath(PlatformColor color) {
  std::string base = RESOURCE_DIR "/material/props/platform/platform-";
  switch (color) {
  case Color::BLUE:
    return base + "blue.png";
  case Color::GREEN:
    return base + "green.png";
  case Color::ORANGE:
    return base + "orange.png";
  case Color::PINK:
    return base + "pink.png";
  case Color::WHITE:
    return base + "white.png";
  case Color::YELLOW:
    return base + "yellow.png";
  default:
    return base + "white.png";
  }
}

void Platform::OnTriggered() {
  m_TargetPosition = m_InitialPosition + m_MoveOffset;
  m_ShouldMove = true;
  m_IsAnimating = true;
}

void Platform::OnReleased() {
  m_TargetPosition = m_InitialPosition;
  m_ShouldMove = false;
  m_IsAnimating = true;
}

void Platform::UpdateAnimation(float deltaTime,
                               const std::vector<Character *> &characters) {
  if (!m_IsAnimating) {
    m_LastDeltaMovement = {0.0f, 0.0f};
    return;
  }

  glm::vec2 oldPos = m_Transform.translation;
  glm::vec2 direction = m_TargetPosition - oldPos;

  // 太近直接傳送
  if (glm::length(direction) < 1.0f) {
    m_LastDeltaMovement = m_TargetPosition - oldPos;
    SetPosition(m_TargetPosition);
    m_IsAnimating = false;
    return;
  }

  float speed = 80.0f;
  glm::vec2 movement = glm::normalize(direction) * speed * deltaTime;

  // 防止 overshoot
  if (glm::length(movement) > glm::length(direction)) {
    movement = direction;
    m_IsAnimating = false;
  }

  // 如果是往下移動，檢查與角色碰撞
  if (movement.y < 0) {
    for (Character *c : characters) {
      if (WillCollideWithCharacterBelow(c, movement)) {
        m_IsAnimating = false;
        m_LastDeltaMovement = {0.0f, 0.0f};
        return;
      }
    }
  }

  SetPosition(oldPos + movement);
  m_LastDeltaMovement = movement;
}

bool Platform::WillCollideWithCharacterBelow(Character *character,
                                             const glm::vec2 &movement) const {
  glm::vec2 nextPlatPos = m_Transform.translation + movement;
  glm::vec2 platSize = GetScaledSize();

  float platLeft = nextPlatPos.x - platSize.x / 2;
  float platRight = nextPlatPos.x + platSize.x / 2;
  float platBottom = nextPlatPos.y;
  float platTop = nextPlatPos.y + 11.5f;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 charSize = character->GetSize();

  float charLeft = charPos.x - charSize.x / 2;
  float charRight = charPos.x + charSize.x / 2;
  float charTop = charPos.y + charSize.y;
  float charBottom = charPos.y + 13.5f;

  bool horizontalOverlap = (charRight > platLeft) && (charLeft < platRight);
  bool verticalOverlap = (charTop > platBottom) && (charBottom < platTop);

  return horizontalOverlap && verticalOverlap;
}

glm::vec2 Platform::GetDeltaMovement() const { return m_LastDeltaMovement; }

bool Platform::IsCharacterOn(Character *character) const {
  // 獲取角色和平台的位置與尺寸
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 platPos = m_Transform.translation;
  glm::vec2 platSize = GetScaledSize();

  // 計算角色底部位置 (注意character底部偏移是13.5f)
  float charBottom = charPos.y + 13.5f;

  // 角色左右邊界
  float charLeft = charPos.x - 5.0f;
  float charRight = charPos.x + 5.0f;

  // 平台邊界
  float platTop = platPos.y + 11.5f;
  float platLeft = platPos.x - (platSize.x / 2);
  float platRight = platPos.x + (platSize.x / 2);

  // 檢查角色腳底是否在平台上方
  bool verticalMatch =
      (charBottom >= platTop - 2.0f) && (charBottom <= platTop + 2.0f);

  // 檢查水平方向是否有足夠重疊
  bool horizontalOverlap =
      (charRight > platLeft + 2.0f) && (charLeft < platRight - 2.0f);

  return verticalMatch && horizontalOverlap;
}

bool Platform::CheckCollision(Character *character, int moveDirection) const {
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 charSize = character->GetSize();
  glm::vec2 platPos = m_Transform.translation;
  glm::vec2 platSize = GetScaledSize();

  // 計算角色的邊界
  float charLeft = charPos.x - (charSize.x / 2);
  float charRight = charPos.x + (charSize.x / 2);
  float charTop = charPos.y + charSize.y;
  float charBottom = charPos.y + 13.5f;

  // 計算平台的邊界
  float platLeft = platPos.x - (platSize.x / 2);
  float platRight = platPos.x + (platSize.x / 2);
  float platTop = platPos.y + 11.5f;
  float platBottom = platPos.y;

  // 相交區域檢查
  bool horizontalOverlap = (charRight > platLeft) && (charLeft < platRight);
  bool verticalOverlap = (charTop > platBottom) && (charBottom < platTop);

  // 檢查是否相交
  if (horizontalOverlap && verticalOverlap) {
    // 使用更寬的碰撞範圍，尤其是跳躍時
    float sideCollisionTolerance = character->IsJumping() ? 10.0f : 5.0f;

    if (moveDirection > 0 && charRight >= platLeft &&
        charRight <= platLeft + sideCollisionTolerance) {
      // 從左側碰撞
      return true;
    } else if (moveDirection < 0 && charLeft <= platRight &&
               charLeft >= platRight - sideCollisionTolerance) {
      // 從右側碰撞
      return true;
    } else if (charBottom <= platTop && charBottom >= platTop - 5.0f) {
      // 從上方碰撞，這種情況由IsCharacterOn處理，不算碰撞
      return false;
    } else if (charTop >= platBottom && charTop <= platBottom + 5.0f) {
      // 從下方碰撞
      return true;
    }
  }
  return false;
}