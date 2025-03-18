#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "LevelData.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>

bool App::CheckCharacterCollision(const glm::vec2 &position, bool isFireboy) {
  if (!m_IsGridLoaded) {
    return false; // 如果網格未載入，默認無碰撞
  }
  return m_GridSystem.CheckCollision(position, isFireboy);
}


// 載入地圖網格，並初始化 GridSystem
bool App::LoadLevelGrid(int levelNumber) {
  /*std::string gridFilePath =
      RESOURCE_DIR "/map/level" + std::to_string(levelNumber) + "_grid.txt";

  // 嘗試載入格子數據
  bool success = m_GridSystem.LoadFromFile(gridFilePath);

  if (success) {
    m_IsGridLoaded = true;
    LOG_INFO("Successfully loaded grid for level {}", levelNumber);
  } else {
    LOG_ERROR("Failed to load grid for level {}", levelNumber);
    // 如果沒有格子檔案，創建一個預設的空網格系統
    m_GridSystem = GridSystem();
    m_IsGridLoaded = false;
  }

  return success;
  */
  switch (levelNumber) {
    case 1:
      m_GridSystem = GridSystem(level1);
    break;
    case 2: // 先讓 level2 ~ level5 預設為 level1，等你完成後再改回來
    case 3:
    case 4:
    case 5:
      m_GridSystem = GridSystem(level1);
    break;
    default:
      LOG_ERROR("Invalid level number: {}", levelNumber);
    return false; }

  CellType type = m_GridSystem.GetCell(10, 10);
  std::cout << "Cell at (10,10) is: " << static_cast<int>(type) << std::endl;

  m_IsGridLoaded = true;
  LOG_INFO("Successfully loaded level {}", levelNumber);

  for (int y = 0; y < m_GridSystem.GetGridHeight(); ++y) {
    for (int x = 0; x < m_GridSystem.GetGridWidth(); ++x) {
      std::cout << static_cast<int>(m_GridSystem.GetCell(x, y)) << " ";
    }
    std::cout << std::endl;
  }

  return true;
}

// 限制玩家在地圖邊界內
void RestrictPlayerPosition(Character &player, App &app, bool isFireboy) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 newPos = pos;

  // 取得網格系統的邊界
  GridSystem &grid = app.GetGridSystem();
  float minX = grid.GetMinX();
  float maxX = grid.GetMaxX();
  float minY = grid.GetMinY();
  float maxY = grid.GetMaxY();

  // 限制玩家在地圖邊界內
  bool positionChanged = false;
  if (pos.x < minX) {
    newPos.x = minX;
    positionChanged = true;
  }
  if (pos.x > maxX) {
    newPos.x = maxX;
    positionChanged = true;
  }
  if (pos.y < minY) {
    newPos.y = minY;
    positionChanged = true;
  }
  if (pos.y > maxY) {
    newPos.y = maxY;
    positionChanged = true;
  }

  // 只有當位置被邊界限制調整了，才需要再次檢查碰撞
  if (positionChanged && app.CheckCharacterCollision(newPos, isFireboy)) {
    glm::vec2 safePos = newPos;
    float adjustment = grid.GetCellSize() / 2.0f;

    if (newPos.x == minX)
      safePos.x += adjustment;
    else if (newPos.x == maxX)
      safePos.x -= adjustment;

    if (newPos.y == minY)
      safePos.y += adjustment;
    else if (newPos.y == maxY)
      safePos.y -= adjustment;

    if (app.CheckCharacterCollision(safePos, isFireboy)) {
      player.SetPosition(newPos);
    } else {
      player.SetPosition(safePos);
    }
  } else {
    if (app.CheckCharacterCollision(newPos, isFireboy)) {
      player.SetPosition(newPos);
    } else {
      player.SetPosition(newPos);
    }
  }
}

// 轉換 CellType 為字串
std::string CellTypeToString(CellType type) {
  switch (type) {
  case CellType::EMPTY:
    return "EMPTY";
  case CellType::FLOOR:
    return "FLOOR";
  case CellType::WALL:
    return "WALL";
  case CellType::DOOR_FIRE:
    return "DOOR_FIRE";
  case CellType::DOOR_WATER:
    return "DOOR_WATER";
  case CellType::GEM_FIRE:
    return "GEM_FIRE";
  case CellType::GEM_WATER:
    return "GEM_WATER";
  case CellType::GEM_GREEN:
    return "GEM_GREEN";
  case CellType::LAVA:
    return "LAVA";
  case CellType::WATER:
    return "WATER";
  case CellType::POISON:
    return "POISON";
  case CellType::BUTTON:
    return "BUTTON";
  case CellType::LEVER:
    return "LEVER";
  case CellType::PLATFORM:
    return "PLATFORM";
  case CellType::FAN:
    return "FAN";
  case CellType::BOX:
    return "BOX";
  case CellType::STONE:
    return "STONE";
  default:
    return "UNKNOWN";
  }
}

// 遊戲主循環
void App::GamePlay() {
  LOG_TRACE("Game Play");

  // 確保地圖網格系統已經載入
  if (!m_IsGridLoaded) {
    if (!LoadLevelGrid(m_CurrentLevel)) {
      LOG_ERROR("Failed to load level {}", m_CurrentLevel);
      return;
    }
  }

  // 初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(35, 4);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(3, 16);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // 記錄角色位置
  static glm::vec2 prevFireboyPos = m_Fireboy->GetPosition();
  static glm::vec2 prevWatergirlPos = m_Watergirl->GetPosition();

  // Fireboy 控制
  int fireboyMoveX = 0;
  bool fireboyUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))
    fireboyMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT))
    fireboyMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::UP))
    fireboyUpKeyPressed = true;

  m_Fireboy->Move(fireboyMoveX, fireboyUpKeyPressed);
  m_Fireboy->UpdateJump();
  RestrictPlayerPosition(*m_Fireboy, *this, true);

  // Watergirl 控制
  int watergirlMoveX = 0;
  bool watergirlUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::A))
    watergirlMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::D))
    watergirlMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::W))
    watergirlUpKeyPressed = true;

  m_Watergirl->Move(watergirlMoveX, watergirlUpKeyPressed);
  m_Watergirl->UpdateJump();
  RestrictPlayerPosition(*m_Watergirl, *this, false);

  // Debug 顯示角色位置
  if (m_Fireboy->GetPosition() != prevFireboyPos) {
    prevFireboyPos = m_Fireboy->GetPosition();
  }
  if (m_Watergirl->GetPosition() != prevWatergirlPos) {
    prevWatergirlPos = m_Watergirl->GetPosition();
  }

  // 處理遊戲結束條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}
