#include "Mechanism/Gem.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Util/Image.hpp"
#include <iostream>

Gem::Gem(GemType type) : m_Type(type) {
  std::string imagePath;
  switch (type) {
  case GemType::FIRE:
    imagePath = RESOURCE_DIR "/material/props/gems/gem-red.png";
    break;
  case GemType::WATER:
    imagePath = RESOURCE_DIR "/material/props/gems/gem-blue.png";
    break;
  case GemType::GREEN:
    imagePath = RESOURCE_DIR "/material/props/gems/gem-green.png";
    break;
  }

  SetDrawable(std::make_shared<Util::Image>(imagePath));
  this->SetZIndex(20);
}

void Gem::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

GemType Gem::GetType() const { return m_Type; }

void Gem::OnCharacterEnter(Character *character) {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  SDL_Rect rect = {static_cast<int>(pos.x - size.x / 2),
                   static_cast<int>(pos.y - size.y / 2),
                   static_cast<int>(size.x), static_cast<int>(size.y)};

  if (SDL_HasIntersection(&rect, &character->getRect())) {
    if (m_Type == GemType::FIRE &&
        dynamic_cast<Fireboy *>(character) == nullptr) {
      return;
    }

    if (m_Type == GemType::WATER &&
        dynamic_cast<Watergirl *>(character) == nullptr) {
      return;
    }

    SetVisible(false);
  }
}

const SDL_Rect &Gem::getRect() const {
  float shrinkFactor = 0.4f;
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize() * shrinkFactor;
  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);
  return m_Rect;
}

bool Gem::IsCollected() const { return !m_Visible; }

void Gem::Respawn() {
  SetPosition(m_InitialPosition);
  SetVisible(true);
}

void Gem::SetInitialPosition(const glm::vec2 &pos) { m_InitialPosition = pos; }
