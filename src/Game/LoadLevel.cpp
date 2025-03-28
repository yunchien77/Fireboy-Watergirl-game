#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Object/GridSystem.hpp"
#include <Mechanism/Gem.hpp>
#include <Mechanism/LiquidTrap.hpp>

// 載入地圖網格，並初始化 GridSystem
bool App::LoadLevelGrid(int levelNumber) {
  for (auto &gem : m_Gems) {
    m_Root.RemoveChild(gem);
  }
  m_Gems.clear();

  std::string gridFilePath =
      RESOURCE_DIR "/map/level" + std::to_string(levelNumber) + "_grid.txt";

  bool success = m_GridSystem.LoadFromFile(gridFilePath);
  if (success) {
    m_IsGridLoaded = true;
    LOG_INFO("Successfully loaded grid for level {}", levelNumber);
  }

  switch (levelNumber) {
  case 1: {
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

    // 水池
    for (auto &trap : m_Traps) {
      trap->SetVisible(true);
    }
    auto water1 =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 waterPos1 = m_GridSystem.CellToGamePosition(27, 7);
    waterPos1.y += 5.0f;
    water1->SetPosition(waterPos1);
    m_Traps.push_back(water1);
    m_Root.AddChild(water1);

    // 水池
    auto water2 =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 waterPos2 = m_GridSystem.CellToGamePosition(22, 7);
    waterPos2.y += 5.0f;
    water2->SetPosition(waterPos2);
    m_Traps.push_back(water2);
    m_Root.AddChild(water2);

    // 水池
    auto water3 =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 waterPos3 = m_GridSystem.CellToGamePosition(17, 7);
    waterPos3.y += 5.0f;
    water3->SetPosition(waterPos3);
    m_Traps.push_back(water3);
    m_Root.AddChild(water3);

    // 水池
    auto water4 =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 waterPos4 = m_GridSystem.CellToGamePosition(19, 12);
    waterPos4.y += 5.0f;
    water4->SetPosition(waterPos4);
    m_Traps.push_back(water4);
    m_Root.AddChild(water4);

    // 岩漿
    auto lava1 = std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 lavaPos1 = m_GridSystem.CellToGamePosition(19, 19);
    lavaPos1.y += 3.0f;
    lava1->SetPosition(lavaPos1);
    m_Traps.push_back(lava1);
    m_Root.AddChild(lava1);

    // 岩漿
    auto lava2 = std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 lavaPos2 = m_GridSystem.CellToGamePosition(14, 19);
    lavaPos2.y += 3.0f;
    lava2->SetPosition(lavaPos2);
    m_Traps.push_back(lava2);
    m_Root.AddChild(lava2);

    // 岩漿
    auto lava3 = std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 lavaPos3 = m_GridSystem.CellToGamePosition(9, 19);
    lavaPos3.y += 3.0f;
    lava3->SetPosition(lavaPos3);
    m_Traps.push_back(lava3);
    m_Root.AddChild(lava3);

    // 岩漿
    auto lava4 = std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 lavaPos4 = m_GridSystem.CellToGamePosition(28, 22);
    lavaPos4.y += 6.0f;
    lava4->SetPosition(lavaPos4);
    m_Traps.push_back(lava4);
    m_Root.AddChild(lava4);

    // 火寶石座標列表
    std::vector<std::pair<int, int>> fireGemCoords = {
        {24, 5}, {19, 5}, {14, 5}};

    for (const auto &[row, col] : fireGemCoords) {
      auto fireGem = std::make_shared<Gem>(GemType::FIRE);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
      pos.x += 13.0f;
      fireGem->SetPosition(pos);
      fireGem->SetInitialPosition(pos);
      m_Gems.push_back(fireGem);
      m_Root.AddChild(fireGem);
    }

    // 水寶石座標列表
    std::vector<std::pair<int, int>> waterGemCoords = {
        {11, 17}, {16, 17}, {21, 17}};

    for (const auto &[row, col] : waterGemCoords) {
      auto waterGem = std::make_shared<Gem>(GemType::WATER);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
      pos.x += 13.0f;
      waterGem->SetPosition(pos);
      waterGem->SetInitialPosition(pos);
      m_Gems.push_back(waterGem);
      m_Root.AddChild(waterGem);
    }

  } break;

  case 2: {
    // 初始化角色 Fireboy
    if (!m_Fireboy) {
      m_Fireboy = std::make_shared<Fireboy>();
      glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(2, 27);
      m_Fireboy->SetPosition(fireboyInitPos);
      m_Fireboy->SetSpawnPoint(fireboyInitPos);
      m_Root.AddChild(m_Fireboy);
    }

    // 初始化角色 Watergirl
    if (!m_Watergirl) {
      m_Watergirl = std::make_shared<Watergirl>();
      glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(4, 27);
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

    // 火寶石座標 (row, col)
    std::vector<std::pair<int, int>> fireGemCoords = {
        {8, 27},  {12, 27}, {24, 24}, {28, 24},
        {28, 18}, {15, 18}, {18, 13}, {17, 3}};

    for (const auto &[row, col] : fireGemCoords) {
      auto gem = std::make_shared<Gem>(GemType::FIRE);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
      pos.y += 13.0f;
      gem->SetPosition(pos);
      gem->SetInitialPosition(pos);
      m_Gems.push_back(gem);
      m_Root.AddChild(gem);
    }

    // 水寶石座標 (row, col)
    std::vector<std::pair<int, int>> waterGemCoords = {
        {8, 24},  {12, 24}, {24, 27}, {28, 27},
        {24, 18}, {10, 18}, {21, 13}, {20, 3}};

    for (const auto &[row, col] : waterGemCoords) {
      auto gem = std::make_shared<Gem>(GemType::WATER);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
      pos.y += 13.0f;
      gem->SetPosition(pos);
      gem->SetInitialPosition(pos);
      m_Gems.push_back(gem);
      m_Root.AddChild(gem);
    }

  } break;
  case 3:
  case 4:
  case 5:
    break;
  default:
    LOG_ERROR("Invalid level number: {}", levelNumber);
    return false;
  }

  for (int y = 0; y < m_GridSystem.GetGridHeight(); ++y) {
    for (int x = 0; x < m_GridSystem.GetGridWidth(); ++x) {
      std::cout << static_cast<int>(m_GridSystem.GetCell(x, y)) << " ";
    }
    std::cout << std::endl;
  }

  return true;
}