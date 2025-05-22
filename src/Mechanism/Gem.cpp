#include "Mechanism/Gem.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Util/Image.hpp"

Gem::Gem(GemType type, const glm::vec2 &pos)
    : MechanismBase(pos, Color::NONE, 20.0f), m_Type(type) {
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
  SetPosition(pos);
  SetPivot(glm::vec2(0.0f, -13.0f));
}

GemType Gem::GetType() const { return m_Type; }

void Gem::OnCharacterEnter(Character *character) {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  SDL_Rect rect = {static_cast<int>(pos.x - size.x / 2),
                   static_cast<int>(pos.y - size.y / 2),
                   static_cast<int>(size.x), static_cast<int>(size.y)};

  if (SDL_HasIntersection(&rect, &character->getRect())) {
    if (m_Type == GemType::FIRE && !dynamic_cast<Fireboy *>(character)) return;
    if (m_Type == GemType::WATER && !dynamic_cast<Watergirl *>(character)) return;

    SetVisible(false);
  }
}

bool Gem::IsCollected() const { return !m_Visible; }
