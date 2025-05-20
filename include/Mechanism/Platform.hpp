#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Interface/ITriggerable.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/Color.hpp"
#include <glm/glm.hpp>

using PlatformColor = Color;

class Character;

class Platform : public MechanismBase, public ITriggerable {
public:
  Platform(PlatformColor color, const glm::vec2 &pos,
           const glm::vec2 &moveOffset);

  void OnTriggered() override;
  void OnReleased() override;
  void UpdateAnimation(float deltaTime,
                       const std::vector<Character *> &characters);
  bool WillCollideWithCharacterBelow(Character *character,
                                     const glm::vec2 &movement) const;

  glm::vec2 GetDeltaMovement() const;
  bool IsCharacterOn(Character *character) const;
  bool CheckCollision(Character *character, int moveDirection) const;

private:
  glm::vec2 m_MoveOffset;
  glm::vec2 m_TargetPosition;
  bool m_ShouldMove = false;
  bool m_IsAnimating = false;
  glm::vec2 m_LastDeltaMovement = {0.0f, 0.0f}; // 記錄上一幀的移動距離

  std::string GetImagePath(PlatformColor color);
};

#endif // PLATFORM_HPP