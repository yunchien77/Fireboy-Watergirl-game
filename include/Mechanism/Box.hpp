#ifndef BOX_HPP
#define BOX_HPP

#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

class Box : public Util::GameObject {
public:
  Box();

  void SetPosition(const glm::vec2 &position);
  glm::vec2 GetPosition() const;

  void Update();
  void ApplyGravity();
  bool IsGrounded();

  void OnCollisionWithCharacter(std::shared_ptr<Character> character);
  bool CheckCollisionWithTerrain(const glm::vec2 &position);
  bool IsCharacterOn(Character *character) const;

  // 新增檢查角色碰撞的方法
  bool CheckCharacterCollision(std::shared_ptr<Character> character);

  void Draw();
  void Respawn();
  void SetInitialPosition(const glm::vec2 &pos);

  void SetGridSystem(GridSystem *grid) { m_GridSystem = grid; }

private:
  float velocityY;
  float gravity;
  float moveSpeed;
  bool grounded;
  glm::vec2 boxSize;
  glm::vec2 m_InitialPosition;

  GridSystem *m_GridSystem = nullptr;
};

#endif // BOX_HPP