#ifndef FAN_HPP
#define FAN_HPP

#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <memory>

class Character;

class Fan : public Util::GameObject {
public:
  Fan(const glm::vec2 &position, float strength = 100.0f,
      float windWidth = 80.0f);

  void Update(float deltaTime);
  bool IsCharacterInWindZone(Character *character) const;
  void ApplyWindForce(Character *character, float deltaTime);
  void SetPosition(const glm::vec2 &position);

  // Visual methods
  void UpdateAnimation(float deltaTime);
  const SDL_Rect &getRect() const;

  // Reset state
  void SetInitialPosition(const glm::vec2 &pos);
  void Respawn();

private:
  glm::vec2 m_InitialPosition;
  float m_Strength;          // Wind strength (upward force)
  float m_WindWidth;         // Width of the wind zone
  float m_WindHeight;        // Height of the wind effect (visual only)
  float m_MaxWindHeight;     // Maximum height the wind effect reaches
  float m_AnimationTime;     // For fan blade animation
  mutable SDL_Rect m_Rect{}; // Rectangle for collision detection

  // Visual wind particles (optional)
  struct WindParticle {
    glm::vec2 position;
    float lifeTime;
    float maxLifeTime;
    float speed;
  };
  std::vector<WindParticle> m_WindParticles;
  void UpdateWindParticles(float deltaTime);
  void GenerateWindParticles();

  std::string GetImagePath() const;
};

#endif // FAN_HPP