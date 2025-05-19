#ifndef ICOLLIDABLE_HPP
#define ICOLLIDABLE_HPP

#include <SDL_rect.h>

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
  virtual bool CheckCollision(ICollidable *other) const {
    const SDL_Rect &rect1 = this->getRect();
    const SDL_Rect &rect2 = other->getRect();
    return SDL_HasIntersection(&rect1, &rect2);
  }

  /**
   * @brief Virtual destructor
   */
  virtual ~ICollidable() = default;
};

#endif // ICOLLIDABLE_HPP