#ifndef MECHANISM_BASE_HPP
#define MECHANISM_BASE_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/IInteractable.hpp"
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
class MechanismBase : public Util::GameObject,
                      public IInteractable,
                      public ICollidable {
public:
  // Constructor with optional color parameter
  MechanismBase(const glm::vec2 &position, Color color = Color::WHITE,
                float zIndex = 20.0f);
  virtual ~MechanismBase() = default;

  // Position handling
  virtual void SetPosition(const glm::vec2 &position);
  virtual glm::vec2 GetPosition() const;

  // Initial position and respawn
  virtual void SetInitialPosition(const glm::vec2 &pos);
  virtual void Respawn();

  // Common update method
  virtual void Update(float deltaTime = 0.0f);

  // Color accessor
  Color GetColor() const;

  // Collision rectangle
  virtual const SDL_Rect &getRect() const override;

  // Character interaction method
  virtual void OnCharacterInteract(Character *character) = 0;

protected:
  glm::vec2 m_InitialPosition;
  Color m_Color;
  mutable SDL_Rect m_Rect{};

  // Update the internal collision rectangle
  void UpdateRect() const;
};

#endif // MECHANISM_BASE_HPP