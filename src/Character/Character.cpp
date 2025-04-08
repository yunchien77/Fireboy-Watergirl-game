#include "Character/Character.hpp"
#include <cmath>
#include <iostream>

Character::Character(const std::string &imagePath, const float zindex = 30)
    : GameObject(std::make_shared<Util::Image>(imagePath), zindex),
      m_ImagePath(imagePath), isMoving(false), currentSprite(false),
      m_IsJumping(false), m_JumpHeight(0), m_JumpMaxHeight(90),
      m_IsOnGround(true), m_UpKeyWasPressed(false), m_FacingRight(true) {

  m_Size = GetScaledSize();
  SetPivot(glm::vec2(0.0f, -m_Size.y / 2 - 13.5));
}

const std::string &Character::GetImagePath() const { return m_ImagePath; }

const glm::vec2 &Character::GetPosition() const {
  return m_Transform.translation;
}

bool Character::GetVisibility() const { return m_Visible; }

bool Character::IsJumping() const { return m_IsJumping; }

bool Character::IsOnGround() const { return m_IsOnGround; }

void Character::SetImage(const std::string &imagePath) {
  m_ImagePath = imagePath;
  SetDrawable(std::make_shared<Util::Image>(imagePath));

  // 應用水平翻轉
  ApplyFlip();
}

void Character::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

glm::vec2 Character::GetSize() const { return m_Size; }

void Character::Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
                     bool isFireboy) {
  isMoving = (deltaX != 0);

  if (deltaX < 0) {
    m_FacingRight = false;
  } else if (deltaX > 0) {
    m_FacingRight = true;
  }

  ApplyFlip();

  // 嘗試水平移動
  glm::vec2 newPos = GetPosition();
  newPos.x += deltaX;

  // 只有當新位置不會碰撞時，才進行移動
  if (!grid.CheckCollision(newPos, m_Size, isFireboy, deltaX)) {
    SetPosition(newPos);
  }

  // 處理跳躍
  // 只有當上鍵是新按下的(之前沒有按下)並且角色在地面上時才跳躍
  if (upKeyPressed && !m_UpKeyWasPressed && m_IsOnGround) {
    m_IsJumping = true;
    m_IsOnGround = false;
    m_JumpHeight = 0;
  }

  // 更新上一次按鍵狀態
  m_UpKeyWasPressed = upKeyPressed;

  UpdateAnimation();
}

// void Character::UpdateJump(const GridSystem &grid) {
//   if (m_IsJumping) {
//     glm::vec2 pos = GetPosition();
//     float fallSpeed = 5.0f; // 下落速度
//     float jumpSpeed = 7.0f; // 跳躍速度

//     // 上升階段
//     if (m_JumpHeight < m_JumpMaxHeight) {
//       glm::vec2 nextPos = pos;
//       nextPos.y += jumpSpeed; // 嘗試向上跳

//       // 檢查跳躍後頭部會到達的格子
//       glm::ivec2 gridPosTop =
//           grid.GameToCellPosition(glm::vec2(pos.x, pos.y + (m_Size.y
//           + 13.5f)));
//       CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

//       // 如果即將碰到任何非空的地形，則停止上升
//       if (aboveCell != CellType::EMPTY) {
//         m_JumpHeight += isMoving ? (jumpSpeed / 1.5f) : m_JumpMaxHeight;
//       } else {
//         pos = nextPos;
//         m_JumpHeight += jumpSpeed;
//       }
//     }
//     // 下降階段
//     else {
//       glm::vec2 nextPos = pos;
//       nextPos.y -= fallSpeed; // 嘗試下降

//       glm::ivec2 gridPosBelow = grid.GameToCellPosition(nextPos);
//       CellType belowCell = grid.GetCell(gridPosBelow.x, gridPosBelow.y);

//       // 如果腳底碰到非空的地形，則停止下降
//       if (belowCell != CellType::EMPTY) {
//         m_IsJumping = false;
//         m_IsOnGround = true;
//         m_JumpHeight = 0;

//         // 修正 Y 軸位置，讓角色貼合地板
//         float cellBottomY =
//             grid.CellToGamePosition(gridPosBelow.x, gridPosBelow.y).y;
//         pos.y = cellBottomY + (grid.GetCellSize() / 2.0f - 13.5f);
//       } else {
//         pos = nextPos;
//       }
//     }

//     SetPosition(pos);
//   }
// }

void Character::UpdateJump(const GridSystem &grid) {
  if (m_IsJumping) {
    glm::vec2 pos = GetPosition();
    float fallSpeed = 5.0f; // 下落速度
    float jumpSpeed = 7.0f; // 跳躍速度

    glm::vec2 nextPos = pos;

    // 上升階段
    if (m_JumpHeight < m_JumpMaxHeight) {
      nextPos.y += jumpSpeed; // 嘗試向上跳

      // 定義角色框框的四個角落點（上升後的位置）
      float left = nextPos.x - (m_Size.x / 2);
      float right = nextPos.x + (m_Size.x / 2);
      // float bottom = nextPos.y + 13.5f;
      float top = nextPos.y + m_Size.y;

      // 檢查整個頂部邊緣的碰撞
      bool topCollision = false;

      // 將頂部邊緣分成多個檢查點
      int checkPoints = 10; // 檢查點數量
      for (int i = 0; i <= checkPoints; i++) {
        float checkX =
            left + (right - left) * (static_cast<float>(i) / checkPoints);
        glm::ivec2 gridPosTop = grid.GameToCellPosition(glm::vec2(checkX, top));
        CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

        if (aboveCell != CellType::EMPTY) {
          std::cout << "topCollision" << std::endl;
          topCollision = true;
          break;
        }
      }

      if (topCollision) {
        m_JumpHeight += isMoving ? (jumpSpeed / 1.5f) : m_JumpMaxHeight;
        return;
      }

      pos = nextPos;
      m_JumpHeight += jumpSpeed;
    }
    // 下降階段
    else {
      nextPos.y -= fallSpeed; // 嘗試下降

      // 定義角色框框的四個角落點（下降後的位置）
      float left = nextPos.x - (m_Size.x / 2);
      float right = nextPos.x + (m_Size.x / 2);
      float bottom = nextPos.y + 13.5f;
      // float top = nextPos.y + m_Size.y + 13.5f;

      // 檢查整個底部邊緣的碰撞
      bool bottomCollision = false;

      // 將底部邊緣分成多個檢查點
      int checkPoints = 15; // 檢查點數量
      for (int i = 0; i <= checkPoints; i++) {
        float checkX =
            left + (right - left) * (static_cast<float>(i) / checkPoints);
        glm::ivec2 gridPosBottom =
            grid.GameToCellPosition(glm::vec2(checkX, bottom));
        CellType belowCell = grid.GetCell(gridPosBottom.x, gridPosBottom.y);

        if (belowCell != CellType::EMPTY) {
          bottomCollision = true;

          // 修正 Y 軸位置，讓角色貼合地板
          float cellBottomY =
              grid.CellToGamePosition(gridPosBottom.x, gridPosBottom.y).y;
          pos.y = cellBottomY + (grid.GetCellSize() / 2.0f - 13.5f);
          m_IsJumping = false;
          m_IsOnGround = true;
          m_JumpHeight = 0;
          SetPosition(pos);
          return;
        }
      }

      if (!bottomCollision) {
        pos = nextPos;
      }
    }

    SetPosition(pos);
  }
}

void Character::ApplyGravity(const GridSystem &grid) {
  if (!m_IsJumping) {
    glm::vec2 pos = GetPosition();
    glm::vec2 nextPos = pos;
    float fallSpeed = 5.0f;
    nextPos.y -= fallSpeed;

    glm::ivec2 gridPos = grid.GameToCellPosition(nextPos);
    CellType belowCell = grid.GetCell(gridPos.x, gridPos.y);

    if (grid.CanStandOn(belowCell, this->IsFireboy())) {
      m_IsOnGround = true;
      float cellBottomY = grid.CellToGamePosition(gridPos.x, gridPos.y).y;
      pos.y = cellBottomY + (grid.GetCellSize() / 2.0f - 12.0f);
    } else {
      m_IsOnGround = false;
      pos = nextPos;
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
  // TODO：加入動畫、暫停輸入等效果
}

bool Character::IsDead() const { return m_IsDead; }

void Character::Respawn() {
  m_IsDead = false;
  SetPosition(m_SpawnPoint);
}

void Character::SetSpawnPoint(const glm::vec2 &spawn) { m_SpawnPoint = spawn; }