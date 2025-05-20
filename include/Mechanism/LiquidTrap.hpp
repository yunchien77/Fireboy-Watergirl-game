#ifndef LIQUID_TRAP_HPP
#define LIQUID_TRAP_HPP

#include "Character/Watergirl.hpp"
#include "Character/Fireboy.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/GridSystem.hpp"

enum class SizeType { SMALL, LARGE };

class LiquidTrap : public MechanismBase {
public:
  explicit LiquidTrap(CellType type, SizeType sizeType = SizeType::LARGE);

  void OnCharacterEnter(Character *character);
  void SetImage(const std::string &imagePath);
  void SetInitialState(const glm::vec2 &position);
  void Update() override {}

protected:
  std::string m_ImagePath;

private:
  CellType type;
  SizeType sizeType;
  glm::vec2 m_InitialPosition;
};

#endif
