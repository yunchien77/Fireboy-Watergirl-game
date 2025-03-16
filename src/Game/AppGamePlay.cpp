#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>

// 載入地圖網格
bool App::LoadLevelGrid(int levelNumber) {
  std::string gridFilePath =
      RESOURCE_DIR "/map/level" + std::to_string(levelNumber) + "_grid.txt";
  bool success = m_GridSystem.LoadFromFile(gridFilePath);

  if (success) {
    m_IsGridLoaded = true;
    LOG_INFO("Successfully loaded grid for level {}", levelNumber);
  } else {
    LOG_ERROR("Failed to load grid for level {}", levelNumber);
    // 如果沒有格子檔案，創建一個預設的空格子
    m_GridSystem = GridSystem();
    m_IsGridLoaded = false;
  }

  return success;
}

// 檢查角色碰撞
bool App::CheckCharacterCollision(const glm::vec2 &position, bool isFireboy) {
  if (!m_IsGridLoaded) {
    return false; // 如果網格未載入，默認無碰撞
  }

  return m_GridSystem.CheckCollision(position, isFireboy);
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
    // 當邊界調整後位置產生碰撞，找尋安全位置
    // 簡單解決方案：稍微向內移動到無碰撞位置
    glm::vec2 safePos = newPos;
    float adjustment = grid.GetCellSize() / 2.0f;

    // 嘗試向中心點方向移動
    if (newPos.x == minX)
      safePos.x += adjustment;
    else if (newPos.x == maxX)
      safePos.x -= adjustment;

    if (newPos.y == minY)
      safePos.y += adjustment;
    else if (newPos.y == maxY)
      safePos.y -= adjustment;

    // 如果調整後仍有碰撞，則至少確保在邊界內
    if (app.CheckCharacterCollision(safePos, isFireboy)) {
      player.SetPosition(newPos); // 至少保持在邊界內
    } else {
      player.SetPosition(safePos);
    }
  } else {
    // 處理一般的移動碰撞
    if (app.CheckCharacterCollision(newPos, isFireboy)) {
      // 保持原位置，但確保仍在邊界內
      player.SetPosition(newPos);
    } else {
      player.SetPosition(newPos);
    }
  }
}

// (Debug)
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

void App::GamePlay() {
  LOG_TRACE("Game Play");

  // 確保地圖網格系統已經載入
  if (!m_IsGridLoaded) {
    LoadLevelGrid(m_CurrentLevel);
  }

  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(1, 27);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(1, 23);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // 記錄先前的位置，用於檢測移動
  static glm::vec2 prevFireboyPos = m_Fireboy->GetPosition();
  static glm::vec2 prevWatergirlPos = m_Watergirl->GetPosition();

  int fireboyMoveX = 0;
  bool fireboyUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))
    fireboyMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT))
    fireboyMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
    fireboyUpKeyPressed = true;
  }

  m_Fireboy->Move(fireboyMoveX, fireboyUpKeyPressed);
  m_Fireboy->UpdateJump();
  RestrictPlayerPosition(*m_Fireboy, *this, true);

  int watergirlMoveX = 0;
  bool watergirlUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::A))
    watergirlMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::D))
    watergirlMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
    watergirlUpKeyPressed = true;
  }

  m_Watergirl->Move(watergirlMoveX, watergirlUpKeyPressed);
  m_Watergirl->UpdateJump();
  RestrictPlayerPosition(*m_Watergirl, *this, false);

  glm::vec2 fireboyPos = m_Fireboy->GetPosition();
  if (fireboyPos != prevFireboyPos) {
    glm::ivec2 fireboyCell = m_GridSystem.GameToCellPosition(fireboyPos);
    CellType fireboyCurrentCell =
        m_GridSystem.GetCell(fireboyCell.x, fireboyCell.y);
    std::cout << "Fireboy Position: (" << fireboyPos.x << ", " << fireboyPos.y
              << ") | "
              << "Cell: (" << fireboyCell.x << ", " << fireboyCell.y << ") | "
              << "Type: " << CellTypeToString(fireboyCurrentCell) << std::endl;
    prevFireboyPos = fireboyPos;
  }

  // 如果水女移動了且按下了按鍵，顯示其位置和格子信息
  glm::vec2 watergirlPos = m_Watergirl->GetPosition();
  if (watergirlPos != prevWatergirlPos) {
    glm::ivec2 watergirlCell = m_GridSystem.GameToCellPosition(watergirlPos);
    CellType watergirlCurrentCell =
        m_GridSystem.GetCell(watergirlCell.x, watergirlCell.y);
    std::cout << "Watergirl Position: (" << watergirlPos.x << ", "
              << watergirlPos.y << ") | "
              << "Cell: (" << watergirlCell.x << ", " << watergirlCell.y
              << ") | "
              << "Type: " << CellTypeToString(watergirlCurrentCell)
              << std::endl;
    prevWatergirlPos = watergirlPos;
  }

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}