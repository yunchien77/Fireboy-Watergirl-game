#include "Mechanism/MechanismBase.hpp"
#include "Util/Image.hpp"

MechanismBase::MechanismBase(const glm::vec2 &position,
                             Color color,
                             float zIndex)
    : GameObject(nullptr, zIndex),
      m_InitialPosition(position),
      m_Color(color) {
  SetZIndex(zIndex);
  SetPivot({0.0f, 0.0f});
}

void MechanismBase::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
  UpdateRect();
}

glm::vec2 MechanismBase::GetPosition() const {
  return m_Transform.translation;
}

void MechanismBase::SetInitialPosition(const glm::vec2 &pos) {
  m_InitialPosition = pos;
}

void MechanismBase::Respawn() {
  SetPosition(m_InitialPosition);
  SetVisible(true);
}

void MechanismBase::Update() {
  // override
}

Color MechanismBase::GetColor() const {
  return m_Color;
}

const SDL_Rect &MechanismBase::getRect() const {
  return m_Rect;
}

void MechanismBase::UpdateRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
}

void MechanismBase::SetZIndex(float index) {
  GameObject::SetZIndex(index);
}

void MechanismBase::SetPivot(const glm::vec2 &pivot) {
  GameObject::SetPivot(pivot);
}