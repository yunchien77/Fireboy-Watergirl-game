#include "Mechanism/Button.hpp"

#include <iostream>

#include "Character/Character.hpp"
#include "Mechanism/Gate.hpp"
#include "Util/Image.hpp"

Button::Button(ButtonColor color, const glm::vec2 &pos) : m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
  SetZIndex(25);
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
  }
  return "";
}

void Button::update(Character *fb, Character *wg) {
  auto rect = getRect();
  bool fbOn = SDL_HasIntersection(&fb->getRect(), &rect);
  bool wgOn = SDL_HasIntersection(&wg->getRect(), &rect);
  bool nowPressed = fbOn || wgOn;

  if (nowPressed && !m_IsPressed) {
    m_IsPressed = true;
    for (auto *t : m_Triggers) {
      if (auto *gate = dynamic_cast<Gate*>(t)) {
        if (gate->GetColor() == m_Color) gate->OnTriggered();
      } else if (auto *platform = dynamic_cast<Platform*>(t)) {
        if (platform->GetColor() == m_Color) platform->OnTriggered();
      }
    }
  } else if (!nowPressed && m_IsPressed) {
    m_IsPressed = false;
    for (auto *t : m_Triggers) {
      if (auto *gate = dynamic_cast<Gate*>(t)) {
        if (gate->GetColor() == m_Color) gate->OnReleased();
      } else if (auto *platform = dynamic_cast<Platform*>(t)) {
        if (platform->GetColor() == m_Color) platform->OnReleased();
      }
    }
  }
}

void Button::linkTrigger(ITriggerable *target) {
if (auto* platform = dynamic_cast<Platform*>(target)) {
    std::cout << "[LinkDebug] Button(" << static_cast<int>(m_Color)
              << ") @" << this
              << " linked to Platform(" << static_cast<int>(platform->GetColor())
              << ") @" << platform << std::endl;
  }

  m_Triggers.push_back(target);
}

const SDL_Rect &Button::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize() * 0.4f; // 可依實際按鈕大小微調縮放
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
