#include "Level/Level5.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level5::Initialize() {
  //   初始化角色 Fireboy
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
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(35, 28);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(11, 17);
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(25, 17);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  // 岩漿和水池
  std::vector<std::tuple<CellType, SizeType, std::vector<std::pair<int, int>>>>
      trapData = {{CellType::WATER, SizeType::LARGE, {{8, 22}, {30, 28}}},
                  {CellType::WATER, SizeType::SMALL, {{27, 13}}},
                  {CellType::LAVA, SizeType::LARGE, {{8, 28}, {30, 22}}},
                  {CellType::LAVA, SizeType::SMALL, {{11, 13}}}};

  for (const auto &[type, size, coords] : trapData) {
    for (const auto &[row, col] : coords) {
      auto trap = std::make_shared<LiquidTrap>(type, size);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
      trap->SetPosition(pos);
      m_Traps.push_back(trap);
      m_Root.AddChild(trap);
    }
  }

  // 火寶石座標
  std::vector<std::pair<int, int>> fireGemCoords = {
      {7, 27},  {10, 27}, {14, 25}, {28, 21}, {31, 21},
      {11, 12}, {36, 5},  {36, 8},  {36, 11}, {22, 10}};

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
      {28, 27}, {31, 27}, {24, 25}, {7, 21}, {10, 21},
      {27, 12}, {2, 5},   {2, 8},   {2, 11}, {16, 10}};

  for (const auto &[row, col] : waterGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  std::vector<std::tuple<int, int, float, FanColor>> fanInfos = {
      {2, 14, 150.0f, FanColor::WHITE}, // col, row, strength, color
      {36, 14, 150.0f, FanColor::WHITE} // col, row, strength, color
  };

  for (const auto &[col, row, strength, color] : fanInfos) {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(col, row);

    auto fan = std::make_shared<Fan>(pos, strength, color);
    fan->SetPosition(pos);
    fan->SetInitialPosition(pos);
    m_Fans.push_back(fan);
    m_Root.AddChild(fan);
  }

  return true;
}
