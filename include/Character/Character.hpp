#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Mechanism/Box.hpp"
#include "Mechanism/Platform.hpp"

class Character : public Util::GameObject {
public:
  explicit Character(const std::string &imagePath, const float zindex);

  Character(const Character &) = delete;
  Character(Character &&) = delete;
  Character &operator=(const Character &) = delete;
  Character &operator=(Character &&) = delete;

  // 基本getter方法
  [[nodiscard]] const std::string &GetImagePath() const;
  [[nodiscard]] const glm::vec2 &GetPosition() const;
  [[nodiscard]] bool GetVisibility() const;
  [[nodiscard]] bool IsJumping() const;
  [[nodiscard]] bool IsOnGround() const;
  [[nodiscard]] bool IsMoving() const;
  [[nodiscard]] bool IsFacingRight() const;
  [[nodiscard]] bool IsDead() const;
  [[nodiscard]] glm::vec2 GetSize() const;
  [[nodiscard]] bool IsStandingOnPlatform() const;
  [[nodiscard]] std::shared_ptr<Platform> GetCurrentPlatform() const;
  [[nodiscard]] const glm::vec2 &GetExternalForce() const;
  [[nodiscard]] bool IsAffectedByWind() const;

  // 角色狀態設定
  void SetImage(const std::string &imagePath);
  void SetPosition(const glm::vec2 &position);
  void SetSpawnPoint(const glm::vec2 &spawn);
  void SetPreviousPosition();
  void UndoMovement();
  void SetStandingOnPlatform(bool value);
  void SetPlatforms(const std::vector<std::shared_ptr<Platform>> &platforms);
  void SetBoxes(const std::vector<std::shared_ptr<Box>> &boxes);
  void SetGridSystem(GridSystem *grid);
  void SetAffectedByWind(bool affected);

  // 角色移動和物理相關
  void Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
            bool isFireboy);
  void MoveWithCollision(const glm::vec2 &offset, const GridSystem &grid);
  void UpdateJump(const GridSystem &grid);
  void ApplyGravity(const GridSystem &grid);
  void ApplyExternalForce(float y);
  void ResetExternalForce();
  void Translate(const glm::vec2 &offset);

  // 角色生命週期
  virtual void Die();
  void Respawn();
  void Update();

  // 動畫相關
  virtual void UpdateAnimation() = 0;

  // 碰撞相關
  bool IsCollidingWithPlatformBottom(Platform *platform,
                                     const glm::vec2 &charPos) const;

  virtual const SDL_Rect &getRect() const = 0;
  virtual bool IsFireboy() const = 0;
  virtual float getX() const = 0;
  virtual float getY() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  static constexpr float GRAVITY_SPEED = 10.0f;
  static constexpr float MAX_EXTERNAL_FORCE = 25.0f;
  static constexpr float MIN_EXTERNAL_FORCE = 0.0f;

protected:
  // 角色移動輔助方法
  void ApplyFlip();

  // 角色基本屬性
  std::string m_ImagePath;
  bool m_FacingRight; // 角色面向方向：true為右，false為左
  glm::vec2 m_Size;   // 角色的尺寸
  bool isMoving;      // 是否正在移動
  bool currentSprite; // 切換動畫幀

  // 跳躍相關屬性
  bool m_IsJumping;
  int m_JumpHeight;    // 當前跳躍高度
  int m_JumpMaxHeight; // 最大跳躍高度
  bool m_IsOnGround;   // 角色是否在地面上
  bool m_UpKeyWasPressed; // 上鍵是否已被按下（用於防止持續按住時重複跳躍）
  bool m_HitCeiling = false;

  // 生命狀態和位置紀錄
  bool m_IsDead = false;
  glm::vec2 m_SpawnPoint;
  glm::vec2 m_LastPosition;

  // 平台和箱子相關
  std::vector<std::shared_ptr<Platform>> m_Platforms;
  std::shared_ptr<Platform> m_CurrentPlatform;
  std::shared_ptr<Platform> m_PreviousPlatform;
  std::vector<std::shared_ptr<Box>> m_Boxes;
  bool m_IsStandingOnPlatform = false;

  // 物理相關
  glm::vec2 m_Velocity = glm::vec2(0.0f);

private:
  // 外部力量(風等)相關
  glm::vec2 m_ExternalForce = {0.0f, 0.0f};
  bool m_AffectedByWind = false;
  GridSystem *m_GridRef = nullptr;
};

#endif // CHARACTER_HPP