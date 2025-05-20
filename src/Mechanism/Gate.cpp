#include "Mechanism/Gate.hpp"
#include "Util/Image.hpp"

Gate::Gate(GateColor color, const glm::vec2 &pos)
    : MechanismBase(pos, color, 15.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
}

std::string Gate::GetImagePath(GateColor color) {
  std::string base = RESOURCE_DIR "/material/props/gate/gate-";
  switch (color) {
    case Color::BLUE:   return base + "blue.png";
    case Color::GREEN:  return base + "green.png";
    case Color::ORANGE: return base + "orange.png";
    case Color::PINK:   return base + "pink.png";
    case Color::WHITE:  return base + "white.png";
    case Color::YELLOW: return base + "yellow.png";
    default:            return "";
  }
}

void Gate::OnTriggered() {
  m_ActiveTriggerCount++;
  if (m_ActiveTriggerCount == 1) {
    SetOpen(true);
  }
}

void Gate::OnReleased() {
  if (m_ActiveTriggerCount > 0) {
    m_ActiveTriggerCount--;
    if (m_ActiveTriggerCount == 0) {
      SetOpen(false);
    }
  }
}

void Gate::SetOpen(bool open) {
  if (m_IsOpen != open)
    m_IsOpen = open;
  m_ShouldOpen = open;
  m_IsAnimating = true;
}

void Gate::SetScale(const glm::vec2 &scale) {
  m_Transform.scale = scale;
}

void Gate::SetInitialState(const glm::vec2 &pos, bool isOpen) {
  m_InitialPosition = pos;
  m_InitialIsOpen = isOpen;
}

void Gate::Respawn() {
  SetPosition(m_InitialPosition);
  m_IsOpen = m_InitialIsOpen;
  m_ShouldOpen = m_InitialIsOpen;
  m_IsAnimating = true;

  SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color)));
  m_ActiveTriggerCount = 0;
}

void Gate::UpdateAnimation(float deltaTime) {
  if (!m_IsAnimating)
    return;

  float speed = 100.0f;
  float scaleSpeed = 2.0f; // 每秒縮放變化率
  float maxOffsetY = 50.0f;

  glm::vec2 pos = m_Transform.translation;
  float currentScale = m_Transform.scale.y;

  if (m_ShouldOpen) {
    // 門往下滑
    pos.y += speed * deltaTime;
    // 同時縮小
    float newScale = std::max(0.0f, currentScale - scaleSpeed * deltaTime);
    SetScale({1.0f, newScale});

    if (pos.y >= m_InitialPosition.y + maxOffsetY || newScale <= 0.1f) {
      pos.y = m_InitialPosition.y + maxOffsetY;
      SetScale({1.0f, 0.0f}); // 完全縮小
      m_IsAnimating = false;
    }
  } else {
    // 門往上升
    pos.y -= speed * deltaTime;
    // 同時放大
    float newScale = std::min(1.0f, currentScale + scaleSpeed * deltaTime);
    SetScale({1.0f, newScale});

    if (pos.y <= m_InitialPosition.y) {
      pos.y = m_InitialPosition.y;
      SetScale({1.0f, 1.0f}); // 恢復原始大小
      m_IsAnimating = false;
    }
  }

  SetPosition(pos);
}

GateColor Gate::GetColor() const {
  return m_Color;
}

bool Gate::IsOpen() const {
  return m_IsOpen;
}

bool Gate::IsBlocking() const {
  return !m_IsOpen;
}