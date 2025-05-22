#ifndef CHARACTER_STATE_HPP
#define CHARACTER_STATE_HPP

#include <glm/glm.hpp>

// Character state enum
enum class CharacterState {
  STANDING,
  JUMPING_ASCENDING,
  JUMPING_DESCENDING,
  MOVING,
  DEAD
};

// Character physics constants
struct CharacterPhysics {
  static constexpr float GRAVITY_SPEED = 10.0f;
  static constexpr float MAX_EXTERNAL_FORCE = 25.0f;
  static constexpr float MIN_EXTERNAL_FORCE = 0.0f;
  static constexpr float CHARACTER_OFFSET = 13.5f;
};

#endif // CHARACTER_STATE_HPP