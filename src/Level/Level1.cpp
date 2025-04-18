// Level1.cpp
#include "Level/Level1.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"


bool Level1::Initialize() {
  // 初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(35, 5);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Fireboy->SetSpawnPoint(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(3, 17);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(32, 14);
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(4, 27);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  // 水池座標列表
  std::vector<std::pair<int, int>> waterTrapCoords = {
      {27, 7}, {22, 7}, {17, 7}, {19, 12}};

  for (const auto &[row, col] : waterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  // 岩漿座標列表
  std::vector<std::pair<int, int>> lavaTrapCoords = {
      {19, 19}, {14, 19}, {9, 19}, {28, 22}};

  for (const auto &[row, col] : lavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  // 火寶石座標列表
  std::vector<std::pair<int, int>> fireGemCoords = {
      {24, 6}, {19, 6}, {14, 6}, {3, 10}};

  for (const auto &[row, col] : fireGemCoords) {
    auto fireGem = std::make_shared<Gem>(GemType::FIRE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    fireGem->SetPosition(pos);
    fireGem->SetInitialPosition(pos);
    m_Gems.push_back(fireGem);
    m_Root.AddChild(fireGem);
  }

  // 水寶石座標列表
  std::vector<std::pair<int, int>> waterGemCoords = {
      {11, 18}, {16, 18}, {21, 18}, {34, 22}};

  for (const auto &[row, col] : waterGemCoords) {
    auto waterGem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    waterGem->SetPosition(pos);
    waterGem->SetInitialPosition(pos);
    m_Gems.push_back(waterGem);
    m_Root.AddChild(waterGem);
  }

  return true;
}