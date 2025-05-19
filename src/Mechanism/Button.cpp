#include "Mechanism/Button.hpp"

#include <iostream>

#include "Character/Character.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Platform.hpp"
#include "Util/Image.hpp"

Button::Button(ButtonColor color, const glm::vec2 &pos) : m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
  m_InitialPosition = pos;
}

std::string Button::GetImagePath(ButtonColor color) {
  switch (color) {
  case ButtonColor::BLUE:
    return RESOURCE_DIR "/material/props/button/button-blue.png";
  case ButtonColor::GREEN:
    return RESOURCE_DIR "/material/props/button/button-green.png";
  case ButtonColor::ORANGE:
    return RESOURCE_DIR "/material/props/button/button-orange.png";
  case ButtonColor::PINK:
    return RESOURCE_DIR "/material/props/button/button-pink.png";
  case ButtonColor::WHITE:
    return RESOURCE_DIR "/material/props/button/button-white.png";
  case ButtonColor::YELLOW:
    return RESOURCE_DIR "/material/props/button/button-yellow.png";
  default:
    return "";
  }
}

void Button::Update(Character *fb, Character *wg) {
  if (!fb || !wg)
    return;

  auto rect = getRect();

  bool fbOn = SDL_HasIntersection(&fb->getRect(), &rect);
  bool wgOn = SDL_HasIntersection(&wg->getRect(), &rect);

  // First determine current overall state
  bool nowPressed = fbOn || wgOn;
  bool wasPressed = m_IsPressedFireboy || m_IsPressedWatergirl;

  // Only trigger on state change
  if (nowPressed != wasPressed) {
    for (auto *trigger : m_Triggers) {
      if (auto *gate = dynamic_cast<Gate *>(trigger)) {
        if (gate->GetColor() == m_Color) {
          if (nowPressed)
            gate->OnTriggered();
          else
            gate->OnReleased();
        }
      } else if (auto *platform = dynamic_cast<Platform *>(trigger)) {
        if (platform->GetColor() == m_Color) {
          if (nowPressed)
            platform->OnTriggered();
          else
            platform->OnReleased();
        }
      }
    }
  }

  m_IsPressedFireboy = fbOn;
  m_IsPressedWatergirl = wgOn;
}

void Button::LinkTrigger(ITriggerable *target) {
  if (target) {
    m_Triggers.push_back(target);
  }
}

const SDL_Rect &Button::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size =
      GetScaledSize() * 0.5f; // Scale according to actual button size
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}

ButtonColor Button::GetColor() const { return m_Color; }

void Button::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

void Button::SetInitialState(const glm::vec2 &pos) { m_InitialPosition = pos; }

void Button::Respawn() {
  SetPosition(m_InitialPosition);
  m_IsPressedFireboy = false;
  m_IsPressedWatergirl = false;
}