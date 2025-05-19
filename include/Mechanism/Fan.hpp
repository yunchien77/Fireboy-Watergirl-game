#ifndef FAN_HPP
#define FAN_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/IInteractable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

using FanColor = Color;

class Fan : public Util::GameObject, public ICollidable, public IInteractable {
public:
  Fan(const glm::vec2 &position, float strength = 100.0f,
      FanColor color = FanColor::WHITE);

  virtual ~Fan() = default;

  // ICollidable interface
  const SDL_Rect &getRect() const override;

  // IInteractable interface
  void OnCharacterEnter(Character *character) override;
  // void OnCharacterExit(Character *character) override;

  // Fan-specific methods
  void ApplyWindForce(Character *character);
  void SetPosition(const glm::vec2 &position);
  void Update(float deltaTime);

  // Reset state
  void SetInitialPosition(const glm::vec2 &pos);
  void Respawn();

  // Get fan color
  FanColor GetColor() const;
  static float GetWindFloatEffect(); // Get global shared wind float effect

private:
  bool IsCharacterInWindZone(Character *character) const;

  glm::vec2 m_InitialPosition;
  float m_Strength;          // Wind strength (upward force)
  float m_MaxWindHeight;     // Maximum height the wind effect reaches
  float m_AnimationTime;     // For fan blade animation
  mutable SDL_Rect m_Rect{}; // Rectangle for collision detection
  FanColor m_Color;          // Fan color

  // Track characters in wind zone
  std::vector<Character *> m_CharactersInWindZone;

  std::string GetImagePath() const;
  static std::string GetImagePath(FanColor color);
  static float s_GlobalWindTime;
};

#endif // FAN_HPP