#ifndef FAN_HPP
#define FAN_HPP

#include "Mechanism/MechanismBase.hpp"
#include "Object/Color.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

using FanColor = Color;

class Fan : public MechanismBase {
public:
  Fan(const glm::vec2 &position, float strength = 100.0f,
      FanColor color = FanColor::WHITE);

  bool IsCharacterInWindZone(Character *character) const;
  void ApplyWindForce(Character *character);

  using MechanismBase::Update;
  void Update(float deltaTime);
  static float GetWindFloatEffect(); // 取得全局共享的風力漂浮效果

private:
  float m_Strength;      // Wind strength (upward force)
  float m_MaxWindHeight; // Maximum height the wind effect reaches
  float m_AnimationTime; // For fan blade animation

  static std::string GetImagePath(FanColor color);
  static float s_GlobalWindTime;
};

#endif // FAN_HPP