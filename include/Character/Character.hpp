#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <GridSystem.hpp>

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>

class Character : public Util::GameObject {
public:
  explicit Character(const std::string &imagePath)
      : GameObject(std::make_shared<Util::Image>(imagePath), 10),
        m_ImagePath(imagePath), isMoving(false), currentSprite(false),
        m_IsJumping(false), m_JumpHeight(0), m_JumpMaxHeight(90),
        m_IsOnGround(true), m_UpKeyWasPressed(false), m_FacingRight(true) {

    glm::vec2 size = GetScaledSize();
    SetPivot(glm::vec2(0.0f, -size.y / 2 - 13.5));
  }

  Character(const Character &) = delete;
  Character(Character &&) = delete;
  Character &operator=(const Character &) = delete;
  Character &operator=(Character &&) = delete;

  [[nodiscard]] const std::string &GetImagePath() const { return m_ImagePath; }
  [[nodiscard]] const glm::vec2 &GetPosition() const {
    return m_Transform.translation;
  }
  [[nodiscard]] bool GetVisibility() const { return m_Visible; }
  [[nodiscard]] bool IsJumping() const { return m_IsJumping; }
  [[nodiscard]] bool IsOnGround() const { return m_IsOnGround; }

  // 更改角色的圖像
  void SetImage(const std::string &imagePath) {
    m_ImagePath = imagePath;
    SetDrawable(std::make_shared<Util::Image>(imagePath));

    // 應用水平翻轉
    ApplyFlip();
  }

  // 更改角色的位置
  void SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
  }

  void Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
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
    if (!grid.CheckCollision(newPos, isFireboy)) {
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

  void UpdateJump(const GridSystem &grid) {
    if (m_IsJumping) {
      glm::vec2 pos = GetPosition();
      float fallSpeed = 5.0f; // 下落速度
      float jumpSpeed = 7.0f; // 跳躍速度

      // **🔼 上升階段**
      if (m_JumpHeight < m_JumpMaxHeight) {
        glm::vec2 nextPos = pos;
        nextPos.y += jumpSpeed; // 嘗試向上跳

        // **檢查跳躍後頭部會到達的格子**
        glm::ivec2 gridPosTop =
            grid.GameToCellPosition(glm::vec2(pos.x, pos.y + m_JumpMaxHeight));
        CellType aboveCell = grid.GetCell(gridPosTop.x, gridPosTop.y);

        // **🛑 如果即將撞到天花板，則停止上升**
        if (aboveCell == CellType::FLOOR) {
          m_JumpHeight = m_JumpMaxHeight; // 強制結束跳躍
        } else {
          pos = nextPos;
          m_JumpHeight += jumpSpeed;
        }
      }
      // **🔽 下降階段**
      else {
        glm::vec2 nextPos = pos;
        nextPos.y -= fallSpeed; // 嘗試下降

        glm::ivec2 gridPosBelow = grid.GameToCellPosition(nextPos);
        CellType belowCell = grid.GetCell(gridPosBelow.x, gridPosBelow.y);

        // **✅ 如果腳底碰到地板，則停止下降**
        if (belowCell == CellType::FLOOR) {
          m_IsJumping = false;
          m_IsOnGround = true;
          m_JumpHeight = 0;

          // **🔧 修正 Y 軸位置，讓角色貼合地板**
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

  void ApplyGravity(const GridSystem &grid) {
    if (!m_IsJumping) { // 只有在未跳躍時才應用重力
      glm::vec2 pos = GetPosition();
      glm::vec2 nextPos = pos;
      float fallSpeed = 5.0f; // 下落速度

      nextPos.y -= fallSpeed; // 嘗試往下移動
      glm::ivec2 gridPos = grid.GameToCellPosition(nextPos);
      CellType belowCell = grid.GetCell(gridPos.x, gridPos.y);

      if (belowCell == CellType::FLOOR) {
        // ✅ 落地時，修正 Y 軸位置，避免浮空
        m_IsOnGround = true;
        float cellBottomY = grid.CellToGamePosition(gridPos.x, gridPos.y).y;
        pos.y = cellBottomY + (grid.GetCellSize() / 2.0f - 12.0f);
      } else {
        // 🚀 沒有地板，繼續掉落
        m_IsOnGround = false;
        pos = nextPos;
      }

      SetPosition(pos);
    }
  }

  virtual void UpdateAnimation() = 0; // 更新角色的動畫(純虛擬函數 -> Fireboy 和
                                      // Watergirl 會實現)

protected:
  // 應用水平翻轉
  void ApplyFlip() {
    if (m_FacingRight) {
      m_Transform.scale.x = std::abs(m_Transform.scale.x); // 確保正向
    } else {
      m_Transform.scale.x = -std::abs(m_Transform.scale.x); // 確保負向
    }
  }

  std::string m_ImagePath;
  bool isMoving;      // 是否正在移動
  bool currentSprite; // 切換動畫幀
  bool m_IsJumping;
  int m_JumpHeight;    // 當前跳躍高度
  int m_JumpMaxHeight; // 最大跳躍高度
  bool m_IsOnGround;   // 角色是否在地面上
  bool m_UpKeyWasPressed; // 上鍵是否已被按下（用於防止持續按住時重複跳躍）
  bool m_FacingRight; // 角色面向方向：true為右，false為左
};

#endif // CHARACTER_HPP