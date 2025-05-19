#ifndef LIQUID_TRAP_HPP
#define LIQUID_TRAP_HPP

#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/ICollidable.hpp"
#include "Interface/ITrap.hpp"
#include "Object/GridSystem.hpp"


enum class SizeType { SMALL, MIDDLE, LARGE };

class LiquidTrap : public ITrap, public ICollidable {
public:
  explicit LiquidTrap(CellType type, SizeType sizeType = SizeType::LARGE);

  // ITrap interface
  void OnCharacterEnter(Character *character) override;

  // ICollidable interface
  const SDL_Rect &getRect() const override;

  // LiquidTrap specific methods
  void SetImage(const std::string &imagePath);
  void SetPosition(const glm::vec2 &position);

protected:
  std::string m_ImagePath;

private:
  CellType type;
  SizeType sizeType;
  mutable SDL_Rect m_Rect{};
};

#endif