#include "Mechanism/LiquidTrap.hpp"
#include <iostream>

LiquidTrap::LiquidTrap(CellType type, SizeType sizeType)
    : type(type), sizeType(sizeType) {

  std::string imagePath;

  switch (type) {
  case CellType::LAVA:
    imagePath = (sizeType == SizeType::LARGE)
                    ? RESOURCE_DIR "/material/props/lava/lava-red-big.png"
                    : RESOURCE_DIR "/material/props/lava/lava-red-small.png";
    break;
  case CellType::WATER:
    imagePath = (sizeType == SizeType::LARGE)
                    ? RESOURCE_DIR "/material/props/lava/lava-blue-big.png"
                    : RESOURCE_DIR "/material/props/lava/lava-blue-small.png";
    break;
  case CellType::POISON:
    imagePath = (sizeType == SizeType::LARGE)
                    ? RESOURCE_DIR "/material/props/lava/lava-green-big.png"
                    : RESOURCE_DIR "/material/props/lava/lava-green-small.png";
    break;
  default:
    std::cerr << "⚠️ Warning: Unknown CellType (" << static_cast<int>(type)
              << ")\n";
    break;
  }
  SetImage(imagePath);
  this->SetZIndex(21);

  SetPivot(glm::vec2(0.0f, -3.5f));
}

void LiquidTrap::SetImage(const std::string &imagePath) {
  m_ImagePath = imagePath;
  SetDrawable(std::make_shared<Util::Image>(imagePath));
}

void LiquidTrap::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

const SDL_Rect &LiquidTrap::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);

  return m_Rect;
}

void LiquidTrap::OnCharacterEnter(Character *character) {
  if (character == nullptr) {
    return;
  }

  switch (type) {
  case CellType::LAVA:
    if (dynamic_cast<Watergirl *>(character)) {
      character->Die();
    }
    break;
  case CellType::WATER:
    if (dynamic_cast<Fireboy *>(character)) {
      character->Die();
    }
    break;
  case CellType::POISON:
    character->Die();
    break;
  default:;
  }
}