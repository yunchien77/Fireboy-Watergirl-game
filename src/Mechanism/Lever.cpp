#include "Mechanism/Lever.hpp"
#include "Util/Image.hpp"

Lever::Lever(LeverColor color, const glm::vec2 &pos)
    : MechanismBase(pos, color, 25.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color, false)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
}

std::string Lever::GetImagePath(LeverColor color, bool isOn) const {
  std::string base = RESOURCE_DIR "/material/props/lever/lever-";
  switch (color) {
  case Color::BLUE:
    base += "blue";
    break;
  case Color::GREEN:
    base += "green";
    break;
  case Color::ORANGE:
    base += "orange";
    break;
  case Color::PINK:
    base += "pink";
    break;
  case Color::WHITE:
    base += "white";
    break;
  case Color::YELLOW:
    base += "yellow";
    break;
  default:
    return base + "white.png";
  }
  base += isOn ? "-on.png" : "-off.png";
  return base;
}

void Lever::Update(Character *fb, Character *wg) {
  auto rect = getRect();

  // 檢查 Fireboy
  if (SDL_HasIntersection(&fb->getRect(), &rect)) {
    bool facing = fb->IsFacingRight();
    if ((!facing && !m_IsOn) || (facing && m_IsOn)) {
      Toggle();
      return;
    }
  }

  // 檢查 Watergirl
  if (SDL_HasIntersection(&wg->getRect(), &rect)) {
    bool facing = wg->IsFacingRight();
    if ((!facing && !m_IsOn) || (facing && m_IsOn)) {
      Toggle();
      return;
    }
  }
}

void Lever::Toggle() {
  m_IsOn = !m_IsOn;
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color, m_IsOn)));
  for (auto *t : m_Triggers) {
    if (m_IsOn)
      t->OnTriggered();
    else
      t->OnReleased();
  }
}

void Lever::LinkTrigger(ITriggerable *target) { m_Triggers.push_back(target); }

LeverColor Lever::GetColor() const { return m_Color; }

const SDL_Rect &Lever::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize() * 0.4f;
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}

void Lever::SetInitialState(const glm::vec2 &pos, bool isOn) {
  m_InitialPosition = pos;
  m_InitialIsOn = isOn;
}

void Lever::Respawn() {
  SetPosition(m_InitialPosition);
  m_IsOn = m_InitialIsOn;
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color, m_IsOn)));
  for (auto *t : m_Triggers) {
    if (m_IsOn)
      t->OnTriggered();
    else
      t->OnReleased();
  }
}