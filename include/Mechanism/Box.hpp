#ifndef BOX_HPP
#define BOX_HPP

#include "Object/GridSystem.hpp"
#include "Mechanism/MechanismBase.hpp"
#include <glm/glm.hpp>

class Character;

class Box : public MechanismBase {
public:
  Box(const glm::vec2 &pos);

  glm::vec2 GetSize() const { return boxSize; }

  void Update();
  void ApplyGravity();
  bool IsGrounded();

  void OnCollisionWithCharacter(std::shared_ptr<Character> character);
  bool CheckCollisionWithTerrain(const glm::vec2 &position);
  bool IsCharacterOn(Character *character) const;

  bool CheckCharacterCollision(std::shared_ptr<Character> character);

  void SetGridSystem(GridSystem *grid) { m_GridSystem = grid; }

private:
  float velocityY;
  float gravity;
  float moveSpeed;
  bool grounded;
  glm::vec2 boxSize;

  GridSystem *m_GridSystem = nullptr;
};

#endif // BOX_HPP
