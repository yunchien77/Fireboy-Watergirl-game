// Level2.cpp
#include "Level/Level2.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"

bool Level2::Initialize() {
  // 初始化 Character 和 Door
  if (!m_Fireboy)
    m_Fireboy = std::make_shared<Fireboy>();
  InitCharacter(m_Fireboy, {2, 28});

  if (!m_Watergirl)
    m_Watergirl = std::make_shared<Watergirl>();
  InitCharacter(m_Watergirl, {4, 28});

  InitDoor(m_Fireboy_Door, {2, 4});
  InitDoor(m_Watergirl_Door, {6, 4});

  // 初始化Trap
  InitLiquidTraps(CellType::WATER, SizeType::LARGE, {{10, 25}, {26, 28}});
  InitLiquidTraps(CellType::LAVA, SizeType::LARGE, {{10, 28}, {26, 25}});
  InitLiquidTraps(CellType::POISON, SizeType::LARGE, {{12, 15}, {27, 15}});

  // 初始化Gem
  InitGems(GemType::FIRE, {
    {8, 27}, {12, 27}, {24, 24}, {28, 24},
    {28, 17}, {15, 17}, {18, 13}, {17, 3}
  });

  InitGems(GemType::WATER, {
    {8, 24}, {12, 24}, {24, 27}, {28, 27},
    {24, 17}, {10, 17}, {21, 13}, {20, 3}
  });

  // 初始化 Gate 和 Button
  auto gate = InitGate(GateColor::WHITE, {20, 17});

  InitButton(ButtonColor::WHITE, {6, 20}, gate.get());
  InitButton(ButtonColor::WHITE, {33, 20}, gate.get());

  return true;
}