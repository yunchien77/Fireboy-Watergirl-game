#include "Level/Level4.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level4::Initialize() {
  //   初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(17, 5);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Fireboy->SetSpawnPoint(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(19, 5);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(35, 19);
  fireboyDoorPos.x += 7.0f;
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(1, 24);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  // 火寶石座標
  std::vector<std::pair<int, int>> fireGemCoords = {
      {8, 3}, {23, 6}, {24, 12}, {13, 23}, {15, 16}, {36, 3}};

  for (const auto &[row, col] : fireGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::FIRE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  // 水寶石座標
  std::vector<std::pair<int, int>> waterGemCoords = {
      {34, 14}, {29, 25}, {8, 11}, {26, 18}, {29, 8}, {11, 6}};

  for (const auto &[row, col] : waterGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  // 水池座標列表
  std::vector<std::pair<int, int>> LargewaterTrapCoords = {{11, 7}};

  for (const auto &[row, col] : LargewaterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  std::vector<std::pair<int, int>> SmallwaterTrapCoords = {
      {34, 15}, {7, 17}, {12, 17}};

  for (const auto &[row, col] : SmallwaterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 3.0f;
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  // 岩漿座標列表
  std::vector<std::pair<int, int>> LargelavaTrapCoords = {{7, 4}};

  for (const auto &[row, col] : LargelavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y += 5.0f;
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  std::vector<std::pair<int, int>> SmalllavaTrapCoords = {{21, 7}, {25, 7}};

  for (const auto &[row, col] : SmalllavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  // 毒池座標列表
  std::vector<std::pair<int, int>> LargepoisionTrapCoords = {
      {8, 27}, {17, 27}, {26, 27}};

  for (const auto &[row, col] : LargepoisionTrapCoords) {
    auto poisionTrap =
        std::make_shared<LiquidTrap>(CellType::POISON, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    poisionTrap->SetPosition(pos);
    m_Traps.push_back(poisionTrap);
    m_Root.AddChild(poisionTrap);
  }

  std::vector<std::pair<int, int>> SmallpoisionTrapCoords = {{32, 27},
                                                             {35, 27}};

  for (const auto &[row, col] : SmallpoisionTrapCoords) {
    auto poisionTrap =
        std::make_shared<LiquidTrap>(CellType::POISON, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 3.0f;
    poisionTrap->SetPosition(pos);
    m_Traps.push_back(poisionTrap);
    m_Root.AddChild(poisionTrap);
  }

  // yellow lever
  glm::vec2 yellowLeverPos = m_GridSystem.CellToGamePosition(35, 5);
  auto yellowLever =
      std::make_shared<Lever>(LeverColor::YELLOW, yellowLeverPos);
  yellowLever->SetPosition(yellowLeverPos);
  yellowLever->SetInitialState(yellowLeverPos, false);
  m_Levers.push_back(yellowLever);
  m_Root.AddChild(yellowLever);

  // yellow gate
  glm::vec2 yellowGatePos = m_GridSystem.CellToGamePosition(12, 2);
  auto yellowGate = std::make_shared<Gate>(GateColor::YELLOW, yellowGatePos);
  yellowGate->SetPosition(yellowGatePos);
  yellowGate->SetInitialState(yellowGatePos, false);
  m_Triggers.push_back(yellowGate);
  m_Root.AddChild(yellowGate);

  // link yellow lever to yellow gate
  yellowLever->linkTrigger(yellowGate.get());

  // blue lever
  glm::vec2 blueLeverPos = m_GridSystem.CellToGamePosition(10, 11);
  auto blueLever = std::make_shared<Lever>(LeverColor::BLUE, blueLeverPos);
  blueLever->SetPosition(blueLeverPos);
  blueLever->SetInitialState(blueLeverPos, false);
  m_Levers.push_back(blueLever);
  m_Root.AddChild(blueLever);

  // blue platform
  glm::vec2 bluePlatformPos = m_GridSystem.CellToGamePosition(33, 9);
  bluePlatformPos.x += 12.0f;
  auto bluePlatform = std::make_shared<Platform>(
      PlatformColor::BLUE, bluePlatformPos, glm::vec2(80, 0));
  bluePlatform->SetPosition(bluePlatformPos);
  m_Platforms.push_back(bluePlatform);
  m_Root.AddChild(bluePlatform);

  // link blue lever to blue platform
  blueLever->linkTrigger(bluePlatform.get());

  return true;
}
