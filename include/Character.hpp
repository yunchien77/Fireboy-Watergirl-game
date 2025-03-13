#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>

class Character : public Util::GameObject {
public:
  explicit Character(const std::string &imagePath)
      : GameObject(std::make_shared<Util::Image>(imagePath), 10),
        m_ImagePath(imagePath), isMoving(false), currentSprite(false),
        m_IsJumping(false), m_JumpHeight(0), m_JumpMaxHeight(50),
        m_IsOnGround(true), m_UpKeyWasPressed(false), m_FacingRight(true) {}

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

  // 處理角色移動，接收 X 和 Y 方向的變化值
  void Move(int deltaX, bool upKeyPressed) {
    isMoving = (deltaX != 0);

    // 更新面向方向
    if (deltaX < 0) {
      m_FacingRight = false;
    } else if (deltaX > 0) {
      m_FacingRight = true;
    }

    // 應用水平翻轉
    ApplyFlip();

    // 處理水平移動
    glm::vec2 newPos = GetPosition();
    newPos.x += deltaX;
    SetPosition(newPos);

    // 處理跳躍請求
    if (upKeyPressed) {
      // 如果在地面上且之前上鍵沒有被按下（或已釋放）
      if (m_IsOnGround && !m_UpKeyWasPressed) {
        m_IsJumping = true;
        m_IsOnGround = false;
        m_JumpHeight = 0;
      }
      // 記錄上鍵已被按下
      m_UpKeyWasPressed = true;
    } else {
      // 上鍵被釋放
      m_UpKeyWasPressed = false;
    }

    UpdateAnimation();
  }

  void UpdateJump() {
    if (m_IsJumping) {
      glm::vec2 pos = GetPosition();

      // 上升階段
      if (m_JumpHeight < m_JumpMaxHeight) {
        pos.y += 5; // 向上移動的速度
        m_JumpHeight += 5;
      }
      // 下降階段
      else {
        pos.y -= 5; // 向下移動的速度

        // 檢查是否到達地面
        if (pos.y <= -288) { // LEVEL_MIN_Y 值
          pos.y = -288;
          m_IsJumping = false;
          m_JumpHeight = 0;
          m_IsOnGround = true;
        }
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