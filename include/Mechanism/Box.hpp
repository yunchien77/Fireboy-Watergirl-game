#ifndef BOX_HPP
#define BOX_HPP

#include "Interface/ICollidable.hpp"
#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

class Box : public Util::GameObject, public ICollidable {
public:
  Box();

  // Position and state methods
  void SetPosition(const glm::vec2 &position);
  glm::vec2 GetPosition() const;
  glm::vec2 GetSize() const { return m_BoxSize; }
  void SetInitialPosition(const glm::vec2 &pos);
  void Respawn();

  // Physics methods
  void Update();
  void ApplyGravity();
  bool IsGrounded();

  // Character interaction methods
  void OnCollisionWithCharacter(std::shared_ptr<Character> character);
  bool IsCharacterOn(Character *character) const;
  bool CheckCharacterCollision(std::shared_ptr<Character> character);

  // Terrain collision methods
  bool CheckCollisionWithTerrain(const glm::vec2 &position);

  // ICollidable implementation
  const SDL_Rect &getRect() const override;

  // Environment setup
  void SetGridSystem(GridSystem *grid) { m_GridSystem = grid; }

private:
  // Physics properties
  float m_VelocityY;
  float m_Gravity;
  float m_MoveSpeed;
  bool m_Grounded;

  // Size and position
  glm::vec2 m_BoxSize;
  glm::vec2 m_InitialPosition;

  // Collision detection
  mutable SDL_Rect m_Rect;

  // Environment reference
  GridSystem *m_GridSystem = nullptr;
};

#endif // BOX_HPP