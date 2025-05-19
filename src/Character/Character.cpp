#include "Character/Character.hpp"
#include "Mechanism/Fan.hpp"
#include <cmath>
#include <iostream>
#include <map>

class Fan;

// 構造函數和基本初始化
Character::Character(const std::string &imagePath, const float zindex)
    : GameObject(std::make_shared<Util::Image>(imagePath), zindex),
      m_ImagePath(imagePath), m_FacingRight(true), isMoving(false),
      currentSprite(false), m_IsJumping(false), m_JumpHeight(0),
      m_JumpMaxHeight(90), m_IsOnGround(true), m_UpKeyWasPressed(false) {

  m_Size = GetScaledSize();
  SetPivot(glm::vec2(0.0f, -m_Size.y / 2 - 13.5));
}

//==================================
// 基本 getter 方法實現
//==================================
const std::string &Character::GetImagePath() const { return m_ImagePath; }
const glm::vec2 &Character::GetPosition() const {
  return m_Transform.translation;
}
bool Character::GetVisibility() const { return m_Visible; }
bool Character::IsJumping() const { return m_IsJumping; }
bool Character::IsOnGround() const { return m_IsOnGround; }
bool Character::IsMoving() const { return isMoving; }
bool Character::IsFacingRight() const { return m_FacingRight; }
bool Character::IsDead() const { return m_IsDead; }
glm::vec2 Character::GetSize() const { return m_Size; }
bool Character::IsStandingOnPlatform() const { return m_IsStandingOnPlatform; }
std::shared_ptr<Platform> Character::GetCurrentPlatform() const {
  return m_CurrentPlatform;
}
const glm::vec2 &Character::GetExternalForce() const { return m_ExternalForce; }
bool Character::IsAffectedByWind() const { return m_AffectedByWind; }

//==================================
// 角色狀態設定方法實現
//==================================
void Character::SetImage(const std::string &imagePath) {
  m_ImagePath = imagePath;
  SetDrawable(std::make_shared<Util::Image>(imagePath));

  // 應用水平翻轉
  ApplyFlip();
}

void Character::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

void Character::SetSpawnPoint(const glm::vec2 &spawn) { m_SpawnPoint = spawn; }

void Character::SetPreviousPosition() {
  m_LastPosition = m_Transform.translation;
}

void Character::UndoMovement() { m_Transform.translation = m_LastPosition; }

void Character::SetStandingOnPlatform(bool value) {
  m_IsStandingOnPlatform = value;
}

void Character::SetPlatforms(
    const std::vector<std::shared_ptr<Platform>> &platforms) {
  m_Platforms = platforms;
}

void Character::SetBoxes(const std::vector<std::shared_ptr<Box>> &boxes) {
  m_Boxes = boxes;
}

void Character::SetGridSystem(GridSystem *grid) { m_GridRef = grid; }

void Character::SetAffectedByWind(bool affected) {
  m_AffectedByWind = affected;
}

//==================================
// 角色移動和物理相關方法實現
//==================================
void Character::Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
                     bool isFireboy) {
  isMoving = (deltaX != 0);

  if (deltaX < 0) {
    m_FacingRight = false;
  } else if (deltaX > 0) {
    m_FacingRight = true;
  }

  ApplyFlip();

  // 提高大距離移動時的碰撞檢測精度
  int moveDirection = (deltaX > 0) ? 1 : ((deltaX < 0) ? -1 : 0);
  int moveAmount = std::abs(deltaX);

  // 針對跳躍時的移動，使用更小的步進值
  int step = (m_IsJumping && moveAmount > 5) ? 1 : 1;

  for (int i = 0; i < moveAmount; i += step) {
    glm::vec2 newPos = GetPosition();
    // 不要超過實際的移動距離
    int actualStep = std::min(step, moveAmount - i);
    newPos.x += moveDirection * actualStep;

    // 檢查新位置是否會與網格碰撞
    if (grid.CheckCollision(newPos, m_Size, isFireboy, moveDirection)) {
      break; // 碰壁了，停止移動
    }

    // 檢查平台碰撞
    bool platformCollision = false;
    for (const auto &platform : m_Platforms) {
      if (platform->CheckCollision(this, moveDirection)) {
        platformCollision = true;
        break;
      }
    }

    if (platformCollision) {
      break; // 碰到平台，停止移動
    }

    // 沒碰壁，更新位置
    SetPosition(newPos);
  }

  // 處理跳躍邏輯
  if (upKeyPressed && !m_UpKeyWasPressed && m_IsOnGround) {
    m_IsJumping = true;
    m_IsOnGround = false;
    m_JumpHeight = 0;
  }

  m_UpKeyWasPressed = upKeyPressed;
  UpdateAnimation();
}

void Character::MoveWithCollision(const glm::vec2 &offset,
                                  const GridSystem &grid) {
  glm::vec2 pos = GetPosition();
  int steps = static_cast<int>(glm::ceil(glm::length(offset) / 2.0f));
  if (steps == 0)
    steps = 1;

  glm::vec2 stepVec = offset / static_cast<float>(steps);

  // 確定移動方向
  int moveDirection = 0;

  // 如果水平移動值接近0，使用角色朝向決定側面碰撞檢測方向
  if (std::abs(offset.x) < 0.0001f) {
    moveDirection = IsFacingRight() ? 1 : -1;
  }

  for (int i = 0; i < steps; ++i) {
    glm::vec2 nextPos = pos + stepVec;
    bool blocked = false;

    // 計算角色在下一位置的邊界
    float charLeft = nextPos.x - m_Size.x / 2.0f;
    float charRight = nextPos.x + m_Size.x / 2.0f;
    float charTop = nextPos.y + m_Size.y;
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
      if (grid.CanStandOn(cell, IsFireboy())) {
        // 判斷是從上方行走還是從側面碰撞
        std::cout << "charBottom: " << charBottom
                  << ", cellTop: " << (cellTop - 20.0f) << std::endl;
        // 1. 角色底部高於地板頂部，視為行走狀態，允許移動
        if (charBottom >= cellTop - 20.0f) { // 添加一點容差
          std::cout << "Allow move on floor" << std::endl;
          continue; // 允許移動
        }

        // 2. 側面碰撞檢測
        if (moveDirection < 0 && checkPos.x <= cellRight) {
          std::cout << "Blocked on left side" << std::endl;
          // 向左移動且撞到地板右側
          blocked = true;
          break;
        } else if (moveDirection > 0 && checkPos.x >= cellLeft) {
          // 向右移動且撞到地板左側
          std::cout << "Blocked on right side" << std::endl;
          blocked = true;
          break;
        }
      }
      // 其他不可通行的格子
      else if (!grid.CanMoveOn(cell, IsFireboy())) {
        blocked = true;
        break;
      }
    }

    if (blocked) {
      // 碰撞：風力重置、停止移動
      if (std::abs(offset.y) > 0.001f) { // 只有垂直移動時才重置風力
        ResetExternalForce();
        SetAffectedByWind(false);
      }
      break;
    } else {
      // 沒有碰撞，更新位置
      pos = nextPos;
    }
  }

  SetPosition(pos);
}

void Character::Translate(const glm::vec2 &offset) {
  m_Transform.translation += offset;
}

//==================================
// 角色跳躍和重力相關方法實現
//==================================
void Character::UpdateJump(const GridSystem &grid) {
  // Only reset jump state when character is jumping, standing on platform,
  // and jump height > 0
  if (m_IsJumping && m_IsStandingOnPlatform && m_JumpHeight > 0) {
    m_IsJumping = false;
    m_Velocity.y = 0;
    return; // Already landed on platform, reset jump state
  }

  if (m_IsJumping) {
    glm::vec2 pos = GetPosition();
    float fallSpeed = 5.0f; // Fall speed
    float jumpSpeed = 7.0f; // Jump speed

    glm::vec2 nextPos = pos;

    // Rising phase
    if (m_JumpHeight < m_JumpMaxHeight) {
      nextPos.y += jumpSpeed; // Try to jump up

      // Define the four corners of the character box (after rising)
      float tolerance = 0.3f; // Small tolerance to avoid false collisions
      float left = nextPos.x - (m_Size.x / 2) + tolerance;
      float right = nextPos.x + (m_Size.x / 2) - tolerance;
      float top = nextPos.y + m_Size.y;
      float middle =
          nextPos.y + (m_Size.y / 2); // Middle height for side collision

      // Check collision along the top edge
      bool topCollision = false;
      int checkPoints = 10; // Number of check points for top
      for (int i = 0; i <= checkPoints; i++) {
        float checkX =
            left + (right - left) * (static_cast<float>(i) / checkPoints);
        glm::ivec2 gridPosTop = grid.GameToCellPosition(glm::vec2(checkX, top));
        CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

        if (aboveCell != CellType::EMPTY) {
          topCollision = true;
          break;
        }
      }

      // Using collision threshold for sides
      int leftCollisionCount = 0;
      int rightCollisionCount = 0;
      int collisionThreshold =
          2; // Need at least this many collision points to count as a collision

      // Check collision along the left side
      checkPoints = 20; // Reduced number of check points for sides
      glm::ivec2 leftCollisionCell(0, 0);

      for (int i = 0; i <= checkPoints; i++) {
        float checkY =
            middle + ((m_Size.y / 2) * (static_cast<float>(i) / checkPoints) -
                      m_Size.y / 4);
        glm::ivec2 gridPosLeft =
            grid.GameToCellPosition(glm::vec2(left, checkY));
        CellType leftCell = grid.GetCell(gridPosLeft.x, gridPosLeft.y);

        if (!topCollision && leftCell != CellType::EMPTY) {
          leftCollisionCount++;
          leftCollisionCell = gridPosLeft; // Save for position correction
        }
      }

      // Check collision along the right side
      glm::ivec2 rightCollisionCell(0, 0);

      for (int i = 0; i <= checkPoints; i++) {
        float checkY =
            middle + ((m_Size.y / 2) * (static_cast<float>(i) / checkPoints) -
                      m_Size.y / 4);
        glm::ivec2 gridPosRight =
            grid.GameToCellPosition(glm::vec2(right, checkY));
        CellType rightCell = grid.GetCell(gridPosRight.x, gridPosRight.y);

        if (!topCollision && rightCell != CellType::EMPTY) {
          rightCollisionCount++;
          rightCollisionCell = gridPosRight; // Save for position correction
        }
      }

      // Apply side collision corrections if threshold is met
      if (leftCollisionCount >= collisionThreshold) {
        // Adjust position to avoid penetrating the wall
        float cellRightEdge =
            grid.CellToGamePosition(leftCollisionCell.x, leftCollisionCell.y)
                .x +
            (grid.GetCellSize() / 2.0f);
        nextPos.x = cellRightEdge + (m_Size.x / 2) - tolerance / 2;
      } else if (rightCollisionCount >= collisionThreshold) {
        // Adjust position to avoid penetrating the wall
        float cellLeftEdge =
            grid.CellToGamePosition(rightCollisionCell.x, rightCollisionCell.y)
                .x -
            (grid.GetCellSize() / 2.0f);
        nextPos.x = cellLeftEdge - (m_Size.x / 2) + tolerance / 2;
      }

      // 檢查與平台的碰撞 (新增)
      int moveDirection = 0;
      if (nextPos.x > pos.x)
        moveDirection = 1;
      else if (nextPos.x < pos.x)
        moveDirection = -1;

      for (const auto &platform : m_Platforms) {
        // 儲存當前位置
        glm::vec2 currentPos = GetPosition();

        // 暫時將角色移動到下一個位置進行碰撞檢測
        SetPosition(nextPos);

        if (platform->CheckCollision(this, moveDirection)) {

          // 還原原始位置
          SetPosition(currentPos);

          // 根據碰撞調整位置
          glm::vec2 platPos = platform->GetPosition();
          glm::vec2 platSize = platform->GetScaledSize();

          if (moveDirection > 0) {
            // 從左側碰撞
            float platLeft = platPos.x - (platSize.x / 2);
            nextPos.x = platLeft - (m_Size.x / 2) - 0.1f;
          } else if (moveDirection < 0) {
            // 從右側碰撞
            float platRight = platPos.x + (platSize.x / 2);
            nextPos.x = platRight + (m_Size.x / 2) + 0.1f;
          }
          break;
        }

        // 還原原始位置
        SetPosition(currentPos);
      }

      if (topCollision) {
        // Hit ceiling - start falling
        m_JumpHeight += isMoving ? (jumpSpeed / 1.5f) : m_JumpMaxHeight;
        return;
      }

      pos = nextPos;
      m_JumpHeight += jumpSpeed;
    }
    // Falling phase
    else {
      nextPos.y -= fallSpeed; // Try to descend

      // Define the character box corners (after descending)
      float tolerance = 0.3f; // Small tolerance to avoid false collisions
      float left = nextPos.x - (m_Size.x / 2) + tolerance;
      float right = nextPos.x + (m_Size.x / 2) - tolerance;
      float bottom = nextPos.y + 13.5f;
      float middle =
          nextPos.y + (m_Size.y / 2); // Middle height for side collision

      // Check collision along the bottom edge
      bool bottomCollision = false;
      int checkPoints = 10; // Number of check points for bottom
      for (int i = 0; i <= checkPoints; i++) {
        float checkX =
            left + (right - left) * (static_cast<float>(i) / checkPoints);
        glm::ivec2 gridPosBottom =
            grid.GameToCellPosition(glm::vec2(checkX, bottom));
        CellType belowCell = grid.GetCell(gridPosBottom.x, gridPosBottom.y);

        if (belowCell != CellType::EMPTY) {
          bottomCollision = true;

          // Adjust Y position to snap to the floor
          float cellBottomY =
              grid.CellToGamePosition(gridPosBottom.x, gridPosBottom.y).y;
          pos.y = cellBottomY + (grid.GetCellSize() / 2.0f) - 13.5f;
          m_IsJumping = false;
          m_IsOnGround = true;
          m_JumpHeight = 0;
          SetPosition(pos);
          return;
        }
      }

      // Using collision threshold for sides during falling
      int leftCollisionCount = 0;
      int rightCollisionCount = 0;
      int collisionThreshold =
          2; // Need at least this many collision points to count as a collision

      // Check collision along the left side while falling
      checkPoints = 10; // Reduced number of check points for sides
      glm::ivec2 leftCollisionCell(0, 0);

      for (int i = 0; i <= checkPoints; i++) {
        float checkY =
            middle + ((m_Size.y / 2) * (static_cast<float>(i) / checkPoints) -
                      m_Size.y / 4);
        glm::ivec2 gridPosLeft =
            grid.GameToCellPosition(glm::vec2(left, checkY));
        CellType leftCell = grid.GetCell(gridPosLeft.x, gridPosLeft.y);

        if (!bottomCollision && leftCell != CellType::EMPTY) {
          leftCollisionCount++;
          leftCollisionCell = gridPosLeft; // Save for position correction
        }
      }

      // Check collision along the right side while falling
      checkPoints = 20;
      glm::ivec2 rightCollisionCell(0, 0);

      for (int i = 0; i <= checkPoints; i++) {
        float checkY =
            middle + ((m_Size.y / 2) * (static_cast<float>(i) / checkPoints) -
                      m_Size.y / 4);
        glm::ivec2 gridPosRight =
            grid.GameToCellPosition(glm::vec2(right, checkY));
        CellType rightCell = grid.GetCell(gridPosRight.x, gridPosRight.y);

        if (!bottomCollision && rightCell != CellType::EMPTY) {
          rightCollisionCount++;
          rightCollisionCell = gridPosRight; // Save for position correction
        }
      }

      // Apply side collision corrections if threshold is met
      if (leftCollisionCount >= collisionThreshold) {
        // Adjust position to avoid penetrating the wall
        float cellRightEdge =
            grid.CellToGamePosition(leftCollisionCell.x, leftCollisionCell.y)
                .x +
            (grid.GetCellSize() / 2.0f);
        nextPos.x = cellRightEdge + (m_Size.x / 2) - tolerance / 2;
      } else if (rightCollisionCount >= collisionThreshold) {
        // Adjust position to avoid penetrating the wall
        float cellLeftEdge =
            grid.CellToGamePosition(rightCollisionCell.x, rightCollisionCell.y)
                .x -
            (grid.GetCellSize() / 2.0f);
        nextPos.x = cellLeftEdge - (m_Size.x / 2) + tolerance / 2;
      }

      // 檢查與平台的碰撞 (下落時) (新增)
      int moveDirection = 0;
      if (nextPos.x > pos.x)
        moveDirection = 1;
      else if (nextPos.x < pos.x)
        moveDirection = -1;

      for (const auto &platform : m_Platforms) {
        // 儲存當前位置
        glm::vec2 currentPos = GetPosition();

        // 暫時將角色移動到下一個位置進行碰撞檢測
        SetPosition(nextPos);

        if (platform->CheckCollision(this, moveDirection)) {

          // 還原原始位置
          SetPosition(currentPos);

          // 根據碰撞調整位置
          glm::vec2 platPos = platform->GetPosition();
          glm::vec2 platSize = platform->GetScaledSize();

          if (moveDirection > 0) {
            // 從左側碰撞
            float platLeft = platPos.x - (platSize.x / 2);
            nextPos.x = platLeft - (m_Size.x / 2) - 0.1f;
          } else if (moveDirection < 0) {
            // 從右側碰撞
            float platRight = platPos.x + (platSize.x / 2);
            nextPos.x = platRight + (m_Size.x / 2) + 0.1f;
          }
          break;
        }

        // 還原原始位置
        SetPosition(currentPos);
      }

      // 檢查角色是否著陸在平台上
      for (const auto &platform : m_Platforms) {
        // 儲存當前位置
        glm::vec2 currentPos = GetPosition();

        // 暫時將角色移動到下一個位置進行碰撞檢測
        SetPosition(nextPos);

        if (platform->IsCharacterOn(this)) {
          // 著陸在平台上
          glm::vec2 platPos = platform->GetPosition();

          // 還原原始位置
          SetPosition(currentPos);

          // 調整位置以正確站在平台上
          pos.y = platPos.y + 11.5f - 13.5f; // platTop - charBottomOffset
          m_IsJumping = false;
          m_IsOnGround = true;
          m_JumpHeight = 0;
          m_IsStandingOnPlatform = true;
          m_CurrentPlatform = platform;
          SetPosition(pos);
          return;
        }

        // 還原原始位置
        SetPosition(currentPos);
      }

      if (!bottomCollision) {
        pos = nextPos;
      }
    }

    SetPosition(pos);
  }
}

struct CellComparator {
  bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const {
    if (a.x != b.x)
      return a.x < b.x;
    return a.y < b.y;
  }
};

void Character::ApplyGravity(const GridSystem &grid) {
  if (m_IsStandingOnPlatform) {
    return; // 如果站在平台上，跳過重力處理
  }

  if (!m_IsJumping) {
    glm::vec2 pos = GetPosition();
    glm::vec2 nextPos = pos;
    float fallSpeed = 5.0f;
    nextPos.y -= fallSpeed;

    // 適當調整腳部寬度 - 使用角色寬度的1/3作為腳部寬度
    float footWidth = m_Size.x / 5.0f;

    // 計算腳的位置（底部中間）
    float footLeft = nextPos.x - (footWidth / 2);
    float footRight = nextPos.x + (footWidth / 2);

    // 使用精確的底部接觸點，添加小的向下偏移確保檢測正確
    float bottom = nextPos.y + 13.5f; // 添加1像素的偏移
    int checkPoints = 7;

    // 統計每個網格有多少檢查點可以站立
    std::map<glm::ivec2, int, CellComparator> standablePoints;

    for (int i = 0; i <= checkPoints; i++) {
      float checkX = footLeft + (footRight - footLeft) *
                                    (static_cast<float>(i) / checkPoints);
      glm::ivec2 gridPosBottom =
          grid.GameToCellPosition(glm::vec2(checkX, bottom));
      CellType belowCell = grid.GetCell(gridPosBottom.x, gridPosBottom.y);

      if (grid.CanStandOn(belowCell, this->IsFireboy())) {
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
      m_IsOnGround = true;
      float cellBottomY = grid.CellToGamePosition(bestGrid.x, bestGrid.y).y;
      pos.y = cellBottomY + (grid.GetCellSize() / 2.0f) - 13.5f;
    } else {
      // 站立點少於3個，設定為不在地面上並下落
      m_IsOnGround = false;
      pos = nextPos;

      // 檢查角色左右兩邊的身體位置，防止卡在牆中
      float bodyLeft = pos.x - (m_Size.x / 2.0f);
      float bodyRight = pos.x + (m_Size.x / 2.0f);
      float bodyMiddleY = pos.y; // 角色中間高度

      // 檢查左側身體
      glm::ivec2 leftBodyCell =
          grid.GameToCellPosition(glm::vec2(bodyLeft, bodyMiddleY));
      CellType leftCellType = grid.GetCell(leftBodyCell.x, leftBodyCell.y);

      // 檢查右側身體
      glm::ivec2 rightBodyCell =
          grid.GameToCellPosition(glm::vec2(bodyRight, bodyMiddleY));
      CellType rightCellType = grid.GetCell(rightBodyCell.x, rightBodyCell.y);

      // 如果左側或右側會碰到牆，調整X位置
      if (!grid.CanMoveOn(leftCellType, this->IsFireboy())) {
        // 如果左側碰牆，向右調整
        float cellRightEdge =
            grid.CellToGamePosition(leftBodyCell.x, leftBodyCell.y).x +
            (grid.GetCellSize() / 2.0f);
        pos.x = cellRightEdge + (m_Size.x / 2.0f);
      } else if (!grid.CanMoveOn(rightCellType, this->IsFireboy())) {
        // 如果右側碰牆，向左調整
        float cellLeftEdge =
            grid.CellToGamePosition(rightBodyCell.x, rightBodyCell.y).x -
            (grid.GetCellSize() / 2.0f);
        pos.x = cellLeftEdge - (m_Size.x / 2.0f);
      }
    }

    SetPosition(pos);
  }
}

void Character::ApplyFlip() {
  if (m_FacingRight) {
    m_Transform.scale.x = std::abs(m_Transform.scale.x); // 確保正向
  } else {
    m_Transform.scale.x = -std::abs(m_Transform.scale.x); // 確保負向
  }
}

void Character::Die() {
  m_IsDead = true;
  SetPosition(m_SpawnPoint);
}

bool Character::IsDead() const { return m_IsDead; }

void Character::Respawn() {
  m_IsDead = false;
  SetPosition(m_SpawnPoint);
}

void Character::SetSpawnPoint(const glm::vec2 &spawn) { m_SpawnPoint = spawn; }

void Character::SetPreviousPosition() {
  m_LastPosition = m_Transform.translation;
}

void Character::UndoMovement() { m_Transform.translation = m_LastPosition; }

void Character::Translate(const glm::vec2 &offset) {
  m_Transform.translation += offset;
}

void Character::SetStandingOnPlatform(bool value) {
  m_IsStandingOnPlatform = value;
}

void Character::SetPlatforms(
    const std::vector<std::shared_ptr<Platform>> &platforms) {
  m_Platforms = platforms;
}

bool Character::IsMoving() const { return isMoving; }

bool Character::IsFacingRight() const { return m_FacingRight; }

void Character::SetBoxes(const std::vector<std::shared_ptr<Box>> &boxes) {
  m_Boxes = boxes;
}

void Character::ApplyExternalForce(float y) {
  // 添加風力
  m_ExternalForce.y += y;

  // 限制最大風力
  if (m_ExternalForce.y > MAX_EXTERNAL_FORCE) {
    m_ExternalForce.y = MAX_EXTERNAL_FORCE;
  }

  m_AffectedByWind = true;
}

void Character::ResetExternalForce() { m_ExternalForce = {0.0f, 0.0f}; }

void Character::Update() {
  if (m_AffectedByWind) {
    if (std::abs(m_ExternalForce.y) > 0.001f && m_GridRef) {
      float floatEffect = Fan::GetWindFloatEffect();
      float windAmount = m_ExternalForce.y * floatEffect * 0.2f;

      // 避免低空飛行硬升高（給點初始推力）
      if (GetPosition().y < 200.0f) {
        windAmount += 2.0f;
      }

      glm::vec2 offset(0.0f, windAmount);
      MoveWithCollision(offset, *m_GridRef);
    }
  }

  // 紀錄之前的platform狀態
  bool wasOnPlatform = m_IsStandingOnPlatform;
  std::shared_ptr<Platform> previousPlatform = m_CurrentPlatform;

  m_IsStandingOnPlatform = false;
  m_CurrentPlatform = nullptr;

  // 首先檢查角色是否站在箱子上
  bool standingOnBox = false;
  for (const auto &box : m_Boxes) {
    if (box->IsCharacterOn(this)) {
      standingOnBox = true;
      m_IsStandingOnPlatform = true;
      m_IsOnGround = true;

      // 調整角色位置以完全站在箱子上
      glm::vec2 pos = GetPosition();
      glm::vec2 boxPos = box->GetPosition();
      glm::vec2 boxSize = box->GetSize();
      pos.y = boxPos.y + boxSize.y - 13.5f;
      SetPosition(pos);
      break;
    }
  }

  // 如果沒有站在箱子上，再檢查是否站在平台上
  if (!standingOnBox) {
    // 檢查角色是否站在任何平台上
    for (const auto &platform : m_Platforms) {
      if (platform->IsCharacterOn(this)) {
        // 找到了角色站立的平台
        m_IsStandingOnPlatform = true;
        m_IsOnGround = true;
        m_CurrentPlatform = platform;

        // 只有首次接觸平台時才調整位置
        if (!wasOnPlatform || previousPlatform != platform) {
          glm::vec2 pos = GetPosition();
          glm::vec2 platPos = platform->GetPosition();
          // 使用準確的偏移量，將角色底部對齊平台頂部
          pos.y = platPos.y + 11.5f - 13.5f; // platTop - charBottomOffset
          SetPosition(pos);
        }

        break;
      }
    }
  }

  // 如果角色站在平台上，應用平台的移動
  if (m_IsStandingOnPlatform && m_CurrentPlatform) {
    // 直接使用平台提供的移動量
    glm::vec2 platMove = m_CurrentPlatform->GetDeltaMovement();
    if (glm::length(platMove) > 0.01f) { // 若有實際移動才應用
      // Translate(platMove);
    }
  }
}
