#ifndef MECHANISM_BASE_HPP
#define MECHANISM_BASE_HPP

#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>

class Character;

/**
 * @brief Base class for all game mechanisms
 *
 * Provides common functionality for all mechanisms in the game, including:
 * - Position management
 * - Color properties
 * - Respawn functionality
 * - Character interaction
 */

class MechanismBase : public Util::GameObject {
public:
  MechanismBase(const glm::vec2 &position,
                Color color = Color::NONE,
                float zIndex = 20.0f);

  virtual ~MechanismBase() = default;

  virtual void SetPosition(const glm::vec2 &position);
  glm::vec2 GetPosition() const;

  void SetInitialPosition(const glm::vec2 &pos);
  virtual void Respawn();

  virtual void Update();

  const SDL_Rect &getRect() const;

  Color GetColor() const;

  void SetZIndex(float z);
  void SetPivot(const glm::vec2 &pivot);

protected:
  void UpdateRect() const;

  glm::vec2 m_InitialPosition;
  Color m_Color;
  mutable SDL_Rect m_Rect;
};

#endif // MECHANISMBASE_HPP
