#include "Mechanism/Gate.hpp"
#include "Util/Image.hpp"
#include <iostream>


Gate::Gate(GateColor color, const glm::vec2 &pos)
    : m_Color(color), m_InitialPosition(pos) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
  SetVisible(true);
  SetScale({1.0f, 1.0f});
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

void Gate::OnTriggered() {
  m_ActiveTriggerCount++;
  if (m_ActiveTriggerCount == 1) {
    std::cout << "Gate set to open\n";
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

void Gate::SetScale(const glm::vec2 &scale) { m_Transform.scale = scale; }

void Gate::UpdateAnimation(float deltaTime) {
  if (!m_IsAnimating)
    return;

  float speed = 100.0f;
  float scaleSpeed = 2.0f; // scale change rate per second
  float maxOffsetY = 50.0f;

  glm::vec2 pos = m_Transform.translation;
  float currentScale = m_Transform.scale.y;

  if (m_ShouldOpen) {
    // Move gate down
    pos.y += speed * deltaTime;
    // Shrink simultaneously
    float newScale = std::max(0.0f, currentScale - scaleSpeed * deltaTime);
    SetScale({1.0f, newScale});

    if (pos.y >= m_InitialPosition.y + maxOffsetY || newScale <= 0.1f) {
      pos.y = m_InitialPosition.y + maxOffsetY;
      SetScale({1.0f, 0.0f}); // Fully shrunk
      m_IsAnimating = false;
    }
  } else {
    // Move gate up
    pos.y -= speed * deltaTime;
    // Expand simultaneously
    float newScale = std::min(1.0f, currentScale + scaleSpeed * deltaTime);
    SetScale({1.0f, newScale});

    if (pos.y <= m_InitialPosition.y) {
      pos.y = m_InitialPosition.y;
      SetScale({1.0f, 1.0f}); // Restore original size
      m_IsAnimating = false;
    }
  }

  SetPosition(pos);
}

void Gate::SetInitialState(const glm::vec2 &pos, bool isOpen) {
  m_InitialPosition = pos;
  m_InitialIsOpen = isOpen;
}

void Gate::Respawn() {
  SetPosition(m_InitialPosition);
  m_IsOpen = m_InitialIsOpen;
  m_ShouldOpen = m_InitialIsOpen;
  m_IsAnimating = true; // Restart animation to restore state

  SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color)));
  m_ActiveTriggerCount = 0;
}