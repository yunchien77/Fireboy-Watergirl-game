#include "Character/Character.hpp"
#include "Character/CharacterComponent.hpp"
#include "Character/CharacterState.hpp"
#include "Mechanism/Fan.hpp"
#include <cmath>
#include <iostream>
#include <map>


// Character constructor and basic initialization
Character::Character(const std::string &imagePath, const float zindex)
    : GameObject(std::make_shared<Util::Image>(imagePath), zindex),
      m_ImagePath(imagePath), m_FacingRight(true), isMoving(false),
      currentSprite(false), m_IsJumping(false), m_JumpHeight(0),
      m_JumpMaxHeight(90), m_IsOnGround(true), m_UpKeyWasPressed(false) {

  m_Size = GetScaledSize();
  SetPivot(glm::vec2(0.0f, -m_Size.y / 2 - CharacterPhysics::CHARACTER_OFFSET));

  // Initialize components
  InitializeComponents();
}

//==================================
// Component initialization
//==================================
void Character::InitializeComponents() {
  m_CollisionComponent = std::make_unique<CharacterCollisionComponent>(this);
  m_MovementComponent = std::make_unique<CharacterMovementComponent>(this);
  m_PhysicsComponent = std::make_unique<CharacterPhysicsComponent>(this);
}

//==================================
// Basic getter methods implementation
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
const glm::vec2 &Character::GetExternalForce() const {
  return m_PhysicsComponent->GetExternalForce();
}
bool Character::IsAffectedByWind() const {
  return m_PhysicsComponent->IsAffectedByWind();
}
CharacterState Character::GetState() const { return m_State; }

//==================================
// Character state setting methods implementation
//==================================
void Character::SetImage(const std::string &imagePath) {
  m_ImagePath = imagePath;
  SetDrawable(std::make_shared<Util::Image>(imagePath));

  // Apply horizontal flip
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
  m_PhysicsComponent->SetAffectedByWind(affected);
}

void Character::SetState(CharacterState state) { m_State = state; }

// ============================================================
// Character movement related helper functions
// ============================================================

void Character::ApplyFlip() {
  if (m_FacingRight) {
    m_Transform.scale.x = std::abs(m_Transform.scale.x); // Ensure positive
  } else {
    m_Transform.scale.x = -std::abs(m_Transform.scale.x); // Ensure negative
  }
}

void Character::Translate(const glm::vec2 &offset) {
  m_Transform.translation += offset;
}

// ============================================================
// Collision detection related functions
// ============================================================

// Adjust position based on platform collision
glm::vec2 Character::AdjustPositionForPlatform(const glm::vec2 &position,
                                               int moveDirection) {
  return m_CollisionComponent->AdjustPositionForPlatform(position,
                                                         moveDirection);
}

// Check if character is standing on a platform
bool Character::CheckStandingOnPlatform(
    const glm::vec2 &position, std::shared_ptr<Platform> &outPlatform) {
  return m_CollisionComponent->CheckStandingOnPlatform(position, outPlatform);
}

bool Character::IsCollidingWithPlatformBottom(Platform *platform,
                                              const glm::vec2 &charPos) const {
  return m_CollisionComponent->IsCollidingWithPlatformBottom(platform, charPos);
}

// ============================================================
// Character movement related methods
// ============================================================

void Character::Move(int deltaX, bool upKeyPressed, const GridSystem &grid,
                     bool isFireboy) {
  m_MovementComponent->Move(deltaX, upKeyPressed, grid, isFireboy);
}

void Character::MoveWithCollision(const glm::vec2 &offset,
                                  const GridSystem &grid) {
  m_MovementComponent->MoveWithCollision(offset, grid);
}

// ============================================================
// Character jump related methods
// ============================================================

void Character::UpdateJump(const GridSystem &grid) {
  m_MovementComponent->UpdateJump(grid);
}

// ============================================================
// Gravity system related methods
// ============================================================

void Character::ApplyGravity(const GridSystem &grid) {
  m_MovementComponent->ApplyGravity(grid);
}

// ============================================================
// State related methods
// ============================================================

void Character::Die() {
  m_IsDead = true;
  SetPosition(m_SpawnPoint);
}

void Character::Respawn() {
  m_IsDead = false;
  SetPosition(m_SpawnPoint);
}

// ============================================================
// External force related methods
// ============================================================

void Character::ApplyExternalForce(float y) {
  m_PhysicsComponent->ApplyExternalForce(y);
}

void Character::ResetExternalForce() {
  m_PhysicsComponent->ResetExternalForce();
}

void Character::Update() {
  // Wind effect handling
  if (m_PhysicsComponent->IsAffectedByWind()) {
    const glm::vec2 &externalForce = m_PhysicsComponent->GetExternalForce();
    if (std::abs(externalForce.y) > 0.001f && m_GridRef) {
      float floatEffect = Fan::GetWindFloatEffect();
      float windAmount = externalForce.y * floatEffect * 0.2f;

      // Avoid low altitude hard ascent (give some initial boost)
      if (GetPosition().y < 200.0f) {
        windAmount += 2.0f;
      }

      glm::vec2 offset(0.0f, windAmount);
      MoveWithCollision(offset, *m_GridRef);
    }
  }

  // Record previous platform state
  bool wasOnPlatform = m_IsStandingOnPlatform;
  std::shared_ptr<Platform> previousPlatform = m_CurrentPlatform;

  m_IsStandingOnPlatform = false;
  m_CurrentPlatform = nullptr;

  // First check if character is standing on a box
  bool standingOnBox = false;
  for (const auto &box : m_Boxes) {
    if (box->IsCharacterOn(this)) {
      standingOnBox = true;
      m_IsStandingOnPlatform = true;
      m_IsOnGround = true;

      // Adjust character position to be completely on the box
      glm::vec2 pos = GetPosition();
      glm::vec2 boxPos = box->GetPosition();
      glm::vec2 boxSize = box->GetSize();
      pos.y = boxPos.y + boxSize.y - CharacterPhysics::CHARACTER_OFFSET;
      SetPosition(pos);
      break;
    }
  }

  // If not standing on a box, check if standing on a platform
  if (!standingOnBox) {
    // Check if character is standing on any platform
    for (const auto &platform : m_Platforms) {
      if (platform->IsCharacterOn(this)) {
        // Found the platform the character is standing on
        m_IsStandingOnPlatform = true;
        m_IsOnGround = true;
        m_CurrentPlatform = platform;

        // Only adjust position on first contact with platform
        if (!wasOnPlatform || previousPlatform != platform) {
          glm::vec2 pos = GetPosition();
          glm::vec2 platPos = platform->GetPosition();
          // Use accurate offset to align character bottom with platform top
          pos.y =
              platPos.y + 11.5f -
              CharacterPhysics::CHARACTER_OFFSET; // platTop - charBottomOffset
          SetPosition(pos);
        }

        break;
      }
    }
  }

  // If the character is standing on a platform, apply the platform's movement
  if (m_IsStandingOnPlatform && m_CurrentPlatform) {
    // Directly use the movement amount provided by the platform
    glm::vec2 platMove = m_CurrentPlatform->GetDeltaMovement();
    if (glm::length(platMove) >
        0.01f) { // Only apply if there is actual movement
      // Translate(platMove);
    }
  }
}