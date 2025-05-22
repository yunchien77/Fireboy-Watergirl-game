#include "Game/CollisionManager.hpp"
#include "Util/Logger.hpp"
#include <iostream>

CollisionManager::CollisionManager(GridSystem &gridSystem)
    : m_GridSystem(gridSystem) {}

bool CollisionManager::CheckCharacterCollision(const glm::vec2 &position,
                                               glm::vec2 size, bool isFireboy,
                                               int deltaX) {
  return m_GridSystem.CheckCollision(position, size, isFireboy, deltaX);
}

void CollisionManager::RestrictPlayerPosition(Character &player) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 newPos = pos;

  float minX = m_GridSystem.GetMinX();
  float maxX = m_GridSystem.GetMaxX();
  float minY = m_GridSystem.GetMinY();
  float maxY = m_GridSystem.GetMaxY();

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

  if (newPos != pos) {
    player.SetPosition(newPos);
  }
}

void CollisionManager::HandleCollision(Character &player, bool isFireboy) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 size = player.GetSize();

  // 檢查碰撞 - deltaX=0 表示檢查當前位置是否有碰撞
  if (CheckCharacterCollision(pos, size, isFireboy, 0)) {
    LOG_TRACE("Collision detected for {}", isFireboy ? "Fireboy" : "Watergirl");

    // 嘗試調整位置
    if (!TryAdjustPosition(player, isFireboy)) {
      LOG_WARN("Unable to resolve collision for {}",
               isFireboy ? "Fireboy" : "Watergirl");
    }
  }
}

bool CollisionManager::TryAdjustPosition(Character &player, bool isFireboy) {
  glm::vec2 pos = player.GetPosition();
  glm::vec2 size = player.GetSize();
  float adjustment = m_GridSystem.GetCellSize() / 5.0f;

  // 嘗試幾個方向的調整
  glm::vec2 testPositions[] = {
      {pos.x, pos.y - adjustment}, // 上
      {pos.x, pos.y + adjustment}, // 下
      {pos.x - adjustment, pos.y}, // 左
      {pos.x + adjustment, pos.y}, // 右
  };

  for (const auto &testPos : testPositions) {
    if (!CheckCharacterCollision(testPos, size, isFireboy, 0)) {
      player.SetPosition(testPos);
      return true;
    }
  }

  return false;
}