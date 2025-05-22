#include "Game/GameMechanicsManager.hpp"
#include "Util/Logger.hpp"
#include <SDL.h>
#include <iostream>

GameMechanicsManager::GameMechanicsManager(GridSystem &gridSystem)
    : m_GridSystem(gridSystem), m_Gems(nullptr), m_Buttons(nullptr),
      m_Gates(nullptr), m_Levers(nullptr), m_Platforms(nullptr),
      m_Boxes(nullptr), m_Fans(nullptr), m_Traps(nullptr) {}

void GameMechanicsManager::SetCharacters(std::shared_ptr<Character> fireboy,
                                         std::shared_ptr<Character> watergirl) {
  m_Fireboy = fireboy;
  m_Watergirl = watergirl;
}

void GameMechanicsManager::SetDoors(std::shared_ptr<Door> fireboyDoor,
                                    std::shared_ptr<Door> watergirlDoor) {
  m_FireboyDoor = fireboyDoor;
  m_WatergirlDoor = watergirlDoor;
}

void GameMechanicsManager::SetGameObjects(
    std::vector<std::shared_ptr<Gem>> &gems,
    std::vector<std::shared_ptr<Button>> &buttons,
    std::vector<std::shared_ptr<Gate>> &gates,
    std::vector<std::shared_ptr<Lever>> &levers,
    std::vector<std::shared_ptr<Platform>> &platforms,
    std::vector<std::shared_ptr<Box>> &boxes,
    std::vector<std::shared_ptr<Fan>> &fans,
    std::vector<std::shared_ptr<LiquidTrap>> &traps) {

  m_Gems = &gems;
  m_Buttons = &buttons;
  m_Gates = &gates;
  m_Levers = &levers;
  m_Platforms = &platforms;
  m_Boxes = &boxes;
  m_Fans = &fans;
  m_Traps = &traps;
}

void GameMechanicsManager::UpdateInteractions(float deltaTime) {
  PrepareCharactersForUpdate();

  UpdateFans(deltaTime);
  UpdatePlatforms(deltaTime);
  UpdateBoxes();
  UpdateButtons();
  UpdateLevers();
  UpdateGates(deltaTime);

  HandleGemCollection();
  HandleLiquidTraps();

  CheckCharacterDoorInteraction();
  UpdateCharacterPlatformMovement();

  // 更新角色
  if (m_Fireboy)
    m_Fireboy->Update();
  if (m_Watergirl)
    m_Watergirl->Update();
}

void GameMechanicsManager::PrepareCharactersForUpdate() {
  if (!m_Fireboy || !m_Watergirl)
    return;

  // 紀錄上一次位置
  m_Fireboy->SetPreviousPosition();
  m_Watergirl->SetPreviousPosition();

  if (m_Boxes) {
    m_Fireboy->SetBoxes(*m_Boxes);
    m_Watergirl->SetBoxes(*m_Boxes);
  }
  if (m_Platforms) {
    m_Fireboy->SetPlatforms(*m_Platforms);
    m_Watergirl->SetPlatforms(*m_Platforms);
  }

  m_Fireboy->SetGridSystem(&m_GridSystem);
  m_Watergirl->SetGridSystem(&m_GridSystem);

  // 重置平台狀態
  m_Fireboy->SetStandingOnPlatform(false);
  m_Watergirl->SetStandingOnPlatform(false);
}

void GameMechanicsManager::UpdateFans(float deltaTime) {
  if (!m_Fans || !m_Fireboy || !m_Watergirl)
    return;

  for (auto &fan : *m_Fans) {
    fan->Update(deltaTime);
    fan->ApplyWindForce(m_Fireboy.get());
    fan->ApplyWindForce(m_Watergirl.get());
  }
}

void GameMechanicsManager::UpdatePlatforms(float deltaTime) {
  if (!m_Platforms || !m_Fireboy || !m_Watergirl)
    return;

  std::vector<Character *> charList = {m_Fireboy.get(), m_Watergirl.get()};
  for (auto &platform : *m_Platforms) {
    platform->UpdateAnimation(deltaTime, charList);
  }
}

void GameMechanicsManager::UpdateBoxes() {
  if (!m_Boxes || !m_Fireboy || !m_Watergirl)
    return;

  for (auto &box : *m_Boxes) {
    box->Update();
    // 僅在不跳躍時檢查碰撞
    if (!m_Fireboy->IsJumping()) {
      box->OnCollisionWithCharacter(m_Fireboy);
    }
    if (!m_Watergirl->IsJumping()) {
      box->OnCollisionWithCharacter(m_Watergirl);
    }
  }
}

void GameMechanicsManager::UpdateButtons() {
  if (!m_Buttons || !m_Fireboy || !m_Watergirl)
    return;

  for (auto &button : *m_Buttons) {
    button->Update(m_Fireboy.get(), m_Watergirl.get());
  }
}

void GameMechanicsManager::UpdateLevers() {
  if (!m_Levers || !m_Fireboy || !m_Watergirl)
    return;

  for (auto &lever : *m_Levers) {
    lever->Update(m_Fireboy.get(), m_Watergirl.get());
  }
}

void GameMechanicsManager::UpdateGates(float deltaTime) {
  if (!m_Gates)
    return;

  for (auto &gate : *m_Gates) {
    gate->UpdateAnimation(deltaTime);
  }
}

void GameMechanicsManager::HandleGemCollection() {
  if (!m_Gems || !m_Fireboy || !m_Watergirl)
    return;

  for (auto &gem : *m_Gems) {
    if (SDL_HasIntersection(&gem->getRect(), &m_Fireboy->getRect())) {
      gem->OnCharacterEnter(m_Fireboy.get());
    }
    if (SDL_HasIntersection(&gem->getRect(), &m_Watergirl->getRect())) {
      gem->OnCharacterEnter(m_Watergirl.get());
    }
  }
}

void GameMechanicsManager::HandleLiquidTraps() {
  if (!m_Traps || !m_Fireboy || !m_Watergirl)
    return;

  // 檢查 Fireboy 是否踩到水或毒液
  glm::ivec2 fireboyCell =
      m_GridSystem.GameToCellPosition(m_Fireboy->GetPosition());
  CellType cellTypeFireboy = m_GridSystem.GetCell(fireboyCell.x, fireboyCell.y);
  if (cellTypeFireboy == CellType::WATER ||
      cellTypeFireboy == CellType::POISON) {
    for (auto &trap : *m_Traps) {
      trap->OnCharacterEnter(m_Fireboy.get());
    }
  }

  // 檢查 Watergirl 是否踩到岩浆或毒液
  glm::ivec2 watergirlCell =
      m_GridSystem.GameToCellPosition(m_Watergirl->GetPosition());
  CellType cellTypeWatergirl =
      m_GridSystem.GetCell(watergirlCell.x, watergirlCell.y);
  if (cellTypeWatergirl == CellType::LAVA ||
      cellTypeWatergirl == CellType::POISON) {
    for (auto &trap : *m_Traps) {
      trap->OnCharacterEnter(m_Watergirl.get());
    }
  }
}

void GameMechanicsManager::HandleGateBlocking() {
  if (!m_Gates || !m_Fireboy || !m_Watergirl)
    return;

  for (const auto &gate : *m_Gates) {
    if (gate->IsBlocking()) {
      if (SDL_HasIntersection(&gate->getRect(), &m_Fireboy->getRect())) {
        m_Fireboy->UndoMovement();
      }
      if (SDL_HasIntersection(&gate->getRect(), &m_Watergirl->getRect())) {
        m_Watergirl->UndoMovement();
      }
    }
  }
}

bool GameMechanicsManager::CheckWinCondition() {
  if (!m_FireboyDoor || !m_WatergirlDoor)
    return false;
  return (m_FireboyDoor->IsOpen() && m_WatergirlDoor->IsOpen());
}

void GameMechanicsManager::CheckCharacterDoorInteraction() {
  if (!m_Fireboy || !m_Watergirl || !m_FireboyDoor || !m_WatergirlDoor)
    return;

  const float interactionDistance = m_GridSystem.GetCellSize() * 1.5f;

  // 檢查 Fireboy 是否在門附近
  glm::vec2 fireboyPos = m_Fireboy->GetPosition();
  glm::vec2 fireboyDoorPos = m_FireboyDoor->GetPosition();
  float fireboyToDoorDistance = glm::length(fireboyPos - fireboyDoorPos);

  bool fireboyAtDoor = (fireboyToDoorDistance < interactionDistance);
  if (fireboyAtDoor != m_FireboyDoor->IsCharacterAtDoor()) {
    m_FireboyDoor->SetCharacterAtDoor(fireboyAtDoor);
  }

  // 檢查 Watergirl 是否在門附近
  glm::vec2 watergirlPos = m_Watergirl->GetPosition();
  glm::vec2 watergirlDoorPos = m_WatergirlDoor->GetPosition();
  float watergirlToDoorDistance = glm::length(watergirlPos - watergirlDoorPos);

  bool watergirlAtDoor = (watergirlToDoorDistance < interactionDistance);
  if (watergirlAtDoor != m_WatergirlDoor->IsCharacterAtDoor()) {
    m_WatergirlDoor->SetCharacterAtDoor(watergirlAtDoor);
  }

  // 更新門動畫
  m_FireboyDoor->UpdateAnimation();
  m_WatergirlDoor->UpdateAnimation();
}

bool GameMechanicsManager::GetGemCollectionStatus() {
  if (!m_Gems)
    return false;

  int collectedGems = 0;
  int totalGems = m_Gems->size();

  for (auto &gem : *m_Gems) {
    if (gem->IsCollected()) {
      collectedGems++;
    }
  }

  return (totalGems == collectedGems);
}

void GameMechanicsManager::ResetLevel() {
  if (m_Fireboy) {
    m_Fireboy->Respawn();
    m_Fireboy->SetAffectedByWind(false);
  }
  if (m_Watergirl) {
    m_Watergirl->Respawn();
    m_Watergirl->SetAffectedByWind(false);
  }

  if (m_Traps) {
    for (auto &trap : *m_Traps) {
      trap->OnCharacterEnter(nullptr);
    }
  }

  if (m_Gems) {
    for (auto &gem : *m_Gems) {
      gem->Respawn();
    }
  }

  if (m_Levers) {
    for (auto &lever : *m_Levers) {
      lever->Respawn();
    }
  }

  if (m_Platforms) {
    for (auto &platform : *m_Platforms) {
      platform->Respawn();
    }
  }

  if (m_Gates) {
    for (auto &gate : *m_Gates) {
      gate->Respawn();
    }
  }

  if (m_Buttons) {
    for (auto &button : *m_Buttons) {
      button->Respawn();
    }
  }

  if (m_Boxes) {
    for (auto &box : *m_Boxes) {
      box->Respawn();
    }
  }

  if (m_Fans) {
    for (auto &fan : *m_Fans) {
      fan->Respawn();
    }
  }
}

bool GameMechanicsManager::IsAnyCharacterDead() {
  return (m_Fireboy && m_Fireboy->IsDead()) ||
         (m_Watergirl && m_Watergirl->IsDead());
}

void GameMechanicsManager::UpdateCharacterPlatformMovement() {
  if (!m_Fireboy || !m_Watergirl)
    return;

  // 若角色站在平台上，改用 MoveWithCollision 来移動並做碰撞修正
  if (m_Fireboy->IsStandingOnPlatform() && m_Fireboy->GetCurrentPlatform()) {
    glm::vec2 delta = m_Fireboy->GetCurrentPlatform()->GetDeltaMovement();
    if (glm::length(delta) > 0.01f) {
      m_Fireboy->MoveWithCollision(delta, m_GridSystem);
    }
  }

  if (m_Watergirl->IsStandingOnPlatform() &&
      m_Watergirl->GetCurrentPlatform()) {
    glm::vec2 delta = m_Watergirl->GetCurrentPlatform()->GetDeltaMovement();
    if (glm::length(delta) > 0.01f) {
      m_Watergirl->MoveWithCollision(delta, m_GridSystem);
    }
  }
}