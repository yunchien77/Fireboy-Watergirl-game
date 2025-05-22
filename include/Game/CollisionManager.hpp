#pragma once
#include "Character/Character.hpp"
#include "Object/GridSystem.hpp"
#include <glm/glm.hpp>
#include <memory>


class CollisionManager {
public:
  CollisionManager(GridSystem &gridSystem);

  // 檢查角色碰撞
  bool CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                               bool isFireboy, int deltaX = 0);

  // 限制玩家在地圖邊界內
  void RestrictPlayerPosition(Character &player);

  // 處理角色碰撞並調整位置
  void HandleCollision(Character &player, bool isFireboy);

private:
  GridSystem &m_GridSystem;

  // 碰撞調整的輔助方法
  bool TryAdjustPosition(Character &player, bool isFireboy);
};