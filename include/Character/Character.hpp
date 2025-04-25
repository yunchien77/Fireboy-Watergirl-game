#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Mechanism/Platform.hpp"

class Character : public Util::GameObject {
public:
  explicit Character(const std::string &imagePath, const float zindex);

  Character(const Character &) = delete;
  Character(Character &&) = delete;
  Character &operator=(const Character &) = delete;
  Character &operator=(Character &&) = delete;

  [[nodiscard]] const std::string &GetImagePath() const;
  [[nodiscard]] const glm::vec2 &GetPosition() const;
  [[nodiscard]] bool GetVisibility() const;
  [[nodiscard]] bool IsJumping() const;
  [[nodiscard]] bool IsOnGround() const;

  // 更改角色的圖像
  void SetImage(const std::string &imagePath);

  // 更改角色的位置
  void SetPosition(const glm::vec2 &position);

  // 取得角色的尺寸
  glm::vec2 GetSize() const;

  // 移動角色
  void Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
            bool isFireboy);

  // 更新跳躍
  void UpdateJump(const GridSystem &grid);

  // 應用重力
  void ApplyGravity(const GridSystem &grid);

  // 更新角色的動畫(純虛擬函數 -> Fireboy 和 Watergirl 會實現)
  virtual void UpdateAnimation() = 0;

  virtual void Die();  // 設定角色死亡
  bool IsDead() const; // 查詢死亡狀態
  void Respawn();      // 重生
  void SetSpawnPoint(const glm::vec2 &spawn);

  virtual const SDL_Rect &getRect() const = 0;

  virtual bool IsFireboy() const = 0;

  virtual float getX() const = 0;
  virtual float getY() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  void SetPreviousPosition();
  void UndoMovement();

  void Translate(const glm::vec2 &offset);
  void Update();
  void SetPlatforms(const std::vector<std::shared_ptr<Platform>> &platforms);
  void SetStandingOnPlatform(bool value);
  static constexpr float GRAVITY_SPEED = 10.0f;

protected:
  // 應用水平翻轉
  void ApplyFlip();

  std::string m_ImagePath;
  bool isMoving;      // 是否正在移動
  bool currentSprite; // 切換動畫幀
  bool m_IsJumping;
  int m_JumpHeight;    // 當前跳躍高度
  int m_JumpMaxHeight; // 最大跳躍高度
  bool m_IsOnGround;   // 角色是否在地面上
  bool m_UpKeyWasPressed; // 上鍵是否已被按下（用於防止持續按住時重複跳躍）
  bool m_FacingRight; // 角色面向方向：true為右，false為左
  glm::vec2 m_Size;   // 角色的尺寸
  bool m_IsDead = false;
  glm::vec2 m_SpawnPoint;
  glm::vec2 m_LastPosition;

  std::vector<std::shared_ptr<Platform>> m_Platforms;
  std::shared_ptr<Platform> m_CurrentPlatform;

  bool m_IsStandingOnPlatform = false;
  glm::vec2 m_Velocity = glm::vec2(0.0f);
  std::shared_ptr<Platform> m_PreviousPlatform;
};

#endif // CHARACTER_HPP
