#include "Mechanism/Fan.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include "config.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <random>

Fan::Fan(const glm::vec2 &position, float strength, FanColor color)
    : m_InitialPosition(position), m_Strength(strength),
      m_MaxWindHeight(270.0f), m_AnimationTime(0.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath()));
  SetPosition(position);
  SetPivot({0.0f, 14.0f});
  SetZIndex(20);
}

std::string Fan::GetImagePath() const { return GetImagePath(m_Color); }

std::string Fan::GetImagePath(FanColor color) {
  std::string base = RESOURCE_DIR "/material/props/fan/fan-";
  switch (color) {
  case FanColor::BLUE:
    base += "blue";
    break;
  case FanColor::GREEN:
    base += "green";
    break;
  case FanColor::PINK:
    base += "pink";
    break;
  case FanColor::ORANGE:
    base += "orange";
    break;
  case FanColor::WHITE:
    base += "white";
    break;
  case FanColor::YELLOW:
    base += "yellow";
    break;
  }
  base += ".png";
  return base;
}

FanColor Fan::GetColor() const { return m_Color; }

bool Fan::IsCharacterInWindZone(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;
  glm::vec2 fanSize = GetScaledSize();

  // Fan's left and right boundaries
  float fanLeft = fanPos.x - (fanSize.x / 2.0f);
  float fanRight = fanPos.x + (fanSize.x / 2.0f);

  // Character's left and right boundaries
  glm::vec2 charSize = character->GetSize();
  float charLeft = charPos.x - (charSize.x / 2.0f);
  float charRight = charPos.x + (charSize.x / 2.0f);

  float charWidth = charSize.x;
  float overlapAmount =
      std::min(charRight, fanRight) - std::max(charLeft, fanLeft);
  bool horizontalOverlap = (overlapAmount > (charWidth * 0.4f));

  // Check if character is above the fan
  bool isAboveFan = charPos.y > (fanPos.y - 28.0);

  return isAboveFan && horizontalOverlap;
}

void Fan::OnCharacterEnter(Character *character) {
  // Add character to the wind zone tracking list if not already there
  if (std::find(m_CharactersInWindZone.begin(), m_CharactersInWindZone.end(),
                character) == m_CharactersInWindZone.end()) {
    m_CharactersInWindZone.push_back(character);
  }
}

// void Fan::OnCharacterExit(Character *character) {
//   // Remove character from wind zone tracking list
//   auto it = std::find(m_CharactersInWindZone.begin(),
//                       m_CharactersInWindZone.end(), character);
//   if (it != m_CharactersInWindZone.end()) {
//     m_CharactersInWindZone.erase(it);

//     // Make sure the character is no longer affected by wind
//     if (character->IsAffectedByWind()) {
//       character->SetAffectedByWind(false);
//       character->ResetExternalForce();
//     }
//   }
// }

void Fan::ApplyWindForce(Character *character) {
  // If character is not in wind zone, gradually reduce wind effect until gone
  if (!IsCharacterInWindZone(character)) {
    if (character->IsAffectedByWind()) {
      character->ApplyExternalForce(-0.15f);
    }

    // If wind force becomes very small, completely cancel wind effect
    if (std::abs(character->GetExternalForce().y) < 0.05f) {
      character->SetAffectedByWind(false);
      character->ResetExternalForce();
    }
    return;
  }

  character->SetAffectedByWind(true);
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;

  // Calculate distance from fan
  float distanceFromFan = std::abs(charPos.y - (fanPos.y - 16.0f));
  if (distanceFromFan > m_MaxWindHeight) {
    character->SetAffectedByWind(false);
    return;
  }

  // Calculate distance factor (further distance = weaker wind)
  float distanceFactor =
      1.0f - std::pow(distanceFromFan / m_MaxWindHeight, 0.3f);
  float minForceFactor = 0.6f;
  distanceFactor = std::max(minForceFactor, distanceFactor);

  // Wind variation (for slight up and down floating)
  float windVariation = std::sin(m_AnimationTime * 0.001f) * 0.08f + 1.0f;

  // Adjust wind force based on ideal height
  float idealHeight = fanPos.y + m_MaxWindHeight * 0.8f;
  float heightDifference = idealHeight - charPos.y;
  float heightAdjustFactor = 1.2f;
  if (std::abs(heightDifference) > 20.0f) {
    if (heightDifference > 0) {
      heightAdjustFactor =
          1.8f; // Character below ideal height, strengthen wind
    } else {
      heightAdjustFactor = 0.9f; // Character above ideal height, weaken wind
    }
  }

  // Calculate base force
  float baseForce = m_Strength * 0.15f;

  // Smooth wind force change to avoid sudden changes
  static std::map<Character *, float> lastForces;
  float lastForce = lastForces.count(character) ? lastForces[character] : 0.0f;
  float newForce =
      baseForce * distanceFactor * windVariation * heightAdjustFactor;
  float smoothedForce = lastForce * 0.7f + newForce * 0.3f;

  // Limit wind force to reasonable range
  smoothedForce = std::min(smoothedForce, 0.65f);
  smoothedForce = std::max(smoothedForce, 0.12f);

  // Store this wind force value for next calculation's smooth transition
  lastForces[character] = smoothedForce;

  // Apply calculated wind force to character
  character->ApplyExternalForce(smoothedForce);
}

float Fan::s_GlobalWindTime = 0.0f;

void Fan::Update(float deltaTime) {
  m_AnimationTime += deltaTime * 1000.0f;
  s_GlobalWindTime += deltaTime;
}

float Fan::GetWindFloatEffect() {
  // Generate slow sine wave effect
  return std::sin(s_GlobalWindTime * 0.7f) * 0.08f + 1.0f;
}

void Fan::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

const SDL_Rect &Fan::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);

  return m_Rect;
}

void Fan::SetInitialPosition(const glm::vec2 &pos) { m_InitialPosition = pos; }

void Fan::Respawn() {
  SetPosition(m_InitialPosition);
  // Clear any characters from wind zone when respawning
  m_CharactersInWindZone.clear();
}