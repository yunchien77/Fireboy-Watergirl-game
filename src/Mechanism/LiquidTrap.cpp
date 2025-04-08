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
  this->SetZIndex(20);

  SetPivot(glm::vec2(0.0f, -5.0f));
}

void LiquidTrap::SetImage(const std::string &imagePath) {
  m_ImagePath = imagePath;
  SetDrawable(std::make_shared<Util::Image>(imagePath));
}

void LiquidTrap::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
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