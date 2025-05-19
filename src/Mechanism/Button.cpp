#include "Mechanism/Button.hpp"

#include <iostream>

#include "Character/Character.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Platform.hpp"
#include "Util/Image.hpp"

Button::Button(Color color, const glm::vec2 &pos)
    : MechanismBase(pos, color, 25.0f) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPivot({0.0f, 0.0f});
}

std::string Button::GetImagePath(Color color) {
  switch (color) {
  case Color::BLUE:
    return RESOURCE_DIR "/material/props/button/button-blue.png";
  case Color::GREEN:
    return RESOURCE_DIR "/material/props/button/button-green.png";
  case Color::ORANGE:
    return RESOURCE_DIR "/material/props/button/button-orange.png";
  case Color::PINK:
    return RESOURCE_DIR "/material/props/button/button-pink.png";
  case Color::WHITE:
    return RESOURCE_DIR "/material/props/button/button-white.png";
  case Color::YELLOW:
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

void Button::SetPosition(const glm::vec2 &position) {
  MechanismBase::SetPosition(position);
}

void Button::Respawn() {
  MechanismBase::Respawn();
  m_IsPressedFireboy = false;
  m_IsPressedWatergirl = false;
}