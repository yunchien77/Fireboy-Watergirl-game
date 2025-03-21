#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "GridSystem.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>

bool App::CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                                  bool isFireboy, int deltaX) {
  if (!m_IsGridLoaded) {
    return false; // 如果網格未載入，默認無碰撞
  }
  return m_GridSystem.CheckCollision(position, size, isFireboy, deltaX);
}

// 載入地圖網格，並初始化 GridSystem
bool App::LoadLevelGrid(int levelNumber) {
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
      m_Root.AddChild(m_Fireboy);
    }

    // 初始化角色 Watergirl
    if (!m_Watergirl) {
      m_Watergirl = std::make_shared<Watergirl>();
      glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(3, 17);
      m_Watergirl->SetPosition(watergirlInitPos);
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
  } break;

  case 2: // 先讓 level2 ~ level5 預設為 level1，等你完成後再改回來
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

// 限制玩家在地圖邊界內
void RestrictPlayerPosition(Character &player, App &app) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 newPos = pos;

  // 取得網格系統的邊界
  GridSystem &grid = app.GetGridSystem();
  float minX = grid.GetMinX();
  float maxX = grid.GetMaxX();
  float minY = grid.GetMinY();
  float maxY = grid.GetMaxY();

  // 限制玩家在地圖邊界內
  if (pos.x < minX) {
    newPos.x = minX;
  }
  if (pos.x > maxX) {
    newPos.x = maxX;
  }
  if (pos.y < minY) {
    newPos.y = minY;
  }
  if (pos.y > maxY) {
    newPos.y = maxY;
  }

  // 設置新位置
  player.SetPosition(newPos);
}

// 處理角色碰撞
void HandleCollision(Character &player, App &app, bool isFireboy) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 size = player.GetSize();

  // 檢查碰撞
  // deltaX=0 -> 檢查當前位置是否有碰撞
  if (app.CheckCharacterCollision(pos, size, isFireboy, 0)) {
    // 如果發生碰撞，嘗試找一個安全的位置
    GridSystem &grid = app.GetGridSystem();
    float adjustment = grid.GetCellSize() / 4.0f;

    // 嘗試幾個方向的調整
    glm::vec2 testPositions[] = {
        {pos.x, pos.y - adjustment}, // 上
        {pos.x, pos.y + adjustment}, // 下
        {pos.x - adjustment, pos.y}, // 左
        {pos.x + adjustment, pos.y}, // 右
    };

    for (const auto &testPos : testPositions) {
      if (!app.CheckCharacterCollision(testPos, size, isFireboy)) {
        player.SetPosition(testPos);
        return;
      }
    }
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

  m_Fireboy->Move(fireboyMoveX, fireboyUpKeyPressed, m_GridSystem, true);
  m_Fireboy->UpdateJump(m_GridSystem);
  m_Fireboy->ApplyGravity(m_GridSystem);

  // 分別處理邊界限制和碰撞檢測
  RestrictPlayerPosition(*m_Fireboy, *this);
  HandleCollision(*m_Fireboy, *this, true);

  // Watergirl 控制
  int watergirlMoveX = 0;
  bool watergirlUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::A))
    watergirlMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::D))
    watergirlMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::W))
    watergirlUpKeyPressed = true;

  m_Watergirl->Move(watergirlMoveX, watergirlUpKeyPressed, m_GridSystem, true);
  m_Watergirl->UpdateJump(m_GridSystem);
  m_Watergirl->ApplyGravity(m_GridSystem);

  // 分別處理邊界限制和碰撞檢測
  RestrictPlayerPosition(*m_Watergirl, *this);
  HandleCollision(*m_Watergirl, *this, false);

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