// Level4.cpp
#include "Level/Level4.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level4::Initialize() {
  // 初始化 Character 和 Door
  if (!m_Fireboy)
    m_Fireboy = std::make_shared<Fireboy>();
  InitCharacter(m_Fireboy, {17, 5});

  if (!m_Watergirl)
    m_Watergirl = std::make_shared<Watergirl>();
  InitCharacter(m_Watergirl, {19, 5});

  InitDoor(m_Fireboy_Door, {35, 19});
  InitDoor(m_Watergirl_Door, {1, 24});

  // 初始化Gem
  InitGems(GemType::FIRE,
           {{8, 3}, {23, 6}, {24, 12}, {13, 23}, {15, 16}, {36, 3}});
  InitGems(GemType::WATER,
           {{34, 14}, {29, 23}, {8, 11}, {26, 18}, {29, 8}, {11, 6}});

  // 初始化Trap
  InitLiquidTraps(CellType::WATER, SizeType::LARGE, {{11, 7}});
  InitLiquidTraps(CellType::WATER, SizeType::SMALL,
                  {{34, 15}, {7, 17}, {12, 17}});
  InitLiquidTraps(CellType::LAVA, SizeType::LARGE, {{7, 4}});
  InitLiquidTraps(CellType::LAVA, SizeType::SMALL, {{21, 7}, {25, 7}});
  InitLiquidTraps(CellType::POISON, SizeType::LARGE,
                  {{8, 27}, {17, 27}, {26, 27}});
  InitLiquidTraps(CellType::POISON, SizeType::SMALL, {{32, 27}, {35, 27}});

  // 機關連動
  auto yellowGate = InitGate(GateColor::YELLOW, {12, 1});
  InitLever(LeverColor::YELLOW, {35, 5}, yellowGate.get());

  auto bluePlatform = InitPlatform(PlatformColor::BLUE, {33, 9}, {80, 0});
  InitLever(LeverColor::BLUE, {10, 11}, bluePlatform.get());

  return true;
}
