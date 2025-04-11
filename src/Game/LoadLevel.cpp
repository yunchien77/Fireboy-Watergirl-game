#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Gate.hpp"
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
        {24, 5}, {19, 5}, {14, 5}, {3, 10}};

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
        {11, 17}, {16, 17}, {21, 17}, {34, 22}};

    for (const auto &[row, col] : waterGemCoords) {
      auto waterGem = std::make_shared<Gem>(GemType::WATER);
      glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
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
        {28, 18}, {15, 18}, {18, 13}, {17, 3}};

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
        {24, 18}, {10, 18}, {21, 13}, {20, 3}};

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
    auto gate = std::make_shared<Gate>(GateColor::WHITE, gatePos);
    m_Triggers.push_back(gate);
    m_Root.AddChild(gate);

    // 按鈕座標列表
    std::vector<glm::ivec2> buttonCells = {{6, 20}, {33, 20}};
    for (const auto &cell : buttonCells) {
      glm::vec2 pos = m_GridSystem.CellToGamePosition(cell.x, cell.y);
      auto button = std::make_shared<Button>(ButtonColor::WHITE, pos);
      button->linkTrigger(gate.get());
      m_Buttons.push_back(button);
      m_Root.AddChild(button);
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