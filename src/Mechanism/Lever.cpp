#include "Mechanism/Lever.hpp"
#include "Util/Image.hpp"

Lever::Lever(LeverColor color, const glm::vec2 &pos) : m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color, false)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
}

std::string Lever::GetImagePath(LeverColor color, bool isOn) const {
  std::string base = RESOURCE_DIR "/material/props/lever/lever-";
  switch (color) {
  case LeverColor::BLUE:
    base += "blue";
    break;
  case LeverColor::GREEN:
    base += "green";
    break;
  case LeverColor::ORANGE:
    base += "orange";
    break;
  case LeverColor::PINK:
    base += "pink";
    break;
  case LeverColor::WHITE:
    base += "white";
    break;
  case LeverColor::YELLOW:
    base += "yellow";
    break;
  }
  base += isOn ? "-on.png" : "-off.png";
  return base;
}

void Lever::update(Character *fb, Character *wg) {
  auto rect = getRect();

  // 檢查 Fireboy
  if (SDL_HasIntersection(&fb->getRect(), &rect)) {
    // 檢查角色的面向方向
    bool characterFacingRight = fb->IsFacingRight();

    // !m_IsOn: 桿子向右(off), m_IsOn: 桿子向左(on)
    // 只有當角色面向與桿子方向相反時才觸發
    if (!characterFacingRight && !m_IsOn) {
      // 角色面向左，桿子向右(off)，方向相反，觸發
      Toggle();
      return;
    } else if (characterFacingRight && m_IsOn) {
      // 角色面向右，桿子向左(on)，方向相反，觸發
      Toggle();
      return;
    }
  }

  // 檢查 Watergirl
  if (SDL_HasIntersection(&wg->getRect(), &rect)) {
    // 檢查角色的面向方向
    bool characterFacingRight = wg->IsFacingRight();

    // 只有當角色面向與桿子方向相反時才觸發
    if (!characterFacingRight && !m_IsOn) {
      // 角色面向左，桿子向右(off)，方向相反，觸發
      Toggle();
      return;
    } else if (characterFacingRight && m_IsOn) {
      // 角色面向右，桿子向左(on)，方向相反，觸發
      Toggle();
      return;
    }
  }
}

void Lever::Toggle() {
  m_IsOn = !m_IsOn;
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color, m_IsOn)));
  for (auto *t : m_Triggers) {
    if (m_IsOn) {
      t->OnTriggered();
    } else {
      t->OnReleased();
    }
  }
}

void Lever::linkTrigger(ITriggerable *target) { m_Triggers.push_back(target); }

LeverColor Lever::GetColor() const { return m_Color; }

const SDL_Rect &Lever::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize() * 0.4f; // 可依實際按鈕大小微調縮放
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}

void Lever::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}