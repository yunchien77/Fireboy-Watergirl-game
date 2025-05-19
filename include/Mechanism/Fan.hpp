#ifndef FAN_HPP
#define FAN_HPP

#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>


class Character;

using FanColor = Color;

class Fan : public Util::GameObject {
public:
  Fan(const glm::vec2 &position, float strength = 100.0f,
      FanColor color = FanColor::WHITE);

  bool IsCharacterInWindZone(Character *character) const;
  void ApplyWindForce(Character *character);
  void SetPosition(const glm::vec2 &position);
  void Update(float deltaTime);

  const SDL_Rect &getRect() const;

  // Reset state
  void SetInitialPosition(const glm::vec2 &pos);
  void Respawn();

  // Get fan color
  FanColor GetColor() const;
  static float GetWindFloatEffect(); // 取得全局共享的風力漂浮效果

private:
  glm::vec2 m_InitialPosition;
  float m_Strength;          // Wind strength (upward force)
  float m_MaxWindHeight;     // Maximum height the wind effect reaches
  float m_AnimationTime;     // For fan blade animation
  mutable SDL_Rect m_Rect{}; // Rectangle for collision detection
  FanColor m_Color;          // Fan color

  std::string GetImagePath() const;
  static std::string GetImagePath(FanColor color);
  static float s_GlobalWindTime;
};

#endif // FAN_HPP