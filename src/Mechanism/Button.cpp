#include "Mechanism/Button.hpp"
#include "Character/Character.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Platform.hpp"
#include "Util/Image.hpp"

Button::Button(ButtonColor color, const glm::vec2 &pos)
    : MechanismBase(pos, color, 25.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
  SetPosition(pos);
  SetPivot({0.0f, 0.0f});
}

std::string Button::GetImagePath(ButtonColor color) {
  std::string base = RESOURCE_DIR "/material/props/button/button-";
  switch (color) {
  case Color::BLUE:
    return base + "blue.png";
  case Color::GREEN:
    return base + "green.png";
  case Color::ORANGE:
    return base + "orange.png";
  case Color::PINK:
    return base + "pink.png";
  case Color::WHITE:
    return base + "white.png";
  case Color::YELLOW:
    return base + "yellow.png";
  default:
    return base + "white.png";
  }
}

void Button::Update(Character *fb, Character *wg) {
  auto rect = getRect();

  bool fbOn = SDL_HasIntersection(&fb->getRect(), &rect);
  bool wgOn = SDL_HasIntersection(&wg->getRect(), &rect);

  bool nowPressed = fbOn || wgOn;
  bool wasPressed = m_IsPressedFireboy || m_IsPressedWatergirl;

  if (nowPressed != wasPressed) {
    for (auto *t : m_Triggers) {
      if (auto *gate = dynamic_cast<Gate *>(t)) {
        if (gate->GetColor() == m_Color) {
          if (nowPressed)
            gate->OnTriggered();
          else
            gate->OnReleased();
        }
      } else if (auto *platform = dynamic_cast<Platform *>(t)) {
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

void Button::LinkTrigger(ITriggerable *target) { m_Triggers.push_back(target); }

ButtonColor Button::GetColor() const { return m_Color; }

const SDL_Rect &Button::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize() * 0.5f; // 可依實際按鈕大小微調縮放
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}