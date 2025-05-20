#include "Character/Character.hpp"
#include "Mechanism/Fan.hpp"
#include <cmath>
#include <iostream>
#include <map>

#define CHARACTER_OFFSET 13.5f

CharacterMovementComponent::CharacterMovementComponent(Character *owner)
    : m_Owner(owner) {}

void CharacterMovementComponent::Move(int deltaX, bool upKeyPressed,
                                      const GridSystem &grid, bool isFireboy) {
  m_Owner->isMoving = (deltaX != 0);

  if (deltaX < 0) {
    m_Owner->m_FacingRight = false;
  } else if (deltaX > 0) {
    m_Owner->m_FacingRight = true;
  }

  m_Owner->ApplyFlip();

  // 提高大距離移動時的碰撞檢測精度
  int moveDirection = (deltaX > 0) ? 1 : ((deltaX < 0) ? -1 : 0);
  int moveAmount = std::abs(deltaX);

  // 針對跳躍時的移動，使用更小的步進值
  int step = (m_Owner->m_IsJumping && moveAmount > 5) ? 1 : 1;

  for (int i = 0; i < moveAmount; i += step) {
    glm::vec2 newPos = m_Owner->GetPosition();
    // 不要超過實際的移動距離
    int actualStep = std::min(step, moveAmount - i);
    newPos.x += moveDirection * actualStep;

    // 檢查新位置是否會與網格碰撞
    if (grid.CheckCollision(newPos, m_Owner->GetSize(), isFireboy,
                            moveDirection)) {
      break; // 碰壁了，停止移動
    }

    // 檢查平台碰撞
    bool platformCollision = false;
    for (const auto &platform : m_Owner->m_Platforms) {
      if (platform->CheckCollision(m_Owner, moveDirection)) {
        platformCollision = true;
        break;
      }
    }

    if (platformCollision) {
      break; // 碰到平台，停止移動
    }

    // 沒碰壁，更新位置
    m_Owner->SetPosition(newPos);
  }

  // 處理跳躍邏輯
  if (upKeyPressed && !m_Owner->m_UpKeyWasPressed && m_Owner->m_IsOnGround &&
      !m_Owner->IsAffectedByWind()) {
    m_Owner->m_IsJumping = true;
    m_Owner->m_IsOnGround = false;
    m_Owner->m_JumpHeight = 0;
  }

  m_Owner->m_UpKeyWasPressed = upKeyPressed;
  m_Owner->UpdateAnimation();
}

void CharacterMovementComponent::MoveWithCollision(const glm::vec2 &offset,
                                                   const GridSystem &grid) {
  glm::vec2 pos = m_Owner->GetPosition();
  int steps = static_cast<int>(glm::ceil(glm::length(offset) / 2.0f));
  if (steps == 0)
    steps = 1;

  glm::vec2 stepVec = offset / static_cast<float>(steps);

  // 確定移動方向
  int moveDirection = 0;

  // 如果水平移動值接近0，使用角色朝向決定側面碰撞檢測方向
  if (std::abs(offset.x) < 0.0001f) {
    moveDirection = m_Owner->IsFacingRight() ? 1 : -1;
  }

  for (int i = 0; i < steps; ++i) {
    glm::vec2 nextPos = pos + stepVec;
    bool blocked = false;

    // 計算角色在下一位置的邊界
    float charLeft = nextPos.x - m_Owner->GetSize().x / 2.0f;
    float charRight = nextPos.x + m_Owner->GetSize().x / 2.0f;
    float charTop = nextPos.y + m_Owner->GetSize().y;
    float charBottom = nextPos.y + 13.5f;

    // 主要檢查點：左邊界、右邊界、底部
    std::vector<glm::vec2> checkPoints;

    // 根據移動方向優先檢查相應的邊界
    if (moveDirection < 0) {
      // 向左移動：檢查左邊界的多個點
      for (int j = 0; j <= 6; ++j) {
        float checkY = charBottom + (charTop - charBottom) * (j / 6.0f);
        checkPoints.push_back({charLeft, checkY});
      }
    } else if (moveDirection > 0) {
      // 向右移動：檢查右邊界的多個點
      for (int j = 0; j <= 6; ++j) {
        float checkY = charBottom + (charTop - charBottom) * (j / 6.0f);
        checkPoints.push_back({charRight, checkY});
      }
    }

    // 檢查底部（無論移動方向）
    for (int j = 0; j <= 4; ++j) {
      float checkX = charLeft + (charRight - charLeft) * (j / 4.0f);
      checkPoints.push_back({checkX, charBottom});
    }

    // 檢查頂部（無論移動方向）- 特別是對於垂直移動
    if (std::abs(stepVec.y) > 0.001f) {
      for (int j = 0; j <= 4; ++j) {
        float checkX = charLeft + (charRight - charLeft) * (j / 4.0f);
        checkPoints.push_back({checkX, charTop});
      }
    }

    // 遍歷所有檢查點
    for (const auto &checkPos : checkPoints) {
      glm::ivec2 gridPos = grid.GameToCellPosition(checkPos);
      CellType cell = grid.GetCell(gridPos.x, gridPos.y);

      // 取得網格中心點和大小
      glm::vec2 cellCenter = grid.CellToGamePosition(gridPos.x, gridPos.y);
      float cellSize = grid.GetCellSize();

      // 計算網格的上下左右邊緣
      float cellLeft = cellCenter.x - cellSize / 2.0f;
      float cellRight = cellCenter.x + cellSize / 2.0f;
      float cellTop = cellCenter.y + cellSize / 2.0f;
      // float cellBottom = cellCenter.y - cellSize / 2.0f;

      // 如果是地板類型，需特殊處理
      if (grid.CanStandOn(cell, m_Owner->IsFireboy())) {
        // 判斷是從上方行走還是從側面碰撞
        // std::cout << "charBottom: " << charBottom
        //           << ", cellTop: " << (cellTop - 20.0f) << std::endl;
        // 1. 角色底部高於地板頂部，視為行走狀態，允許移動
        if (charBottom >= cellTop - 20.0f) { // 添加一點容差
          std::cout << "Allow move on floor" << std::endl;
          continue; // 允許移動
        }

        // 2. 側面碰撞檢測
        if (moveDirection < 0 && checkPos.x <= cellRight) {
          std::cout << "(left move) Blocked on floor right side" << std::endl;
          // 向左移動且撞到地板右側
          blocked = true;
          break;
        } else if (moveDirection > 0 && checkPos.x >= cellLeft) {
          // 向右移動且撞到地板左側
          std::cout << "(right move) Blocked on floor left side" << std::endl;
          blocked = true;
          break;
        }
      }
      // 其他不可通行的格子
      else if (!grid.CanMoveOn(cell, m_Owner->IsFireboy())) {
        blocked = true;
        break;
      }
    }

    if (blocked) {
      // 碰撞：風力重置、停止移動
      if (std::abs(offset.y) > 0.001f) { // 只有垂直移動時才重置風力
        m_Owner->ResetExternalForce();
        m_Owner->SetAffectedByWind(false);
      }
      break;
    } else {
      // 沒有碰撞，更新位置
      pos = nextPos;
    }
  }

  m_Owner->SetPosition(pos);
}

void CharacterMovementComponent::ApplyGravity(const GridSystem &grid) {
  if (m_Owner->m_IsStandingOnPlatform) {
    return; // 如果站在平台上，跳過重力處理
  }

  if (!m_Owner->m_IsJumping) {
    glm::vec2 pos = m_Owner->GetPosition();
    glm::vec2 nextPos = pos;
    float fallSpeed = 5.0f;
    nextPos.y -= fallSpeed;

    // 適當調整腳部寬度 - 使用角色寬度的1/3作為腳部寬度
    float footWidth = m_Owner->GetSize().x / 5.0f;

    // 計算腳的位置（底部中間）
    float footLeft = nextPos.x - (footWidth / 2);
    float footRight = nextPos.x + (footWidth / 2);

    float bottom = nextPos.y + CHARACTER_OFFSET;
    int checkPoints = 7;

    // Define CellComparator before using it
    struct CellComparator {
      bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const {
        if (a.x != b.x)
          return a.x < b.x;
        return a.y < b.y;
      }
    };

    // 統計每個網格有多少檢查點可以站立
    std::map<glm::ivec2, int, CellComparator> standablePoints;

    for (int i = 0; i <= checkPoints; i++) {
      float checkX = footLeft + (footRight - footLeft) *
                                    (static_cast<float>(i) / checkPoints);
      glm::ivec2 gridPosBottom =
          grid.GameToCellPosition(glm::vec2(checkX, bottom));
      CellType belowCell = grid.GetCell(gridPosBottom.x, gridPosBottom.y);

      if (grid.CanStandOn(belowCell, m_Owner->IsFireboy())) {
        standablePoints[gridPosBottom]++;
      }
    }

    // 找出最多點可站立的網格
    glm::ivec2 bestGrid(0, 0);
    int maxCount = 0;
    int minRequiredPoints = 3; // 至少需要幾個點才能站立

    for (const auto &pair : standablePoints) {
      if (pair.second > maxCount) {
        maxCount = pair.second;
        bestGrid = pair.first;
      }
    }

    // 如果有找到至少3個可站立點
    if (maxCount >= minRequiredPoints) {
      m_Owner->m_IsOnGround = true;
      float cellBottomY = grid.CellToGamePosition(bestGrid.x, bestGrid.y).y;
      pos.y = cellBottomY + (grid.GetCellSize() / 2.0f) - CHARACTER_OFFSET;
    } else {
      pos = nextPos;
      m_Owner->m_CollisionComponent->AdjustPositionToAvoidWalls(pos, grid);
    }

    m_Owner->SetPosition(pos);
  }
}

void CharacterMovementComponent::HandleJumpAscending(const GridSystem &grid) {
  glm::vec2 pos = m_Owner->GetPosition();
  float jumpSpeed = 7.0f;
  glm::vec2 nextPos = pos;
  nextPos.y += jumpSpeed;

  // 檢查頂部碰撞
  bool topCollision =
      m_Owner->m_CollisionComponent->CheckTopCollision(nextPos, grid);

  // 檢查側面碰撞並調整位置
  nextPos = m_Owner->m_CollisionComponent->CheckAndAdjustSideCollisions(nextPos,
                                                                        grid);

  // 檢查平台碰撞並調整位置
  int moveDirection = (nextPos.x > pos.x) ? 1 : ((nextPos.x < pos.x) ? -1 : 0);
  nextPos = m_Owner->m_CollisionComponent->AdjustPositionForPlatform(
      nextPos, moveDirection);

  if (topCollision) {
    // 撞到天花板 - 開始下落
    m_Owner->m_JumpHeight +=
        m_Owner->IsMoving() ? (jumpSpeed / 1.5f) : m_Owner->m_JumpMaxHeight;
    return;
  }

  pos = nextPos;
  m_Owner->m_JumpHeight += jumpSpeed;
  m_Owner->SetPosition(pos);
}

void CharacterMovementComponent::HandleJumpDescending(const GridSystem &grid) {
  glm::vec2 pos = m_Owner->GetPosition();
  float fallSpeed = 5.0f;
  glm::vec2 nextPos = pos;
  nextPos.y -= fallSpeed;

  // 檢查底部碰撞
  bool bottomCollision =
      m_Owner->m_CollisionComponent->CheckBottomCollision(nextPos, grid);
  if (bottomCollision) {
    return; // 已著陸
  }

  // 檢查側面碰撞並調整位置
  nextPos = m_Owner->m_CollisionComponent->CheckAndAdjustSideCollisions(nextPos,
                                                                        grid);

  // 檢查平台碰撞並調整位置
  int moveDirection = (nextPos.x > pos.x) ? 1 : ((nextPos.x < pos.x) ? -1 : 0);
  nextPos = m_Owner->m_CollisionComponent->AdjustPositionForPlatform(
      nextPos, moveDirection);

  // 檢查是否著陸在平台上
  std::shared_ptr<Platform> landingPlatform;
  if (m_Owner->m_CollisionComponent->CheckStandingOnPlatform(nextPos,
                                                             landingPlatform)) {
    HandleLandingOnPlatform(landingPlatform);
    return;
  }

  pos = nextPos;
  m_Owner->SetPosition(pos);
}

void CharacterMovementComponent::HandleLandingOnPlatform(
    std::shared_ptr<Platform> platform) {
  glm::vec2 pos = m_Owner->GetPosition();
  glm::vec2 platPos = platform->GetPosition();

  // 調整位置以正確站在平台上
  pos.y = platPos.y + 11.5f - CHARACTER_OFFSET; // platTop - charBottomOffset
  m_Owner->m_IsJumping = false;
  m_Owner->m_IsOnGround = true;
  m_Owner->m_JumpHeight = 0;
  m_Owner->m_IsStandingOnPlatform = true;
  m_Owner->m_CurrentPlatform = platform;
  m_Owner->SetPosition(pos);
}

void CharacterMovementComponent::UpdateJump(const GridSystem &grid) {
  // 如果角色已經站在平台上且已經跳躍，則重置跳躍狀態
  if (m_Owner->m_IsJumping && m_Owner->m_IsStandingOnPlatform &&
      m_Owner->m_JumpHeight > 0) {
    m_Owner->m_IsJumping = false;
    m_Owner->m_Velocity.y = 0;
    return;
  }

  if (m_Owner->m_IsJumping) {
    // 跳躍分為上升和下降兩個階段
    if (m_Owner->m_JumpHeight < m_Owner->m_JumpMaxHeight) {
      // 上升階段
      HandleJumpAscending(grid);
    } else {
      // 下降階段
      HandleJumpDescending(grid);
    }
  }
}