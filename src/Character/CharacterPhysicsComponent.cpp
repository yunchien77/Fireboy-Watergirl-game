#include "Character/Character.hpp"
#include "Mechanism/Fan.hpp"
#include <cmath>
#include <iostream>
#include <map>

#define CHARACTER_OFFSET 13.5f

CharacterPhysicsComponent::CharacterPhysicsComponent(Character *owner)
    : m_Owner(owner) {}

void CharacterPhysicsComponent::ApplyExternalForce(float y) {
  // 添加風力
  m_ExternalForce.y += y;

  // 限制最大風力
  if (m_ExternalForce.y > CharacterPhysics::MAX_EXTERNAL_FORCE) {
    m_ExternalForce.y = CharacterPhysics::MAX_EXTERNAL_FORCE;
  }

  m_AffectedByWind = true;
}

void CharacterPhysicsComponent::ResetExternalForce() {
  m_ExternalForce = {0.0f, 0.0f};
}

const glm::vec2 &CharacterPhysicsComponent::GetExternalForce() const {
  return m_ExternalForce;
}

bool CharacterPhysicsComponent::IsAffectedByWind() const {
  return m_AffectedByWind;
}

void CharacterPhysicsComponent::SetAffectedByWind(bool affected) {
  m_AffectedByWind = affected;
}
