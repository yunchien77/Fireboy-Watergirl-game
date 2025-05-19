// Level3.cpp
#include "Level/Level3.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level3::Initialize() {
  // 初始化 Character 和 door
  if (!m_Fireboy)
    m_Fireboy = std::make_shared<Fireboy>();
  InitCharacter(m_Fireboy, {3, 28});

  if (!m_Watergirl)
    m_Watergirl = std::make_shared<Watergirl>();
  InitCharacter(m_Watergirl, {3, 24});

  InitDoor(m_Fireboy_Door, {30, 5});
  InitDoor(m_Watergirl_Door, {34, 5});

  // 初始化Trap
  InitLiquidTraps(CellType::WATER, SizeType::SMALL, {{28, 28}});
  InitLiquidTraps(CellType::LAVA, SizeType::SMALL, {{20, 28}});
  InitLiquidTraps(CellType::POISON, SizeType::SMALL, {{26, 22}});

  // 初始化Gem
  InitGems(GemType::FIRE, {{20, 26}, {7, 14}, {12, 3}});
  InitGems(GemType::WATER, {{28, 26}, {23, 15}, {2, 5}});

  // 機關連動
  auto greenPlatform = InitPlatform(PlatformColor::GREEN, {2, 15}, {0, -80});
  InitLever(LeverColor::GREEN, {10, 19}, greenPlatform.get());

  auto pinkPlatform = InitPlatform(PlatformColor::PINK, {35, 12}, {0, -80});
  InitButton(ButtonColor::PINK, {15, 15}, pinkPlatform.get());
  InitButton(ButtonColor::PINK, {31, 11}, pinkPlatform.get());

  // Box
  InitBoxes({{24, 9}});

  // 設定platform與box給character
  m_Fireboy->SetPlatforms(m_Platforms);
  m_Watergirl->SetPlatforms(m_Platforms);
  m_Fireboy->SetBoxes(m_Boxes);
  m_Watergirl->SetBoxes(m_Boxes);

  return true;
}