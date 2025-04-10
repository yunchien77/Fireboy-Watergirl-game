#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Object/GridSystem.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <Mechanism/Gem.hpp>
#include <Mechanism/LiquidTrap.hpp>
#include <iostream>
#include <memory>

bool App::CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                                  bool isFireboy, int deltaX) {
  if (!m_IsGridLoaded) {
    return false; // 如果網格未載入，默認無碰撞
  }
  return m_GridSystem.CheckCollision(position, size, isFireboy, deltaX);
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
    std::cout << "Collision detected!" << std::endl;
    // 如果發生碰撞，嘗試找一個安全的位置
    GridSystem &grid = app.GetGridSystem();
    float adjustment = grid.GetCellSize() / 5.0f;

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

void App::ResetGameLevel() {
  if (m_Fireboy)
    m_Fireboy->Respawn();
  if (m_Watergirl)
    m_Watergirl->Respawn();

  for (auto &trap : m_Traps) {
    trap->OnCharacterEnter(nullptr);
  }

  for (auto &gem : m_Gems) {
    gem->Respawn();
  }
}

// 檢查是否達成勝利條件
bool App::CheckWinCondition() {
  // 兩個門都必須為全開狀態
  return (m_Fireboy_Door->IsOpen() && m_Watergirl_Door->IsOpen());
}

// 檢查角色與門的互動
void App::CheckCharacterDoorInteraction() {
  // Check if Fireboy is at his door
  glm::vec2 fireboyPos = m_Fireboy->GetPosition();
  glm::vec2 fireboyDoorPos = m_Fireboy_Door->GetPosition();

  // Define a distance threshold for door interaction
  const float interactionDistance = m_GridSystem.GetCellSize() * 1.5f;

  // Calculate distances
  float fireboyToDoorDistance = glm::length(fireboyPos - fireboyDoorPos);

  // Update Fireboy's door state based on proximity
  bool fireboyAtDoor = (fireboyToDoorDistance < interactionDistance);
  if (fireboyAtDoor != m_Fireboy_Door->IsCharacterAtDoor()) {
    m_Fireboy_Door->SetCharacterAtDoor(fireboyAtDoor);
  }

  // Check if Watergirl is at her door
  glm::vec2 watergirlPos = m_Watergirl->GetPosition();
  glm::vec2 watergirlDoorPos = m_Watergirl_Door->GetPosition();

  float watergirlToDoorDistance = glm::length(watergirlPos - watergirlDoorPos);

  // Update Watergirl's door state based on proximity
  bool watergirlAtDoor = (watergirlToDoorDistance < interactionDistance);
  if (watergirlAtDoor != m_Watergirl_Door->IsCharacterAtDoor()) {
    m_Watergirl_Door->SetCharacterAtDoor(watergirlAtDoor);
  }

  if (CheckWinCondition()) {
    m_CurrentState = State::GAME_WIN;
    LOG_INFO("Level completed! Both characters at their doors.");
  }
}

// 檢查寶石收集狀態
bool App::GetGemCollectionStatus() {
  int collectedGems = 0;
  int totalGems = m_Gems.size();
  for (auto &gem : m_Gems) {
    if (gem->IsCollected()) {
      collectedGems++;
    }
  }

  if (totalGems == collectedGems) {
    return true; // 所有寶石都已收集
  } else {
    return false; // 還有未收集的寶石
  }
}

void App::GamePlay() {
  LOG_TRACE("Game Play");

  if (!m_IsGridLoaded) {
    if (!LoadLevelGrid(m_CurrentLevel)) {
      LOG_ERROR("Failed to load level {}", m_CurrentLevel);
      return;
    }
  }

  // 控制輸入判斷區
  int fireboyMoveX = 0;
  bool fireboyUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))
    fireboyMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT))
    fireboyMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::UP))
    fireboyUpKeyPressed = true;

  int watergirlMoveX = 0;
  bool watergirlUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::A))
    watergirlMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::D))
    watergirlMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::W))
    watergirlUpKeyPressed = true;

  // 記錄上一次位置（給門阻擋使用）
  m_Fireboy->SetPreviousPosition();
  m_Watergirl->SetPreviousPosition();

  // 執行移動、跳躍、重力
  m_Fireboy->Move(fireboyMoveX, fireboyUpKeyPressed, m_GridSystem, true);
  m_Fireboy->UpdateJump(m_GridSystem);
  m_Fireboy->ApplyGravity(m_GridSystem);

  m_Watergirl->Move(watergirlMoveX, watergirlUpKeyPressed, m_GridSystem, true);
  m_Watergirl->UpdateJump(m_GridSystem);
  m_Watergirl->ApplyGravity(m_GridSystem);

  // Gate 阻擋判斷
  for (const auto &gate : m_Triggers) {
    if (gate->IsBlocking()) {
      if (SDL_HasIntersection(&gate->getRect(), &m_Fireboy->getRect())) {
        m_Fireboy->UndoMovement();
      }
      if (SDL_HasIntersection(&gate->getRect(), &m_Watergirl->getRect())) {
        m_Watergirl->UndoMovement();
      }
    }
  }

  // 限制在邊界內、格子碰撞
  RestrictPlayerPosition(*m_Fireboy, *this);
  HandleCollision(*m_Fireboy, *this, true);
  RestrictPlayerPosition(*m_Watergirl, *this);
  HandleCollision(*m_Watergirl, *this, false);

  // 處理寶石收集
  for (auto &gem : m_Gems) {
    if (SDL_HasIntersection(&gem->getRect(), &m_Fireboy->getRect())) {
      gem->OnCharacterEnter(m_Fireboy.get());
    }
    if (SDL_HasIntersection(&gem->getRect(), &m_Watergirl->getRect())) {
      gem->OnCharacterEnter(m_Watergirl.get());
    }
  }

  // 處理液體陷阱觸發
  glm::ivec2 fireboyCell =
      m_GridSystem.GameToCellPosition(m_Fireboy->GetPosition());
  CellType cellTypeFireboy = m_GridSystem.GetCell(fireboyCell.x, fireboyCell.y);
  if (cellTypeFireboy == CellType::WATER ||
      cellTypeFireboy == CellType::POISON) {
    for (auto &trap : m_Traps)
      trap->OnCharacterEnter(m_Fireboy.get());
  }

  glm::ivec2 watergirlCell =
      m_GridSystem.GameToCellPosition(m_Watergirl->GetPosition());
  CellType cellTypeWatergirl =
      m_GridSystem.GetCell(watergirlCell.x, watergirlCell.y);
  if (cellTypeWatergirl == CellType::LAVA ||
      cellTypeWatergirl == CellType::POISON) {
    for (auto &trap : m_Traps)
      trap->OnCharacterEnter(m_Watergirl.get());
  }

  // 門動畫更新 & 勝利判斷
  m_Fireboy_Door->UpdateAnimation();
  m_Watergirl_Door->UpdateAnimation();
  CheckCharacterDoorInteraction();

  // 其他按鍵判斷與離開條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  if (m_Fireboy->IsDead() || m_Watergirl->IsDead()) {
    std::cout << "有角色死亡，關卡重置中...\n";
    ResetGameLevel();
    return;
  }

  // 按鈕更新（踩到的邏輯）
  for (auto &b : m_Buttons) {
    b->update(m_Fireboy.get(), m_Watergirl.get());
  }

  float deltaTime = 1.0f / 60.0f;
  for (auto &g : m_Triggers) {
    g->UpdateAnimation(deltaTime);
  }

  m_Root.Update();
}
