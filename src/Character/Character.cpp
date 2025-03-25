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

glm::vec2 Character::GetSize() { return m_Size; }

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

void Character::UpdateJump(const GridSystem &grid) {
  if (m_IsJumping) {
    glm::vec2 pos = GetPosition();
    float fallSpeed = 5.0f; // 下落速度
    float jumpSpeed = 7.0f; // 跳躍速度

    // 上升階段
    if (m_JumpHeight < m_JumpMaxHeight) {
      glm::vec2 nextPos = pos;
      nextPos.y += jumpSpeed; // 嘗試向上跳

      // 檢查跳躍後頭部會到達的格子
      glm::ivec2 gridPosTop =
          grid.GameToCellPosition(glm::vec2(pos.x, pos.y + m_JumpMaxHeight));
      CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

      // 如果即將撞到天花板，則停止上升
      if (aboveCell == CellType::FLOOR || aboveCell == CellType::WATER) {
        m_JumpHeight = m_JumpMaxHeight; // 強制結束跳躍
      } else {
        pos = nextPos;
        m_JumpHeight += jumpSpeed;
      }
    }
    // 下降階段
    else {
      glm::vec2 nextPos = pos;
      nextPos.y -= fallSpeed; // 嘗試下降

      glm::ivec2 gridPosBelow = grid.GameToCellPosition(nextPos);
      CellType belowCell = grid.GetCell(gridPosBelow.x, gridPosBelow.y);

      // 如果腳底碰到地板，則停止下降
      if (grid.CanStandOn(belowCell, this->IsFireboy())) {
        m_IsJumping = false;
        m_IsOnGround = true;
        m_JumpHeight = 0;

        // 修正 Y 軸位置，讓角色貼合地板
        float cellBottomY =
            grid.CellToGamePosition(gridPosBelow.x, gridPosBelow.y).y;
        pos.y = cellBottomY + (grid.GetCellSize() / 2.0f - 12.0f);
      } else {
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
  std::cout << "角色死亡\n";
  SetPosition(m_SpawnPoint);
  // TODO：加入動畫、暫停輸入等效果
}

bool Character::IsDead() const {
  return m_IsDead;
}

void Character::Respawn() {
  m_IsDead = false;
  SetPosition(m_SpawnPoint);
  std::cout << "角色重生，回到出生點\n";
}

void Character::SetSpawnPoint(const glm::vec2 &spawn) {
  m_SpawnPoint = spawn;
}