// Level2.cpp
#include "Level/Level2.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"

bool Level2::Initialize() {
  // 初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(2, 28);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Fireboy->SetSpawnPoint(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(4, 28);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(2, 4);
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(6, 4);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  // 水池座標列表
  std::vector<std::pair<int, int>> waterTrapCoords = {{10, 25}, {26, 28}};

  for (const auto &[row, col] : waterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  // 岩漿座標列表
  std::vector<std::pair<int, int>> lavaTrapCoords = {{10, 28}, {26, 25}};

  for (const auto &[row, col] : lavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  // 毒池座標列表
  std::vector<std::pair<int, int>> poisionTrapCoords = {{12, 15}, {27, 15}};

  for (const auto &[row, col] : poisionTrapCoords) {
    auto poisionTrap =
        std::make_shared<LiquidTrap>(CellType::POISON, SizeType::LARGE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    poisionTrap->SetPosition(pos);
    m_Traps.push_back(poisionTrap);
    m_Root.AddChild(poisionTrap);
  }

  // 火寶石座標 (row, col)
  std::vector<std::pair<int, int>> fireGemCoords = {
      {8, 27},  {12, 27}, {24, 24}, {28, 24},
      {28, 17}, {15, 17}, {18, 13}, {17, 3}};

  for (const auto &[row, col] : fireGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::FIRE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  // 水寶石座標 (row, col)
  std::vector<std::pair<int, int>> waterGemCoords = {
      {8, 24},  {12, 24}, {24, 27}, {28, 27},
      {24, 17}, {10, 17}, {21, 13}, {20, 3}};

  for (const auto &[row, col] : waterGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  // 初始化 Gate
  glm::ivec2 gateCell = {20, 17};
  glm::vec2 gatePos = m_GridSystem.CellToGamePosition(gateCell.x, gateCell.y);
  gatePos.y -= 13.0f;
  auto gate = std::make_shared<Gate>(GateColor::WHITE, gatePos);
  m_Triggers.push_back(gate);
  m_Root.AddChild(gate);

  // 按鈕座標列表
  std::vector<glm::ivec2> buttonCells = {{6, 20}, {33, 20}};
  for (const auto &cell : buttonCells) {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(cell.x, cell.y);
    pos.y += 20;
    auto button = std::make_shared<Button>(ButtonColor::WHITE, pos);
    button->SetInitialState(pos);
    button->linkTrigger(gate.get());
    m_Buttons.push_back(button);
    m_Root.AddChild(button);
  }

  return true;
}