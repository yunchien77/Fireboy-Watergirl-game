#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Interface/ITriggerable.hpp"
#include "Util/GameObject.hpp"
#include "Object/Color.hpp"
#include <glm/glm.hpp>

using PlatformColor = Color;

class Character;

class Platform : public Util::GameObject, public ITriggerable {
public:
  Platform(PlatformColor color, const glm::vec2 &pos,
           const glm::vec2 &moveOffset);

  void OnTriggered() override;
  void OnReleased() override;
  void UpdateAnimation(float deltaTime);
  void SetPosition(const glm::vec2 &position);
  // get position
  [[nodiscard]] const glm::vec2 &GetPosition() const {
    return m_Transform.translation;
  };

  PlatformColor GetColor() const;
  glm::vec2 GetDeltaMovement() const;
  bool IsCharacterOn(Character *character) const;
  const SDL_Rect &getRect() const;

private:
  glm::vec2 m_InitialPosition;
  glm::vec2 m_MoveOffset;
  glm::vec2 m_TargetPosition;
  bool m_ShouldMove = false;
  bool m_IsAnimating = false;
  mutable SDL_Rect m_Rect{};
  PlatformColor m_Color;
  glm::vec2 m_LastDeltaMovement = {0.0f, 0.0f}; // 記錄上一幀的移動距離

  std::string GetImagePath(PlatformColor color) const;
};

#endif // PLATFORM_HPP
