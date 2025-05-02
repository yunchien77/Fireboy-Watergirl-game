#include "Mechanism/Platform.hpp"

#include "Character/Character.hpp"
#include "Util/Image.hpp"

Platform::Platform(PlatformColor color, const glm::vec2 &pos,
                   const glm::vec2 &moveOffset)
    : m_InitialPosition(pos), m_MoveOffset(moveOffset), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
}

std::string Platform::GetImagePath(PlatformColor color) const {
  std::string base = RESOURCE_DIR "/material/props/platform/platform-";
  switch (color) {
  case PlatformColor::BLUE:
    base += "blue";
    break;
  case PlatformColor::GREEN:
    base += "green";
    break;
  case PlatformColor::ORANGE:
    base += "orange";
    break;
  case PlatformColor::PINK:
    base += "pink";
    break;
  case PlatformColor::WHITE:
    base += "white";
    break;
  case PlatformColor::YELLOW:
    base += "yellow";
    break;
  }
  base += ".png";
  return base;
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

void Platform::UpdateAnimation(float deltaTime) {
  if (!m_IsAnimating) {
    m_LastDeltaMovement = {0.0f, 0.0f}; // 沒動就清空
    return;
  }

  glm::vec2 oldPos = m_Transform.translation;
  glm::vec2 direction = m_TargetPosition - oldPos;

  // 如果距離太近，直接移動到目標位置
  if (glm::length(direction) < 1.0f) {
    m_LastDeltaMovement = m_TargetPosition - oldPos;
    SetPosition(m_TargetPosition);
    m_IsAnimating = false;
    return;
  }

  float speed = 80.0f;
  glm::vec2 movement = glm::normalize(direction) * speed * deltaTime;

  // 防止超過目標
  if (glm::length(movement) > glm::length(direction)) {
    movement = direction;
    m_IsAnimating = false;
  }

  // 更新位置並記錄移動量
  SetPosition(oldPos + movement);
  m_LastDeltaMovement = movement;
}

void Platform::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

PlatformColor Platform::GetColor() const { return m_Color; }

glm::vec2 Platform::GetDeltaMovement() const { return m_LastDeltaMovement; }

bool Platform::IsCharacterOn(Character *character) const {
  // 獲取角色和平台的位置與尺寸
  glm::vec2 charPos = character->GetPosition();
  // glm::vec2 charSize = character->GetSize();
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

const SDL_Rect &Platform::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x);
  m_Rect.y = static_cast<int>(pos.y);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}
