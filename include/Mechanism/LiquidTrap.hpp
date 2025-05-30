#ifndef LIQUID_TRAP_HPP
#define LIQUID_TRAP_HPP

#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/GridSystem.hpp"

enum class SizeType { SMALL, LARGE };

class LiquidTrap : public MechanismBase {
public:
  explicit LiquidTrap(CellType type, SizeType sizeType = SizeType::LARGE);

  void OnCharacterEnter(Character *character);
  void SetImage(const std::string &imagePath);
  void SetInitialState(const glm::vec2 &position);

protected:
  std::string m_ImagePath;

private:
  CellType type;
  SizeType sizeType;
};

#endif
