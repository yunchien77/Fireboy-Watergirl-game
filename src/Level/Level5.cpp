// Level5.cpp
#include "Level/Level5.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"

bool Level5::Initialize() {
  // 初始化 Character 和 Door
  if (!m_Fireboy)
    m_Fireboy = std::make_shared<Fireboy>();
  InitCharacter(m_Fireboy, {2, 28});

  if (!m_Watergirl)
    m_Watergirl = std::make_shared<Watergirl>();
  InitCharacter(m_Watergirl, {35, 28});

  InitDoor(m_Fireboy_Door, {11, 17});
  InitDoor(m_Watergirl_Door, {25, 17});

  // 初始化Trap
  InitLiquidTraps(CellType::WATER, SizeType::LARGE, {{8, 22}, {30, 28}});
  InitLiquidTraps(CellType::WATER, SizeType::SMALL, {{27, 13}});
  InitLiquidTraps(CellType::LAVA, SizeType::LARGE, {{8, 28}, {30, 22}});
  InitLiquidTraps(CellType::LAVA, SizeType::SMALL, {{11, 13}});

  // 初始化Gem
  InitGems(GemType::FIRE, {
    {7, 27}, {10, 27}, {14, 25}, {28, 21}, {31, 21},
    {11, 12}, {36, 5}, {36, 8}, {36, 11}, {22, 10}
  });

  InitGems(GemType::WATER, {
    {28, 27}, {31, 27}, {24, 25}, {7, 21}, {10, 21},
    {27, 12}, {2, 5}, {2, 8}, {2, 11}, {16, 10}
  });

  // 初始化Fan
  InitFans({
    {2, 14, 150.0f, FanColor::WHITE},
    {36, 14, 150.0f, FanColor::WHITE}
  });

  return true;
}