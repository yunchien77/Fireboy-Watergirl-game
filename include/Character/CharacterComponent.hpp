#ifndef CHARACTER_COMPONENTS_HPP
#define CHARACTER_COMPONENTS_HPP

#include "Character/CharacterState.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Forward declarations
class Character;
class GridSystem;
class Platform;
class Box;

// Character collision component
class CharacterCollisionComponent {
public:
  explicit CharacterCollisionComponent(class Character *owner);

  // Collision detection methods
  bool CheckTopCollision(const glm::vec2 &position, const GridSystem &grid);
  bool CheckBottomCollision(const glm::vec2 &position, const GridSystem &grid);
  glm::vec2 CheckAndAdjustSideCollisions(const glm::vec2 &position,
                                         const GridSystem &grid);
  void AdjustPositionToAvoidWalls(glm::vec2 &position, const GridSystem &grid);
  bool CheckStandingOnPlatform(const glm::vec2 &position,
                               std::shared_ptr<Platform> &outPlatform);
  glm::vec2 AdjustPositionForPlatform(const glm::vec2 &position,
                                      int moveDirection);
  bool IsCollidingWithPlatformBottom(Platform *platform,
                                     const glm::vec2 &charPos) const;

private:
  Character *m_Owner;
};

// Movement component
class CharacterMovementComponent {
public:
  explicit CharacterMovementComponent(class Character *owner);

  // Movement methods
  void Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
            bool isFireboy);
  void MoveWithCollision(const glm::vec2 &offset, const GridSystem &grid);
  void ApplyGravity(const GridSystem &grid);
  void UpdateJump(const GridSystem &grid);

  // Jump-related methods
  void HandleJumpAscending(const GridSystem &grid);
  void HandleJumpDescending(const GridSystem &grid);
  void HandleLandingOnPlatform(std::shared_ptr<Platform> platform);

private:
  Character *m_Owner;
};

// Physics component
class CharacterPhysicsComponent {
public:
  explicit CharacterPhysicsComponent(class Character *owner);

  // External forces methods
  void ApplyExternalForce(float y);
  void ResetExternalForce();

  [[nodiscard]] const glm::vec2 &GetExternalForce() const;
  [[nodiscard]] bool IsAffectedByWind() const;
  void SetAffectedByWind(bool affected);

private:
  Character *m_Owner;
  glm::vec2 m_ExternalForce = {0.0f, 0.0f};
  bool m_AffectedByWind = false;
};

#endif // CHARACTER_COMPONENTS_HPP