#ifndef ICOLLIDABLE_HPP
#define ICOLLIDABLE_HPP

#include <SDL_rect.h>

class Character;

/**
 * @brief Interface for objects that can collide with other objects
 *
 * This interface defines the contract for game objects that have
 * collision detection capabilities.
 */
class ICollidable {
public:
  /**
   * @brief Get the collision rectangle of the object
   *
   * @return const SDL_Rect& Reference to the object's collision rectangle
   */
  virtual const SDL_Rect &getRect() const = 0;

  /**
   * @brief Check if this object collides with another collidable object
   *
   * @param other The other collidable object to check collision with
   * @return true if collision detected, false otherwise
   */
  virtual bool CheckCollision(const ICollidable *other) const {
    const SDL_Rect &rectA = this->getRect();
    const SDL_Rect &rectB = other->getRect();
    return SDL_HasIntersection(&rectA, &rectB) == SDL_TRUE;
  }

  virtual bool CheckCollision(Character *character, int moveDirection) const {
    return false;
  }

  /**
   * @brief Virtual destructor
   */
  virtual ~ICollidable() = default;
};

#endif // ICOLLIDABLE_HPP