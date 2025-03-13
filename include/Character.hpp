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
        m_IsJumping(false), m_JumpHeight(0), m_JumpMaxHeight(50) {}

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

  // 更改角色的圖像
  void SetImage(const std::string &imagePath) {
    m_ImagePath = imagePath;
    SetDrawable(std::make_shared<Util::Image>(imagePath));
  }

  // 更改角色的位置
  void SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
  }

  // 處理角色移動，接收 X 和 Y 方向的變化值
  void Move(int deltaX, int deltaY) {
    isMoving = (deltaX != 0 || deltaY != 0);

    // 處理水平移動
    glm::vec2 newPos = GetPosition();
    newPos.x += deltaX;

    // 處理跳躍
    if (deltaY > 0 && !m_IsJumping) {
      m_IsJumping = true;
      m_JumpHeight = 0;
    }

    // 更新角色位置
    SetPosition(newPos);
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

        // 檢查是否回到起跳點或地面
        if (pos.y <= -288) {   // LEVEL_MIN_Y 值，表示地面
          pos.y = -288;        // 確保不會低於地面
          m_IsJumping = false; // 重置跳躍狀態
          m_JumpHeight = 0;
        }
      }

      SetPosition(pos);
    }
  }

  virtual void UpdateAnimation() = 0; // 更新角色的動畫(純虛擬函數 -> Fireboy 和
                                      // Watergirl 會實現)

protected:
  std::string m_ImagePath;
  bool isMoving;      // 是否正在移動
  bool currentSprite; // 切換動畫幀
  bool m_IsJumping;
  int m_JumpHeight;    // 當前跳躍高度
  int m_JumpMaxHeight; // 最大跳躍高度
};

#endif // CHARACTER_HPP
