#include "Character/Character.hpp"
#include "Character/CharacterComponent.hpp"
#include "Character/CharacterState.hpp"
#include "Mechanism/Fan.hpp"
#include <cmath>
#include <iostream>
#include <map>

#define CHARACTER_OFFSET 13.5f

CharacterCollisionComponent::CharacterCollisionComponent(Character *owner)
    : m_Owner(owner) {}

bool CharacterCollisionComponent::CheckTopCollision(const glm::vec2 &position,
                                                    const GridSystem &grid) {
  float left = position.x - (m_Owner->GetSize().x / 2) + 0.3f; // 添加容差
  float right = position.x + (m_Owner->GetSize().x / 2) - 0.3f;
  float top = position.y + m_Owner->GetSize().y;

  int checkPoints = 10;
  for (int i = 0; i <= checkPoints; i++) {
    float checkX =
        left + (right - left) * (static_cast<float>(i) / checkPoints);
    glm::ivec2 gridPosTop = grid.GameToCellPosition(glm::vec2(checkX, top));
    CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

    if (aboveCell != CellType::EMPTY) {
      return true;
    }
  }

  return false;
}

bool CharacterCollisionComponent::CheckBottomCollision(
    const glm::vec2 &position, const GridSystem &grid) {
  float left = position.x - (m_Owner->GetSize().x / 2) + 0.3f; // 添加容差
  float right = position.x + (m_Owner->GetSize().x / 2) - 0.3f;
  float bottom = position.y + CHARACTER_OFFSET;

  int checkPoints = 10;
  for (int i = 0; i <= checkPoints; i++) {
    float checkX =
        left + (right - left) * (static_cast<float>(i) / checkPoints);
    glm::ivec2 gridPosBottom =
        grid.GameToCellPosition(glm::vec2(checkX, bottom));
    CellType belowCell = grid.GetCell(gridPosBottom.x, gridPosBottom.y);

    if (belowCell != CellType::EMPTY) {
      // 調整Y位置以吸附到地板
      float cellBottomY =
          grid.CellToGamePosition(gridPosBottom.x, gridPosBottom.y).y;
      glm::vec2 pos = m_Owner->GetPosition();
      pos.y = cellBottomY + (grid.GetCellSize() / 2.0f) - CHARACTER_OFFSET;
      m_Owner->m_IsJumping = false;
      m_Owner->m_IsOnGround = true;
      m_Owner->m_JumpHeight = 0;
      m_Owner->SetPosition(pos);
      return true;
    }
  }

  return false;
}

glm::vec2 CharacterCollisionComponent::CheckAndAdjustSideCollisions(
    const glm::vec2 &position, const GridSystem &grid) {
  float tolerance = 0.3f;
  float left = position.x - (m_Owner->GetSize().x / 2) + tolerance;
  float right = position.x + (m_Owner->GetSize().x / 2) - tolerance;
  float middle = position.y + (m_Owner->GetSize().y / 2);

  glm::vec2 adjustedPos = position;
  int checkPoints = 20;

  // 檢查左側碰撞
  int leftCollisionCount = 0;
  glm::ivec2 leftCollisionCell(0, 0);

  for (int i = 0; i <= checkPoints; i++) {
    float checkY = middle + ((m_Owner->GetSize().y / 2) *
                                 (static_cast<float>(i) / checkPoints) -
                             m_Owner->GetSize().y / 4);
    glm::ivec2 gridPosLeft = grid.GameToCellPosition(glm::vec2(left, checkY));
    CellType leftCell = grid.GetCell(gridPosLeft.x, gridPosLeft.y);

    if (leftCell != CellType::EMPTY) {
      leftCollisionCount++;
      leftCollisionCell = gridPosLeft;
    }
  }

  // 檢查右側碰撞
  int rightCollisionCount = 0;
  glm::ivec2 rightCollisionCell(0, 0);

  for (int i = 0; i <= checkPoints; i++) {
    float checkY = middle + ((m_Owner->GetSize().y / 2) *
                                 (static_cast<float>(i) / checkPoints) -
                             m_Owner->GetSize().y / 4);
    glm::ivec2 gridPosRight = grid.GameToCellPosition(glm::vec2(right, checkY));
    CellType rightCell = grid.GetCell(gridPosRight.x, gridPosRight.y);

    if (rightCell != CellType::EMPTY) {
      rightCollisionCount++;
      rightCollisionCell = gridPosRight;
    }
  }

  // 調整位置以避免碰撞
  int collisionThreshold = 2;
  if (leftCollisionCount >= collisionThreshold) {
    float cellRightEdge =
        grid.CellToGamePosition(leftCollisionCell.x, leftCollisionCell.y).x +
        (grid.GetCellSize() / 2.0f);
    adjustedPos.x = cellRightEdge + (m_Owner->GetSize().x / 2) - tolerance / 2;
  } else if (rightCollisionCount >= collisionThreshold) {
    float cellLeftEdge =
        grid.CellToGamePosition(rightCollisionCell.x, rightCollisionCell.y).x -
        (grid.GetCellSize() / 2.0f);
    adjustedPos.x = cellLeftEdge - (m_Owner->GetSize().x / 2) + tolerance / 2;
  }

  return adjustedPos;
}

void CharacterCollisionComponent::AdjustPositionToAvoidWalls(
    glm::vec2 &position, const GridSystem &grid) {
  float bodyLeft = position.x - (m_Owner->GetSize().x / 2.0f);
  float bodyRight = position.x + (m_Owner->GetSize().x / 2.0f);
  float bodyTop = position.y + m_Owner->GetSize().y;
  float bodyBottom = position.y + CHARACTER_OFFSET;

  // 增加更多的檢查點來檢測身體兩側
  int verticalCheckPoints = 8; // 從底部到頂部檢查8個點

  // 檢查左側身體的多個點
  bool leftSideBlocked = false;
  glm::ivec2 leftBlockingCell(0, 0);

  for (int i = 0; i < verticalCheckPoints; i++) {
    float checkY =
        bodyBottom + (bodyTop - bodyBottom) *
                         (static_cast<float>(i) / (verticalCheckPoints - 1));
    glm::ivec2 leftBodyCell =
        grid.GameToCellPosition(glm::vec2(bodyLeft, checkY));
    CellType leftCellType = grid.GetCell(leftBodyCell.x, leftBodyCell.y);

    if (!grid.CanMoveOn(leftCellType, m_Owner->IsFireboy())) {
      leftSideBlocked = true;
      leftBlockingCell = leftBodyCell;
      break; // 找到第一個阻擋的格子就停止
    }
  }

  // 檢查右側身體的多個點
  bool rightSideBlocked = false;
  glm::ivec2 rightBlockingCell(0, 0);

  for (int i = 0; i < verticalCheckPoints; i++) {
    float checkY =
        bodyBottom + (bodyTop - bodyBottom) *
                         (static_cast<float>(i) / (verticalCheckPoints - 1));
    glm::ivec2 rightBodyCell =
        grid.GameToCellPosition(glm::vec2(bodyRight, checkY));
    CellType rightCellType = grid.GetCell(rightBodyCell.x, rightBodyCell.y);

    if (!grid.CanMoveOn(rightCellType, m_Owner->IsFireboy())) {
      rightSideBlocked = true;
      rightBlockingCell = rightBodyCell;
      break; // 找到第一個阻擋的格子就停止
    }
  }

  // 根據檢測結果調整位置
  if (leftSideBlocked && !rightSideBlocked) {
    // 左側被阻擋，向右調整
    float cellRightEdge =
        grid.CellToGamePosition(leftBlockingCell.x, leftBlockingCell.y).x +
        (grid.GetCellSize() / 2.0f);
    position.x = cellRightEdge + (m_Owner->GetSize().x / 2.0f) +
                 0.1f; // 添加小幅偏移避免貼邊
  } else if (rightSideBlocked && !leftSideBlocked) {
    // 右側被阻擋，向左調整
    float cellLeftEdge =
        grid.CellToGamePosition(rightBlockingCell.x, rightBlockingCell.y).x -
        (grid.GetCellSize() / 2.0f);
    position.x = cellLeftEdge - (m_Owner->GetSize().x / 2.0f) -
                 0.1f; // 添加小幅偏移避免貼邊
  } else if (leftSideBlocked && rightSideBlocked) {
    // 兩側都被阻擋，尋找最近的安全位置
    float leftDistance = std::abs(
        position.x -
        (grid.CellToGamePosition(leftBlockingCell.x, leftBlockingCell.y).x +
         (grid.GetCellSize() / 2.0f) + (m_Owner->GetSize().x / 2.0f)));
    float rightDistance = std::abs(
        position.x -
        (grid.CellToGamePosition(rightBlockingCell.x, rightBlockingCell.y).x -
         (grid.GetCellSize() / 2.0f) - (m_Owner->GetSize().x / 2.0f)));

    if (leftDistance < rightDistance) {
      // 向右側調整（離右邊牆較近）
      float cellRightEdge =
          grid.CellToGamePosition(leftBlockingCell.x, leftBlockingCell.y).x +
          (grid.GetCellSize() / 2.0f);
      position.x = cellRightEdge + (m_Owner->GetSize().x / 2.0f) + 0.1f;
    } else {
      // 向左側調整（離左邊牆較近）
      float cellLeftEdge =
          grid.CellToGamePosition(rightBlockingCell.x, rightBlockingCell.y).x -
          (grid.GetCellSize() / 2.0f);
      position.x = cellLeftEdge - (m_Owner->GetSize().x / 2.0f) - 0.1f;
    }
  }
}

bool CharacterCollisionComponent::CheckStandingOnPlatform(
    const glm::vec2 &position, std::shared_ptr<Platform> &outPlatform) {
  glm::vec2 currentPos = m_Owner->GetPosition();

  // 暫時設定到測試位置
  m_Owner->SetPosition(position);

  bool isStanding = false;
  for (const auto &platform : m_Owner->m_Platforms) {
    if (platform->IsCharacterOn(m_Owner)) {
      isStanding = true;
      outPlatform = platform;
      break;
    }
  }

  // 恢復原位置
  m_Owner->SetPosition(currentPos);
  return isStanding;
}

glm::vec2 CharacterCollisionComponent::AdjustPositionForPlatform(
    const glm::vec2 &position, int moveDirection) {
  glm::vec2 adjustedPos = position;
  glm::vec2 currentPos = m_Owner->GetPosition();

  // 暫時設定到測試位置
  m_Owner->SetPosition(position);

  for (const auto &platform : m_Owner->m_Platforms) {
    if (platform->CheckCollision(m_Owner, moveDirection)) {
      // 根據碰撞方向調整位置
      glm::vec2 platPos = platform->GetPosition();
      glm::vec2 platSize = platform->GetScaledSize();

      if (moveDirection > 0) {
        // 從左側碰撞
        float platLeft = platPos.x - (platSize.x / 2);
        adjustedPos.x = platLeft - (m_Owner->GetSize().x / 2) - 0.1f;
      } else if (moveDirection < 0) {
        // 從右側碰撞
        float platRight = platPos.x + (platSize.x / 2);
        adjustedPos.x = platRight + (m_Owner->GetSize().x / 2) + 0.1f;
      }
      break;
    }
  }

  // 恢復原位置
  m_Owner->SetPosition(currentPos);
  return adjustedPos;
}

bool CharacterCollisionComponent::IsCollidingWithPlatformBottom(
    Platform *platform, const glm::vec2 &charPos) const {
  // 取得平台的位置和大小
  glm::vec2 platPos = platform->GetPosition();
  glm::vec2 platSize = platform->GetScaledSize();

  // 角色的上邊界
  float charTop = charPos.y + m_Owner->GetSize().y;

  // 平台的下邊界
  float platBottom = platPos.y - (platSize.y / 2);

  // 如果角色頂部接觸平台底部，且水平位置有重疊，則發生碰撞
  bool verticalCollision = std::abs(charTop - platBottom) < 5.0f;
  bool horizontalOverlap =
      charPos.x + (m_Owner->GetSize().x / 2) > platPos.x - (platSize.x / 2) &&
      charPos.x - (m_Owner->GetSize().x / 2) < platPos.x + (platSize.x / 2);

  return verticalCollision && horizontalOverlap;
}