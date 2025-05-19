#ifndef BOX_HPP
#define BOX_HPP

#include "Mechanism/MechanismBase.hpp"
#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

class Box : public MechanismBase {
public:
  Box();

  // Position and state methods - override from MechanismBase
  void SetPosition(const glm::vec2 &position) override;
  glm::vec2 GetPosition() const override;
  glm::vec2 GetSize() const { return m_BoxSize; }
  void SetInitialPosition(const glm::vec2 &pos) override;
  void Respawn() override;

  // Physics methods
  void Update() override;
  void ApplyGravity();
  bool IsGrounded();

  // Character interaction methods
  void OnCollisionWithCharacter(std::shared_ptr<Character> character);
  bool IsCharacterOn(Character *character) const;
  bool CheckCharacterCollision(std::shared_ptr<Character> character);

  // Terrain collision methods
  bool CheckCollisionWithTerrain(const glm::vec2 &position);

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

  // Environment reference
  GridSystem *m_GridSystem = nullptr;
};

#endif // BOX_HPP
