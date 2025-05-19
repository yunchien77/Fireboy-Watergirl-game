#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/IInteractable.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <vector>

using PlatformColor = Color;

class Character;

/**
 * @brief Platform game object that can be triggered to move and interact with
 * characters
 *
 * Platforms can be moved when triggered and can interact with characters
 * providing collision detection and movement capabilities.
 */
class Platform : public Util::GameObject,
                 public ITriggerable,
                 public ICollidable,
                 public IInteractable {
public:
  /**
   * @brief Construct a new Platform object
   *
   * @param color Color of the platform
   * @param pos Initial position
   * @param moveOffset Movement offset when triggered
   */
  Platform(PlatformColor color, const glm::vec2 &pos,
           const glm::vec2 &moveOffset);

  // ITriggerable implementation
  void OnTriggered() override;
  void OnReleased() override;

  // ICollidable implementation
  const SDL_Rect &getRect() const override;

  // IInteractable implementation
  void OnCharacterEnter(Character *character) override;
  // void OnCharacterExit(Character *character) override;

  /**
   * @brief Update platform animation and movement
   *
   * @param deltaTime Time since last frame
   * @param characters Vector of characters to check for collisions
   */
  void UpdateAnimation(float deltaTime,
                       const std::vector<Character *> &characters);

  /**
   * @brief Check if platform will collide with character below when moving
   *
   * @param character Character to check collision with
   * @param movement Platform movement vector
   * @return true if collision detected, false otherwise
   */
  bool WillCollideWithCharacterBelow(Character *character,
                                     const glm::vec2 &movement) const;

  /**
   * @brief Set platform position
   *
   * @param position New position
   */
  void SetPosition(const glm::vec2 &position);

  /**
   * @brief Get current platform position
   *
   * @return const glm::vec2& Reference to platform position
   */
  [[nodiscard]] const glm::vec2 &GetPosition() const {
    return m_Transform.translation;
  };

  /**
   * @brief Get platform color
   *
   * @return PlatformColor Platform's color
   */
  PlatformColor GetColor() const;

  /**
   * @brief Get platform movement delta
   *
   * @return glm::vec2 Movement delta from last frame
   */
  glm::vec2 GetDeltaMovement() const;

  /**
   * @brief Check if character is standing on the platform
   *
   * @param character Character to check
   * @return true if character is on platform, false otherwise
   */
  bool IsCharacterOn(Character *character) const;

  /**
   * @brief Check collision with character in specific movement direction
   *
   * @param character Character to check collision with
   * @param moveDirection Direction of movement (-1 left, 1 right)
   * @return true if collision detected, false otherwise
   */
  bool CheckCollision(Character *character, int moveDirection) const override;

  /**
   * @brief Set initial position for respawn
   *
   * @param pos Initial position
   */
  void SetInitialPosition(const glm::vec2 &pos);

  /**
   * @brief Reset platform to initial position
   */
  void Respawn();

private:
  glm::vec2 m_InitialPosition; ///< Initial spawn position
  glm::vec2 m_MoveOffset;      ///< Movement offset when triggered
  glm::vec2 m_TargetPosition;  ///< Target position when moving
  bool m_ShouldMove = false;   ///< Flag indicating if platform should move
  bool m_IsAnimating =
      false; ///< Flag indicating if platform is currently animating
  mutable SDL_Rect m_Rect{}; ///< Collision rectangle
  PlatformColor m_Color;     ///< Platform color
  glm::vec2 m_LastDeltaMovement = {0.0f,
                                   0.0f}; ///< Movement delta from last frame
  std::vector<Character *>
      m_CharactersOnPlatform; ///< Characters currently on this platform

  /**
   * @brief Get image path based on platform color
   *
   * @param color Platform color
   * @return std::string Path to platform image
   */
  std::string GetImagePath(PlatformColor color) const;
};

#endif // PLATFORM_HPP