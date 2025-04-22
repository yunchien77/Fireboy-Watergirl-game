// Level3.cpp
#include "Level/Level3.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level3::Initialize() {
  //   初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(3, 28);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Fireboy->SetSpawnPoint(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(3, 24);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(30, 5);
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(34, 5);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  std::vector<std::pair<int, int>> waterTrapCoords = {{28, 28}};

  for (const auto &[row, col] : waterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  // 岩漿座標列表
  std::vector<std::pair<int, int>> lavaTrapCoords = {{20, 28}};

  for (const auto &[row, col] : lavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  // 毒池座標列表
  std::vector<std::pair<int, int>> poisionTrapCoords = {{26, 22}};

  for (const auto &[row, col] : poisionTrapCoords) {
    auto poisionTrap =
        std::make_shared<LiquidTrap>(CellType::POISON, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    poisionTrap->SetPosition(pos);
    m_Traps.push_back(poisionTrap);
    m_Root.AddChild(poisionTrap);
  }

  // 火寶石座標 (row, col)
  std::vector<std::pair<int, int>> fireGemCoords = {
    {20, 26}, {7, 14}, {12, 3}};

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
    {28, 26}, {23, 15}, {2, 5}};

  for (const auto &[row, col] : waterGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  std::vector<std::pair<int, int>> leverCoords = {
    {6, 20}, {12, 12}, {25, 8}
  };

  for (const auto &[row, col] : leverCoords) {
    auto lever = std::make_shared<Lever>(LeverColor::GREEN, m_GridSystem.CellToGamePosition(row, col));
    m_Levers.push_back(lever);
    m_Root.AddChild(lever);
  }

  std::vector<std::tuple<int, int, int, int>> platformInfos = {
    {15, 10, 0, -100},  // row, col, dx, dy
    {20, 5,  0, -80}
  };

  for (const auto &[row, col, dx, dy] : platformInfos) {
    auto platform = std::make_shared<Platform>(
        PlatformColor::GREEN,
        m_GridSystem.CellToGamePosition(row, col),
        glm::vec2(dx, dy)
    );
    m_Platforms.push_back(platform);
    m_Root.AddChild(platform);
  }

  return true;
}
