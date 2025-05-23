#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Character/CharacterComponent.hpp"
#include "Character/CharacterState.hpp"
#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Mechanism/Box.hpp"
#include "Mechanism/Platform.hpp"

// Forward declarations
class GridSystem;
class Platform;
class Box;

// Base Character class
class Character : public Util::GameObject {
  friend class CharacterCollisionComponent;
  friend class CharacterMovementComponent;
  friend class CharacterPhysicsComponent;

public:
  explicit Character(const std::string &imagePath, const float zindex);

  Character(const Character &) = delete;
  Character(Character &&) = delete;
  Character &operator=(const Character &) = delete;
  Character &operator=(Character &&) = delete;

  //==================================
  // Getters
  //==================================
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
  [[nodiscard]] CharacterState GetState() const;

  //==================================
  // Setters
  //==================================
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
  void SetState(CharacterState state);

  //==================================
  // Movement methods
  //==================================
  void Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
            bool isFireboy);
  void MoveWithCollision(const glm::vec2 &offset, const GridSystem &grid);
  void Translate(const glm::vec2 &offset);

  //==================================
  // Physics methods
  //==================================
  void UpdateJump(const GridSystem &grid);
  void ApplyGravity(const GridSystem &grid);
  void ApplyExternalForce(float y);
  void ResetExternalForce();

  //==================================
  // Collision methods
  //==================================
  bool CheckStandingOnPlatform(const glm::vec2 &position,
                               std::shared_ptr<Platform> &outPlatform);
  bool IsCollidingWithPlatformBottom(Platform *platform,
                                     const glm::vec2 &charPos) const;
  glm::vec2 AdjustPositionForPlatform(const glm::vec2 &position,
                                      int moveDirection);

  //==================================
  // Lifecycle methods
  //==================================
  virtual void Die();
  void Respawn();
  void Update();

  //==================================
  // Animation
  //==================================
  virtual void UpdateAnimation() = 0;

  //==================================
  // Pure virtual methods for derived classes
  //==================================
  virtual const SDL_Rect &getRect() const = 0;
  virtual bool IsFireboy() const = 0;
  virtual float getX() const = 0;
  virtual float getY() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  void SetInvincible(bool enable);
  bool IsInvincible() const;

protected:
  // Apply horizontal flip based on facing direction
  void ApplyFlip();

  // Character state
  CharacterState m_State = CharacterState::STANDING;

  // Basic attributes
  std::string m_ImagePath;
  bool m_FacingRight; // true = right, false = left
  glm::vec2 m_Size;   // Character dimensions
  bool isMoving;      // Is the character moving
  bool currentSprite; // Animation frame toggle

  // Jump-related attributes
  bool m_IsJumping;
  int m_JumpHeight;       // Current jump height
  int m_JumpMaxHeight;    // Maximum jump height
  bool m_IsOnGround;      // Is character on ground
  bool m_UpKeyWasPressed; // Was up key already pressed (prevents continuous
                          // jumps)
  bool m_HitCeiling = false;

  // Lifecycle and position tracking
  bool m_IsDead = false;
  glm::vec2 m_SpawnPoint;
  glm::vec2 m_LastPosition;

  // Platform and box interaction
  std::vector<std::shared_ptr<Platform>> m_Platforms;
  std::shared_ptr<Platform> m_CurrentPlatform;
  std::shared_ptr<Platform> m_PreviousPlatform;
  std::vector<std::shared_ptr<Box>> m_Boxes;
  bool m_IsStandingOnPlatform = false;

  // Physics
  glm::vec2 m_Velocity = glm::vec2(0.0f);
  GridSystem *m_GridRef = nullptr;

  // Component references
  std::unique_ptr<CharacterCollisionComponent> m_CollisionComponent;
  std::unique_ptr<CharacterMovementComponent> m_MovementComponent;
  std::unique_ptr<CharacterPhysicsComponent> m_PhysicsComponent;

  bool isInvincible = false;

private:
  // Component initialization
  void InitializeComponents();
};

#endif // CHARACTER_HPP