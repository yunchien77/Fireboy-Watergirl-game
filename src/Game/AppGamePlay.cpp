#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Game/CollisionManager.hpp"
#include "Game/GameMechanicsManager.hpp"
#include "Game/InputHandler.hpp"
#include "Mechanism/Box.hpp"
#include "Object/GridSystem.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <Mechanism/Gem.hpp>
#include <Mechanism/LiquidTrap.hpp>
#include <iostream>
#include <memory>

void App::InitializeManagers() {
  m_CollisionManager = std::make_unique<CollisionManager>(m_GridSystem);
  m_MechanicsManager = std::make_unique<GameMechanicsManager>(m_GridSystem);
  m_InputHandler = std::make_unique<InputHandler>();

  // 設置角色和門
  m_MechanicsManager->SetCharacters(m_Fireboy, m_Watergirl);
  m_MechanicsManager->SetDoors(m_Fireboy_Door, m_Watergirl_Door);

  // 設置遊戲物件
  m_MechanicsManager->SetGameObjects(m_Gems, m_Buttons, m_Gates, m_Levers,
                                     m_Platforms, m_Boxes, m_Fans, m_Traps);
}

bool App::CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                                  bool isFireboy, int deltaX) {
  if (!m_IsGridLoaded || !m_CollisionManager) {
    return false;
  }
  return m_CollisionManager->CheckCharacterCollision(position, size, isFireboy,
                                                     deltaX);
}

bool App::CheckWinCondition() {
  return m_MechanicsManager ? m_MechanicsManager->CheckWinCondition() : false;
}

bool App::GetGemCollectionStatus() {
  return m_MechanicsManager ? m_MechanicsManager->GetGemCollectionStatus()
                            : false;
}

bool App::CheckBoxCollision(std::shared_ptr<Character> character) {
  bool hasCollision = false;
  for (auto &box : m_Boxes) {
    if (box->CheckCharacterCollision(character)) {
      hasCollision = true;
    }
  }
  return hasCollision;
}

void App::ResetGameLevel() {
  if (m_MechanicsManager) {
    m_MechanicsManager->ResetLevel();
  }
}

void App::HandleAllCollisions() {
  if (!m_CollisionManager)
    return;

  // 限制角色在地圖邊界內
  m_CollisionManager->RestrictPlayerPosition(*m_Fireboy);
  m_CollisionManager->RestrictPlayerPosition(*m_Watergirl);

  // 處理角色與網格的碰撞
  m_CollisionManager->HandleCollision(*m_Fireboy, true);
  m_CollisionManager->HandleCollision(*m_Watergirl, false);

  // 處理門的阻擋
  if (m_MechanicsManager) {
    m_MechanicsManager->HandleGateBlocking();
  }
}

void App::GamePlay() {
  LOG_TRACE("Game Play");

  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  glm::vec2 gameCoords = ConvertToGameCoordinates(mouseX, mouseY);

  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    if (m_pauseButton->HandleClick(gameCoords.x, gameCoords.y)) {
      m_CurrentState = State::GAME_PAUSE;
      return;
    }
  }

  if (!m_IsGridLoaded) {
    if (!LoadLevelGrid(m_CurrentLevel)) {
      LOG_ERROR("Failed to load level {}", m_CurrentLevel);
      return;
    }
    InitializeManagers();
  }

  if (!m_CollisionManager || !m_MechanicsManager || !m_InputHandler) {
    InitializeManagers();
  }

  if (m_InputHandler->IsExitRequested()) {
    m_CurrentState = State::END;
    return;
  }

  // 檢查角色死亡狀態
  if (m_MechanicsManager->IsAnyCharacterDead()) {
    LOG_INFO("Character died, resetting level...");
    ResetGameLevel();
    return;
  }

  // 處理角色移動輸入
  m_InputHandler->ProcessCharacterMovement(m_Fireboy, m_Watergirl,
                                           m_GridSystem);

  // 處理所有碰撞
  HandleAllCollisions();

  // 更新所有遊戲機制
  float deltaTime = 1.0f / 60.0f; // 60fps
  m_MechanicsManager->UpdateInteractions(deltaTime);

  // 檢查勝利條件
  if (m_MechanicsManager->CheckWinCondition()) {
    m_CurrentState = State::GAME_WIN;
    LOG_INFO("Level completed! Both characters at their doors.");
  }

  m_Root.Update();
}
