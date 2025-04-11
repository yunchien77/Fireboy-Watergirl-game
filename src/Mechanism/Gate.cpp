#include "Mechanism/Gate.hpp"
#include "Util/Image.hpp"

Gate::Gate(GateColor color, const glm::vec2 &pos)
    : m_Color(color), m_InitialPosition(pos) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
  SetVisible(true);
}

std::string Gate::GetImagePath(GateColor color) {
  switch (color) {
  case GateColor::BLUE:
    return RESOURCE_DIR "/material/props/gate/gate-blue.png";
  case GateColor::GREEN:
    return RESOURCE_DIR "/material/props/gate/gate-green.png";
  case GateColor::ORANGE:
    return RESOURCE_DIR "/material/props/gate/gate-orange.png";
  case GateColor::PINK:
    return RESOURCE_DIR "/material/props/gate/gate-pink.png";
  case GateColor::WHITE:
    return RESOURCE_DIR "/material/props/gate/gate-white.png";
  case GateColor::YELLOW:
    return RESOURCE_DIR "/material/props/gate/gate-yellow.png";
  }
  return "";
}

void Gate::OnTriggered() { SetOpen(true); }

void Gate::OnReleased() { SetOpen(false); }

void Gate::SetOpen(bool open) {
  if (m_IsOpen == open)
    return;
  m_IsOpen = open;
  m_ShouldOpen = open;
  m_IsAnimating = true;
}

void Gate::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

bool Gate::IsOpen() const { return m_IsOpen; }

bool Gate::IsBlocking() const { return !m_IsOpen; }

GateColor Gate::GetColor() const { return m_Color; }

const SDL_Rect &Gate::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}

void Gate::UpdateAnimation(float deltaTime) {
  if (!m_IsAnimating)
    return;

  float speed = 150.0f; // pixel/sec，門滑動速度
  float maxOffsetY = 100.0f;
  glm::vec2 pos = m_Transform.translation;

  if (m_ShouldOpen) {
    // 門往下滑
    pos.y += speed * deltaTime;
    if (pos.y >= m_InitialPosition.y + maxOffsetY) {
      pos.y = m_InitialPosition.y + maxOffsetY;
      m_IsAnimating = false;
    }
  } else {
    // 門往上升
    pos.y -= speed * deltaTime;
    if (pos.y <= m_InitialPosition.y) {
      pos.y = m_InitialPosition.y;
      m_IsAnimating = false;
    }
  }

  SetPosition(pos);
}
