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

void Lever::OnCharacterEnter(Character *character) {
  if (character == nullptr) {
    return;
  }

  // Add character to interacting list if not already present
  if (std::find(m_InteractingCharacters.begin(), m_InteractingCharacters.end(),
                character) == m_InteractingCharacters.end()) {
    m_InteractingCharacters.push_back(character);
  }

  // Check if we should toggle the lever
  if (ShouldToggle(character)) {
    Toggle();
  }
}

// void Lever::OnCharacterExit(Character *character) {
//   if (character == nullptr) {
//     return;
//   }

//   // Remove character from interacting list
//   auto it = std::find(m_InteractingCharacters.begin(),
//                       m_InteractingCharacters.end(), character);
//   if (it != m_InteractingCharacters.end()) {
//     m_InteractingCharacters.erase(it);
//   }
// }

bool Lever::ShouldToggle(Character *character) const {
  // Check character's facing direction
  bool characterFacingRight = character->IsFacingRight();

  // !m_IsOn: 桿子向右(off), m_IsOn: 桿子向左(on)
  // Only toggle when character is facing opposite to lever direction
  if (!characterFacingRight && !m_IsOn) {
    // Character facing left, lever right(off), directions opposite, trigger
    return true;
  } else if (characterFacingRight && m_IsOn) {
    // Character facing right, lever left(on), directions opposite, trigger
    return true;
  }

  return false;
}

void Lever::update(Character *fb, Character *wg) {
  // This method is kept for backward compatibility
  // It could be refactored to use the collision detection system
  // For now, we'll keep the old implementation
  auto rect = getRect();

  // Check Fireboy
  if (SDL_HasIntersection(&fb->getRect(), &rect)) {
    bool characterFacingRight = fb->IsFacingRight();

    if (!characterFacingRight && !m_IsOn) {
      Toggle();
      return;
    } else if (characterFacingRight && m_IsOn) {
      Toggle();
      return;
    }
  }

  // Check Watergirl
  if (SDL_HasIntersection(&wg->getRect(), &rect)) {
    bool characterFacingRight = wg->IsFacingRight();

    if (!characterFacingRight && !m_IsOn) {
      Toggle();
      return;
    } else if (characterFacingRight && m_IsOn) {
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